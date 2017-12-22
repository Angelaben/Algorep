#include "external_node.h"


ExternalNode::ExternalNode(int rank, int total_nodes)
: Node(rank)
, _total_nodes(total_nodes)
{}


/** Bubble sort, best sorting algorithm, , trust me, I am an engineer **/
void ExternalNode::bubbleSort(int size, std::tuple<int, int> ptr)
{
		for (int i = 0; i < size - 1; i++)
		{
        std::cout << " Iteration i " << i << std::endl;
			for (int j = 0; j <  size - i - 1; j++) {
                //std::cout << " Recupere pour i " << i << " et j " << j << " les valeurs de j et j+1 " << j+1 << std::endl;
				int first = this->read_rep(ptr, j);
				int second = this->read_rep(ptr, j + 1);
				if (first > second) {
					this->rewrite_rep(ptr, j, second);
					this->rewrite_rep(ptr, j + 1, first);
				}
			}
	}

}
void ExternalNode::run()
{
	std::cout << "[extern " << this->_rank << "] \t" << "start running" << std::endl;

	int data[10] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
	int unbalanced[10] = { 2, 22, 0, 10, 54, 32, 108, 42, 1, 98};
	auto ptr = this->malloc_rep(unbalanced, 10);
	std::cout << "[extern " << this->_rank << "] \t" << "malloc res: (" << std::get<0>(ptr) << ", " << std::get<1>(ptr) << ")" << std::endl;
	/** Test **/
	std::cout << " Lecture du tableau envoye " << std::endl;
	for (int i = 0; i < 10; i++) {
		int toto = this->read_rep(ptr, i);
		std::cout << "readed: " << toto << std::endl;
	}
	/** Test **/
	/* std::cout << " Lecture du tableau modifie en 0 et 4 " << std::endl;
	this->rewrite_rep(ptr, 0, 32);
	this->rewrite_rep(ptr, 4, 64);
	for (int i = 0; i < 10; i++) {
		int toto = this->read_rep(ptr, i);
		std::cout << "readed: " << toto << std::endl; */
	std::cout << " Test tableau  bubblesort" <<std::endl;
	bubbleSort(10, ptr);
    std::cout << " Test tableau apres bubblesort" <<std::endl;
	for (int i = 0; i < 10; i++) {
		int toto = this->read_rep(ptr, i);
		std::cout << "readed: " << toto << std::endl;
	}
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
	MPI_Probe(master_dest, Tag(DONE), MPI_COMM_WORLD, &status);

	int recv_data_length;
	MPI_Get_count(&status, MPI_CHAR, &recv_data_length);
	
	char *recv_data 	= 	(char *)malloc(sizeof(char) * recv_data_length);

	MPI_Recv(recv_data, recv_data_length, MPI_CHAR, master_dest, 0, MPI_COMM_WORLD, &status);

	msg_t *recv_message = (msg_t *)recv_data;

	auto ptr = std::make_tuple(recv_message->master_dest, recv_message->id);

	free(data);
	free(recv_data);

	return ptr;
}

int ExternalNode::read_rep(std::tuple<int, int> ptr)
{
	return this->read_rep(ptr, 0);
}

int ExternalNode::read_rep(std::tuple<int, int> ptr, int position)
{
	int master_dest 	= 0;

	int data_length 	= sizeof(msg_t) + sizeof(int);
	char *data = (char *)malloc(data_length);
	int *values_cpy 	= (int *)(data + sizeof(msg_t));
	*values_cpy 		= 0;
	msg_t *message		= (msg_t *)data;

	message->origin 			= this->_rank;
	message->master_origin 		= 0;
	message->master_dest 		= std::get<0>(ptr);
	message->id 				= std::get<1>(ptr);
	message->element_idx 		= position;
	message->loop_iterations 	= 0;


	MPI_Send(data, data_length, MPI_CHAR, master_dest, Tag(READ), MPI_COMM_WORLD);


	MPI_Status status;
	MPI_Probe(master_dest, Tag(DONE), MPI_COMM_WORLD, &status);

	int recv_data_length;
	MPI_Get_count(&status, MPI_CHAR, &recv_data_length);
	
	char *recv_data 	= 	(char *)malloc(sizeof(char) * recv_data_length);

	MPI_Recv(recv_data, recv_data_length, MPI_CHAR, master_dest, Tag(DONE), MPI_COMM_WORLD, &status);

	int result = *(recv_data + sizeof(msg_t));

	free(data);
	free(recv_data);

	return result;
}

void ExternalNode::rewrite_rep(std::tuple<int, int> ptr, int position, int value)
{
	int master_dest 	= 0;

	int data_length 	= sizeof(msg_t) + 2 * sizeof(int);
	char *data 			= (char *)malloc(data_length);
	int *values_cpy 	= (int *)(data + sizeof(msg_t));
	msg_t *message		= (msg_t *)data;

	values_cpy[0]		=	value;
	values_cpy[1]		= 	0;

	message->origin 			= this->_rank;
	message->master_origin 		= 0;
	message->master_dest 		= std::get<0>(ptr);
	message->id 				= std::get<1>(ptr);
	message->element_idx 		= position;
	message->loop_iterations 	= 0;

	MPI_Send(data, data_length, MPI_CHAR, master_dest, Tag(WRITE), MPI_COMM_WORLD);

	MPI_Status status;
	MPI_Probe(master_dest, Tag(DONE), MPI_COMM_WORLD, &status);

	int recv_data_length;
	MPI_Get_count(&status, MPI_CHAR, &recv_data_length);
	
	char *recv_data 	= 	(char *)malloc(sizeof(char) * recv_data_length);

	MPI_Recv(recv_data, recv_data_length, MPI_CHAR, master_dest, Tag(DONE), MPI_COMM_WORLD, &status);

	free(data);
	free(recv_data);
}

void ExternalNode::free_rep(std::tuple<int, int> ptr)
{
	int master_dest 	= 0;

	int data_length 	= sizeof(msg_t);
	char *data 			= (char *)malloc(data_length);
	msg_t *message		= (msg_t *)data;

	message->origin 			= this->_rank;
	message->master_origin 		= 0;
	message->master_dest 		= std::get<0>(ptr);
	message->id 				= std::get<1>(ptr);
	message->element_idx 		= 0;
	message->loop_iterations 	= 0;

	MPI_Send(data, data_length, MPI_CHAR, master_dest, Tag(FREE), MPI_COMM_WORLD);

	free(data);
}

