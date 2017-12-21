#include "external_node.h"


ExternalNode::ExternalNode(int rank, int total_nodes)
: Node(rank)
, _total_nodes(total_nodes)
{}


void ExternalNode::run()
{
	std::cout << "[extern " << this->_rank << "] \t" << "start running" << std::endl;

	int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int data2[4] = {1, 2, 3, 4};
	auto ptr = this->malloc_rep(data, 10);
	std::cout << "[extern " << this->_rank << "] \t" << "malloc res: (" << std::get<0>(ptr) << ", " << std::get<1>(ptr) << ")" << std::endl;
	auto ptr2 = this->malloc_rep(data, 10);
	std::cout << "[extern " << this->_rank << "] \t" << "malloc res: (" << std::get<0>(ptr2) << ", " << std::get<1>(ptr2) << ")" << std::endl;
	auto ptr3 = this->malloc_rep(data2, 4);
	std::cout << "[extern " << this->_rank << "] \t" << "malloc res: (" << std::get<0>(ptr3) << ", " << std::get<1>(ptr3) << ")" << std::endl;
	auto ptr4 = this->malloc_rep(data2, 4);
	std::cout << "[extern " << this->_rank << "] \t" << "malloc res: (" << std::get<0>(ptr4) << ", " << std::get<1>(ptr4) << ")" << std::endl;
}

std::tuple<int, int> ExternalNode::malloc_rep(int *values, int length)
{
	int data_length 	= sizeof(msg_t) + length * sizeof(int);
	char *data = (char *)malloc(data_length);

	int master_dest 	= 0;

	int *values_cpy 	= (int *)(data + sizeof(msg_t));
	msg_t *message		= (msg_t *)data;

	message->origin 			= this->_rank;
	message->master_origin 		= 0;
	message->master_dest 		= -1;
	message->id 				= -1;
	message->element_idx 		= -1;
	message->loop_iterations 	= 0;

	for (int i = 0; i < length; i++) { values_cpy[i] = values[i]; }


	MPI_Send(data, data_length, MPI_CHAR, master_dest, Tag(ALLOC), MPI_COMM_WORLD);


	MPI_Status status;
	MPI_Probe(master_dest, 0, MPI_COMM_WORLD, &status);

	int recv_data_length;
	MPI_Get_count(&status, MPI_CHAR, &data_length);
	
	char *recv_data 	= 	(char *)malloc(sizeof(char) * recv_data_length);

	MPI_Recv(recv_data, recv_data_length, MPI_CHAR, master_dest, 0, MPI_COMM_WORLD, &status);

	msg_t *recv_message = (msg_t *)recv_data;

	auto ptr = std::make_tuple(recv_message->master_dest, recv_message->id);

	free(data);
	free(recv_data);

	return ptr;

}
