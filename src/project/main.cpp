#include <iostream>

#include "mpi.h"

#include "master_node.h"
#include "child_node.h"
#include "external_node.h"

#define N_NODES  	6
#define N_MASTERS	2


int main(int argc, char** argv)
{
	// MPI initialisations
	
	int rank, size, len;

	char version[MPI_MAX_LIBRARY_VERSION_STRING];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_library_version(version, &len);

	Node *node = 	(rank >= N_NODES) ? 
						(Node *)(new ExternalNode(rank, N_NODES)) 
						: 	((rank % (N_NODES / N_MASTERS) == 0) ? 
								(Node *)(new MasterNode(rank, N_NODES, N_MASTERS)) 
								: (Node *)(new ChildNode(rank)));
	node->run();

	MPI_Finalize();

	delete node;
}