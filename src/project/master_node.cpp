#include "master_node.h"

#include <iostream>
#include <unistd.h>
			
#define MIN(x, y) ( x > y ? y : x )
#define MAX(x, y) ( x < y ? y : x )


MasterNode::MasterNode(int rank, int total_nodes, int total_masters)
: Node(rank)
, _total_nodes(total_nodes)
, _total_masters(total_masters)
, _children_allocations_table()
, _current_requests()
, _unique_id(0)
{
	this->_n_children 					= 	(this->_total_nodes - this->_total_masters) / this->_total_masters;
	this->_children_available_space 	= 	(int *)malloc(sizeof(int) * this->_n_children);
	this->_busy_children 				= 	(bool *)malloc(sizeof(bool) * this->_n_children);

	for (int i = 0; i < this->_n_children; i++)
	{
		this->_children_available_space[i] 	= CHILD_MEMORY_SIZE;
		this->_busy_children[i] 			= false;
	}
}


MasterNode::~MasterNode()
{
	free(this->_children_available_space);
	free(this->_busy_children);
}


int MasterNode::get_unique_id()
{
	this->_unique_id = (this->_unique_id + 1 == INT_MAX) ? 0 : this->_unique_id + 1;
	return this->_unique_id;
}


bool MasterNode::are_children_busy()
{
	for (int i = 0; i < this->_n_children; i++)
	{
		if (!this->_busy_children[i]) { return false; }
	}
	return true;
}


void MasterNode::run()
{
	for (;;)
	{
		MPI_Status status;

		// Getting informations on incommin message before getting its data (sourd, tag and data length)
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int source 		= status.MPI_SOURCE;
		int tag 		= status.MPI_TAG;
		int data_length;

		MPI_Get_count(&status, MPI_CHAR, &data_length);

		// if the source is another master or a external node
		if (source == (this->_rank - (this->_n_children + 1)) + (this->_rank == 0 ? this->_total_nodes : 0) || source >= this->_total_nodes) 
		{

			char 	*data 				= 	(char *)malloc(sizeof(char) * data_length);

			MPI_Recv(data, data_length, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);

			msg_t 	*message		 	=	(msg_t *)data;
			int 	*numbers 			=	(int *)(data + sizeof(msg_t));
			int 	numbers_length 		=	(data_length - sizeof(msg_t)) / sizeof(int);


			if (message->master_origin == this->_rank)
			{
				message->loop_iterations += 1;
				if (message->loop_iterations > LOOP_LIMIT)
				{
					MPI_Send(data, data_length, MPI_CHAR, message->origin, Tag(DONE), MPI_COMM_WORLD);
					continue;
				}
			}

			if (tag == Tag(DONE))
			{
				if (message->master_origin == this->_rank)
				{
					MPI_Send(data, data_length, MPI_CHAR, message->origin, Tag(DONE), MPI_COMM_WORLD);
				}
				else
				{
					MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);
				}
			}
			if (tag == Tag(FREE))	{	this->free_memory_demand(source, data_length, data, numbers_length, numbers, message);	}
			if (tag == Tag(READ)) 	{ 	this->read_memory_demand(source, data_length, data, numbers_length, numbers, message);	}				
			if (tag == Tag(ALLOC))	{	this->allocate_memory_demand(source, data_length, data, numbers_length, numbers, message);	}
			if (tag == Tag(WRITE))	{	this->rewrite_memory_demand(source, data_length, data, numbers_length, numbers, message);	}

			free(data);
		}
		else
		{
			char 	*data 			= 	(char *)malloc(sizeof(char) * data_length + sizeof(int));
			MPI_Recv(data, data_length, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);

			int 	*numbers 		=	(int *)data;
			int 	numbers_length	=	data_length / sizeof(int);

			if (tag == Tag(READ)) 	{ 	this->read_memory_result(source, numbers_length, numbers); }
			if (tag == Tag(ALLOC))	{ 	this->allocate_memory_result(source, numbers_length, numbers); }
			if (tag == Tag(WRITE)) 	{ 	this->rewrite_memory_result(source, numbers_length, numbers); }

			free(data);
		}

	}
}

