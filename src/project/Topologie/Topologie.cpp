//
// Created by benjamin angelard on 16/12/2017.
//

#include <stdio.h>
#include "HeadMemoryProcess.h"
#include "mpi.h"
#include <thread>         // std::this_thread::sleep_for

void as_God(int rank, int nb_child)
{
}

/** Launch me : mpirun -np 5 --hostfile hostfile  topologi  **/
void Topologies_ring(int argc , char* argv[],int nb_membre_anneau, int nb_head)
{

    int rank ,size, len ;
    char version [MPI_MAX_LIBRARY_VERSION_STRING] ;
    MPI_Init(& argc, &argv ) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    MPI_Comm_size (MPI_COMM_WORLD, &size) ;
    MPI_Get_library_version ( version , &len) ;

    /* Nombre de fils par processus */
    int nb_process_per_head = (nb_membre_anneau) / nb_head;
    // Get the rank and size in the original communicator
    int color = rank / nb_process_per_head; // Repartis en sous groupe les processus qui seront transfomés en arbre
    MPI_Comm row_comm; /* L'id du sous groupe auquel appartient le node **/
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &row_comm);

    int row_rank, row_size; /* Rank dans son sous groupe , osef du row_size, nombre de thread dans le groupe **/
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    printf("WORLD RANK/SIZE: %d \t ROW RANK/SIZE: %d/%d\n",
           rank, row_rank, row_size);

    if (row_rank == 0)
    {
        // Master
    }
    else
    {
        // Child
    }

}

int main (int argc , char* argv[])
{

    int nb_membre_anneau = 10;
    int nb_head = 5;
    Topologies_ring(argc, argv, nb_membre_anneau, nb_head);
    MPI_Finalize() ;
    return 0 ;
}

