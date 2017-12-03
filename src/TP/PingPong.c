#include <stdio.h>
#include "mpi.h"

#include<stdlib.h>
void PingPong(int argc , char* argv[])
{
	int cpt_ping_pong = 0;
	int rank ,size, len ;
	char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
	MPI_Init(& argc, &argv ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	MPI_Get_library_version ( version , &len) ;
	while (cpt_ping_pong < 10)
	{
		if (rank == 0){
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process 0 Ping %i\n", cpt_ping_pong);
			cpt_ping_pong ++;
		}
		else {
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process 1 : Pong %i\n", cpt_ping_pong);
			cpt_ping_pong ++;
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize() ;

}
int main (int argc , char* argv[])
{
	PingPong(argc, argv);
	MPI_Finalize() ;
return 0 ;
}