void MasterNode::free_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message)
{
	if (message->master_dest == this->_rank)	// if reading demand concern this master node
	{
		// Send reading request to the child

		if (message->element_idx >= this->_children_allocations_table.at(message->id)->size())
		{
			MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);
		}
		else
		{
			for (int i = 0; i < this->_children_allocations_table.at(message->id)->size(); i++)
			{
				int child_rank 		=	std::get<0>(this->_children_allocations_table.at(message->id)->at(i));
				int position		=	std::get<1>(this->_children_allocations_table.at(message->id)->at(i));

				MPI_Send(&position, 1, MPI_INT, child_rank, Tag(FREE), MPI_COMM_WORLD);
				this->_children_available_space[child_rank - this->_rank] -= 1;
			}
			this->_children_allocations_table.erase(this->_children_allocations_table.begin() + message->id);
		}
	}
	else 	// else we send the message to the neighboor
	{
		MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(READ), MPI_COMM_WORLD);
	}
}



void MasterNode::read_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message)
{
	if (message->master_dest == this->_rank)	// if reading demand concern this master node
	{
		// Send reading request to the child

		if (message->element_idx >= this->_children_allocations_table.at(message->id)->size())
		{
			MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);
		}
		else
		{
			int child_rank 		=	std::get<0>(this->_children_allocations_table.at(message->id)->at(message->element_idx));
			int position		=	std::get<1>(this->_children_allocations_table.at(message->id)->at(message->element_idx));
			
			int request_id		=	this->get_unique_id();
			reqsaf_t *request 	= 	(reqsaf_t *)malloc(sizeof(reqsaf_t));
			request->id 		=	request_id;
			request->message 	=	*message;

			numbers[0]			=	position;
			numbers[1]			=	request_id;

			this->_current_requests.push_back(request);
			MPI_Send(numbers, numbers_length + 1, MPI_INT, child_rank, Tag(READ), MPI_COMM_WORLD);
		}
	}
	else 	// else we send the message to the neighboor
	{
		MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(READ), MPI_COMM_WORLD);
	}
}

void MasterNode::rewrite_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message)
{
	if (message->master_dest == this->_rank)	// if reading demand concern this master node
	{
		// Send reading request to the child

		if (message->element_idx >= this->_children_allocations_table.at(message->id)->size())
		{
			MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);
		}
		else
		{
			int child_rank 		=	std::get<0>(this->_children_allocations_table.at(message->id)->at(message->element_idx));
			int position		=	std::get<1>(this->_children_allocations_table.at(message->id)->at(message->element_idx));
			
			int request_id		=	this->get_unique_id();
			reqsaf_t *request 	= 	(reqsaf_t *)malloc(sizeof(reqsaf_t));
			request->id 		=	request_id;
			request->message 	=	*message;

			numbers[1]			=	position;
			numbers[2]			=	request_id;

			this->_current_requests.push_back(request);
			MPI_Send(numbers, numbers_length + 1, MPI_INT, child_rank, Tag(WRITE), MPI_COMM_WORLD);
		}
	}
	else 	// else we send the message to the neighboor
	{
		MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(WRITE), MPI_COMM_WORLD);
	}
}

void MasterNode::rewrite_memory_result(int source, int numbers_length, int *numbers)
{
	int request_id	=	numbers[2];

	for (int i = 0; i < this->_current_requests.size(); i++)
	{
		auto request = this->_current_requests[i];

		if (request->id == request_id)
		{		
			char *data 					=	(char *)malloc(sizeof(msg_t));
			msg_t *message 				=	(msg_t *)data;
			*message 					=	request->message;


			MPI_Send(data, sizeof(msg_t), MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);

			free(data);

			this->_current_requests.erase(this->_current_requests.begin() + i);

			break;
		}
	}
}


void MasterNode::read_memory_result(int source, int numbers_length, int *numbers)
{
	int result 		=	numbers[0];
	int request_id	=	numbers[1];

	for (int i = 0; i < this->_current_requests.size(); i++)
	{
		auto request = this->_current_requests[i];

		if (request->id == request_id)
		{		
			char *data 					=	(char *)malloc(sizeof(msg_t) + 2 * sizeof(int));
			msg_t *message 				=	(msg_t *)data;
			*message 					=	request->message;
			*(data + sizeof(msg_t)) 	=	result;

			MPI_Send(data, sizeof(msg_t) + sizeof(int), MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, Tag(DONE), MPI_COMM_WORLD);

			free(data);

			this->_current_requests.erase(this->_current_requests.begin() + i);

			break;
		}
	}
}

