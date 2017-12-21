#include <iostream>

#include "mpi.h"

#include "master_node.h"
#include "child_node.h"
#include "external_node.h"



int main(int argc, char** argv)
{
	// MPI initialisations
	
	int rank, size, len;

	char version[MPI_MAX_LIBRARY_VERSION_STRING];
	MPI_Init(&argc, &argv);
    std::cout << "[USAGE]  mpirun -np 5 --hostfile hostfile  Executable N_NODES N_MASTERs "<<std::endl;
    if (argc < 3)
    {
        std::cout << "[ERROR] Invalid parameters, requires N_NODES and N_MASTERS " << std::endl;
        return -1;
    }
    int N_NODES = std::atoi(argv[1]);
    int N_MASTERS = std::atoi(argv[2]);
	for (int i = 0; i < argc; i++)
	{
		std::cout << "argv i " << i << " " << argv[i] << std::endl;
	}
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