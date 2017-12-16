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
    bool is_a_Master = rank % nb_head == 0; /* Tous les nb_head processus sont des têtes, 0 5 10 ...**/
    /* Nombre de fils par processus */
    int nb_process_per_head = (nb_membre_anneau) / nb_head;
    // Get the rank and size in the original communicator
    int color = rank / nb_process_per_head; // Repartis en sous groupe les processus qui seront transfomés en arbre
    MPI_Comm row_comm; /* L'id du sous groupe auquel appartient le node **/
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &row_comm);

    int row_rank, row_size; /* Rank dans son sous groupe , osef du row_size, nombre de thread dans le groupe **/
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    /* printf("WORLD RANK/SIZE: %d \t ROW RANK/SIZE: %d/%d\n",
           rank, row_rank, row_size); */ //Debug

    if (row_rank > 0)
    {
        // Child
        MPI_Send(&rank, 1, MPI_INT, 0, 0, row_comm);
    }
    else
    { /* Master attend que ses fils le ping **/
        for (int i = 1; i < row_size; i++)
        {
            int rank_son = 0;
            MPI_Recv(&rank_son, 1, MPI_INT, i, 0, row_comm, MPI_STATUS_IGNORE);
            std::cout << " Rank " << rank << " received son " << rank_son << std::endl;
            /** AJouter au noeud tête le rank de son  fils **/
        }
    }
    /** Make anneau **/

    if(rank == 0) // Lead
    {
        std::cout << "=======================================≠≠" << std::endl;
        std::cout << " Rank " << rank << " Send his data to " << rank + nb_head << std::endl;
        MPI_Send(&rank, 1, MPI_INT, rank + nb_head, 0, MPI_COMM_WORLD);
        int rank_pred = 0;
        std::cout << " Rank "<< rank << " Receive his data from " << rank - nb_head << std::endl;
        MPI_Recv(&rank_pred, 1, MPI_INT, nb_membre_anneau - nb_head, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if(is_a_Master)
    {
        int rank_pred = 0;
        std::cout << " Rank "<< rank << " Receive his data from " << rank - nb_head << std::endl;
        MPI_Recv(&rank_pred, 1, MPI_INT, rank - nb_head, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << " Rank " << rank << " Send his data to " << (rank + nb_head) % nb_membre_anneau  << std::endl;
        MPI_Send(&rank, 1, MPI_INT, (rank + nb_head) % nb_membre_anneau, 0, MPI_COMM_WORLD);
    }

}

int main (int argc , char* argv[])
{

    int nb_membre_anneau = 20;
    int nb_head = 5;
    Topologies_ring(argc, argv, nb_membre_anneau, nb_head);
    MPI_Finalize() ;
    return 0 ;
}

