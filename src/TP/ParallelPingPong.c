#include <stdio.h>
#include "mpi.h"

#include<stdlib.h>
void PingPong(int argc , char* argv[])
{

	int rank ,size, len ;
	char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
	MPI_Init(& argc, &argv ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	MPI_Get_library_version ( version , &len) ;
	int nb_coup = rand() % 10;

}
int main (int argc , char* argv[])
{
	PingPong(argc, argv);
	MPI_Finalize() ;
return 0 ;
}

