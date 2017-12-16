//
// Created by benjamin angelard on 16/12/2017.
//

#include "Topologie.h"

#include <stdio.h>
#include "HeadMemoryProcess.h"
#include "mpi.h"
#include <unistd.h>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for


/** Launch me : mpirun -np 5 --hostfile hostfile  topologi  **/
void Topologies_ring(int argc , char* argv[],int nb_membre_anneau)
{
    int cpt_ping_pong = 0;
    int rank ,size, len ;
    char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
    MPI_Init(& argc, &argv ) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size (MPI_COMM_WORLD, &size) ;
    MPI_Get_library_version ( version , &len) ;
    int cpt_tour_boucle = 0;
    while (cpt_tour_boucle < 3)
    {
        cpt_tour_boucle++;
        if (rank == 0)
        {
            printf("My rank is %i \n", rank);
        /*    MPI_Send(&cpt_ping_pong, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            usleep(1000);
            MPI_Recv(&cpt_ping_pong, 1, MPI_INT, nb_membre_anneau - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            usleep(1000);
            printf("Process 0 Ping %i\n", cpt_tour_boucle);
            cpt_tour_boucle ++;
            */
        }
        else {
            printf("My rank is %i \n", rank);
/*
            MPI_Recv(&cpt_ping_pong, 1, MPI_INT, (rank - 1) % nb_membre_anneau, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf ( "Hello, world, I am %d of %d , \n " , rank , size );
            MPI_Send(&cpt_ping_pong, 1, MPI_INT, (rank + 1) % nb_membre_anneau, 0, MPI_COMM_WORLD);
           */
        }
    }

}

int main (int argc , char* argv[])
{

    int nb_membre_anneau = 5;
    Topologies_ring(argc, argv, nb_membre_anneau);
    MPI_Finalize() ;
    return 0 ;
}

