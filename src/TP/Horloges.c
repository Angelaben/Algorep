#include <stdio.h>
#include "mpi.h"
#include <unistd.h>
#include <time.h>
#include<stdlib.h>
void Topologies_ring_lamport(int argc , char* argv[])
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

	int estampille = 0;
	srand(time(NULL));   // should only be called once
	int estampille_received = 0;
	while (cpt_tour_boucle < 10)
	{
		if (rank == 0){
			MPI_Send(&estampille, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

			int r = rand() % 5; 
			if (r == 4)
				estampille ++;
			MPI_Recv(&estampille_received, 1, MPI_INT, nb_membre_anneau - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (estampille_received > estampille)
				estampille = estampille_received;
			printf("Process 0 received  %i and had %i \n", estampille_received, estampille);
			cpt_tour_boucle ++;
		}
		else {
			printf("My rank is %i \n", rank);

			MPI_Recv(&estampille_received, 1, MPI_INT, (rank - 1) % nb_membre_anneau, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (estampille_received > estampille)
				estampille = estampille_received;
			printf("Process %i received  %i and had %i \n", rank, estampille_received, estampille);
			MPI_Send(&estampille, 1, MPI_INT, (rank + 1) % nb_membre_anneau, 0, MPI_COMM_WORLD);
			int r = rand() % 5; 
			if (r == 4)
				estampille ++;

		}
	}
	MPI_Finalize() ;

}


void Topologies_ring_mattern(int argc , char* argv[])
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

	int estampille = 0;
	srand(time(NULL));   // should only be called once
	int estampille_received = 0;
	while (cpt_tour_boucle < 10)
	{
		if (rank == 0){
			MPI_Send(&estampille, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

			int r = rand() % 5; 
			if (r == 4)
				estampille ++;
			MPI_Recv(&estampille_received, 1, MPI_INT, nb_membre_anneau - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (estampille_received > estampille)
				estampille = estampille_received;
			printf("Process 0 received  %i and had %i \n", estampille_received, estampille);
			cpt_tour_boucle ++;
		}
		else {
			printf("My rank is %i \n", rank);

			MPI_Recv(&estampille_received, 1, MPI_INT, (rank - 1) % nb_membre_anneau, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (estampille_received > estampille)
				estampille = estampille_received;
			printf("Process %i received  %i and had %i \n", rank, estampille_received, estampille);
			MPI_Send(&estampille, 1, MPI_INT, (rank + 1) % nb_membre_anneau, 0, MPI_COMM_WORLD);
			int r = rand() % 5; 
			if (r == 4)
				estampille ++;

		}
	}
	MPI_Finalize() ;

}

int main (int argc , char* argv[])
{
	Topologies_ring_lamport(argc, argv);
	MPI_Finalize() ;
return 0 ;
}

