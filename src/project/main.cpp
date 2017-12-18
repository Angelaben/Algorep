#include <iostream>

#include "mpi.h"
#include "topologie.h"

int main(int argc, char** argv)
{

	// int elements_bis[5] = {1, 2, 3, 4, 5};
	// int *elements = elements_bis;
	// for (int i = 0; i < 5; i++)
	// {
	// 	std::cout << elements[i] << ' ';
	// }
	// std::cout << std::endl;
	// return 0;


	// MPI initialisations
	
	int rank, size, len;

	char version[MPI_MAX_LIBRARY_VERSION_STRING];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_library_version(version, &len);


	rank = 0;

	Node *node = (rank == 0) ? (Node *)(new MasterNode(rank)) : (Node *)(new ChildNode(rank));
	node->run();

	MPI_Finalize();

	delete node;
}