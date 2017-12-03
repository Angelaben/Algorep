#include <stdio.h>
#include "mpi.h"

#include<stdlib.h>
/** Launch me : mpirun -np 5 --hostfile hostfile  topologi 5 **/
void Topologies_ring(int argc , char* argv[])
{
	int cpt_ping_pong = 0;
	int rank ,size, len ;
	char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
	MPI_Init(& argc, &argv ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	MPI_Get_library_version ( version , &len) ;
	printf("Value %s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
	int nb_membre_anneau = atoi(argv[1]);
	int cpt_tour_boucle = 0;
	while (cpt_tour_boucle < 10)
	{
		if (rank == 0){
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			sleep(1);
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			printf("Process 0 Ping %i\n", cpt_tour_boucle);
			cpt_tour_boucle ++;
		}
		else {
			printf("My rank is %i \n", rank);

			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, (rank - 1) % nb_membre_anneau, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			printf ( "Hello, world, I am %d of %d , (%s , %d ) \n " , rank , size , version, len);
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, (rank + 1) % nb_membre_anneau, 0, MPI_COMM_WORLD);
			sleep(1);
		}
	}
	MPI_Finalize() ;

}


void Topologies_master(int argc , char* argv[])
{
	int cpt_ping_pong = 0;
	int rank ,size, len ;
	char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
	MPI_Init(& argc, &argv ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	MPI_Get_library_version ( version , &len) ;


	printf("Value %s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
	

	int nb_membre_anneau = atoi(argv[1]);
	printf("Nb anneau %i\n", nb_membre_anneau);
	int cpt_tour_boucle = 0;
	int next_son = 1;
	while (cpt_tour_boucle < 10)
	{
		if (rank == 0){ /* Maitre */

			MPI_Send(&cpt_ping_pong, 1, MPI_INT, next_son, 0, MPI_COMM_WORLD);
			sleep(1);
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, next_son, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			next_son = (next_son + 1) % nb_membre_anneau;
			if (next_son == 0)
				next_son++;
			printf("Process 0 Ping %i\n", next_son);
			cpt_tour_boucle ++;
		}
		else {
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			printf ( "Hello, world, I am %d of %d , (%s , %d ) \n " , rank , size , version, len);
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			sleep(1);
		}
	}
	MPI_Finalize() ;

}


void Topologies_Arbuste(int argc , char* argv[])
{
	int cpt_ping_pong = 0;
	int rank ,size, len ;
	char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
	MPI_Init(& argc, &argv ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	MPI_Get_library_version ( version , &len) ;


	printf("Value %s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
	

	int nb_membre_anneau = atoi(argv[1]);
	printf("Nb anneau %i\n", nb_membre_anneau);
	int cpt_tour_boucle = 0;
	int next_son = 1;
	while (cpt_tour_boucle < 10)
	{
		if (rank == 0){ /* Maitre */

			MPI_Send(&cpt_ping_pong, 1, MPI_INT, next_son, 0, MPI_COMM_WORLD);
			sleep(1);
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, next_son, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			next_son = (next_son + 1) % nb_membre_anneau;
			if (next_son == 0)
				next_son++;
			printf("Process 0 Ping %i\n", next_son);
			cpt_tour_boucle ++;
		}
		else {
			MPI_Recv(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sleep(1);
			printf ( "Hello, world, I am %d of %d , (%s , %d ) \n " , rank , size , version, len);
			MPI_Send(&cpt_ping_pong, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			sleep(1);
		}
	}
	MPI_Finalize() ;

}

int main (int argc , char* argv[])
{
	// Topologies_ring(argc, argv);
	Topologies_master(argc, argv);
	MPI_Finalize() ;
return 0 ;
}