void MasterNode::allocate_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message)
{
	// Get total memory space available
	int total_children_available_space 	= 0;
	int max_child_available_space 		= -1;
	int max_child_index 				= -1;

	// Looking for a child able to store the whole data
	for (int i = 0; i < this->_n_children; i++) 
	{
		if (!this->_busy_children[i] && this->_children_available_space[i] > max_child_available_space) 
		{
			max_child_available_space 	= 	this->_children_available_space[i];
			max_child_index				=	i;
		}
		total_children_available_space += this->_children_available_space[i];
	}

	// If it is impossible to store the data, we send it to the neighboor 
	if (numbers_length > total_children_available_space)
	{
		MPI_Send(data, data_length, MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, 2, MPI_COMM_WORLD);
	}
	// Else, we store the data in the nodes
	else
	{
		// Prepare request safegard
		int request_id					=	this->get_unique_id();
		reqsaf_t *request 				= 	(reqsaf_t *)malloc(sizeof(reqsaf_t));
		request->id 					=	request_id;
		request->message 				=	*message;
		request->message.id 			=	this->_children_allocations_table.size();
		request->n_completed_children 	=	0;
		request->children_affiliations	=	new std::vector<std::tuple<int, int>>();
		numbers[numbers_length]			=	request_id;

		// Prepare new allocation table
		auto associations = new std::vector<std::tuple<int, int>>();

		for (int i = 0; i < numbers_length; i++) { associations->push_back(std::make_tuple(0, 0)); }
		this->_children_allocations_table.push_back(associations);

		// Checking if one node can contains the whole data
		if (max_child_available_space > numbers_length)
		{
			this->_busy_children[max_child_index]	= true;

			MPI_Send(numbers, numbers_length + 1, MPI_INT, this->_rank + max_child_index + 1, Tag(ALLOC), MPI_COMM_WORLD);
			this->_children_available_space[max_child_index] -= numbers_length;

			request->n_affected_children 	= 	1;
		}
		// Else we dispatch the data between nodes
		else
		{
			int index = 0;
			int n_affected_children = 0;
			for (int i = 0; i < this->_n_children && index < numbers_length; i++)
			{
				if (!this->_busy_children[i] && this->_children_available_space[i] > 0)
				{
					int splitted_data_length = MIN(this->_children_available_space[i], numbers_length - index);
					int *splitted_data = (int *)malloc(sizeof(int) * splitted_data_length + 1);

					for (int j = 0; j < splitted_data_length; j++)
					{
						splitted_data[j] = numbers[index + j];
					}
					splitted_data[splitted_data_length] = request_id;

					int child_rank = this->_rank + i + 1;

					this->_busy_children[i]	= true;
					MPI_Send(splitted_data, splitted_data_length + 1, MPI_INT, child_rank, Tag(ALLOC), MPI_COMM_WORLD);
					
					this->_children_available_space[i] -= splitted_data_length;
					
					request->children_affiliations->push_back(std::make_tuple<int, int>(std::move(child_rank), std::move(index)));

					index += splitted_data_length;

					n_affected_children += 1;

					free(splitted_data);
				}
			}

			request->n_affected_children	=	n_affected_children;
		}

		this->_current_requests.push_back(request);
	}
}

void MasterNode::allocate_memory_result(int source, int numbers_length, int *numbers)
{
	int request_id 	=	numbers[numbers_length - 1];

	for (int i = 0; i < this->_current_requests.size(); i++)
	{
		auto request = this->_current_requests[i];

		if (request->id == request_id)
		{			
			this->_busy_children[source - this->_rank] = false;

			request->n_completed_children += 1;

			// Get the begining of splitted data for this child
			int begining = 0;

			if (request->n_affected_children > 1)
			{
				for (auto tuple : *(request->children_affiliations))
				{
					if (std::get<0>(tuple) == source) 
					{ 
						begining = std::get<1>(tuple); 
						break;
					}
				}
			}
			
			// Write the result in the data
			auto allocation_table = this->_children_allocations_table[request->message.id];
			for (int i = 0; i < numbers_length - 1; i++)
			{					
				allocation_table->at(i + begining) 
					= std::make_tuple<int, int>(std::move(source), std::move(numbers[i]));
			}

			// If the request was completed, send the result

			if (request->n_completed_children == request->n_affected_children)
			{
				request->message.master_dest = this->_rank;

				MPI_Send(&request->message, sizeof(msg_t), MPI_CHAR, (this->_rank + this->_n_children + 1) % this->_total_nodes, 0, MPI_COMM_WORLD);
			
				// Free the request
				delete request->children_affiliations;
				free(request);
				this->_current_requests.erase(this->_current_requests.begin() + i);

			}

			break;
		}
	}
}