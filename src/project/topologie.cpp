#include "topologie.h"

#include <iostream>
#include <unistd.h>
			
 /*
 	Comportement d'un master
	-> On attend un message de la part d'un autre noeud de l'anneau
		-> Si le tag est 0 : c'est une demande de lecture
			-> On effectue cette demande auprès du fils associé
			-> On attend sa réponse
			  	-> En renvoit le résultat
		-> Si le tag est 1 : c'est une demande d'écriture
		  	-> On effectue cette demande auprès du premier fils
		  	-> On attend une réponse
		  	-> tant que la demande est refusée, on passe au fils suivant et on recommence
		  		-> Si la demande est acceptée, on renvoit l'identifiant associé à la position de l'allocation
		  		-> Si la demande est refusée partout, on renvoit faux et on passe la main au master suivant de l'anneau
	  	-> Si le tag est 1, et que la requête a fait le tour de l'anneau : c'est le moment de splitter la donnée
		  		-> On partage la donnée autant que possible dans les fils :
		  			-> On envoit la data (tag passe à 3)
		  			-> On récupère depuis le fils la taille allouée et la taille restante
		  		-> On envoit le reste au fils suivant, et s'il n'y en a plus, au master voisin
	  	-> Si le tag est 2, on effectue la libération de la mémoire
			
 	Comportement d'un child
	-> On attend un message de la part du maitre
		-> Si le tag est 0 : c'est une demande de lecture
			-> On envoit les informations concernant la variable
		-> Si le tag est 1 : c'est une demande d'écriture
			-> Si la place est suffisante, on écrit la donnée, et on renvoit une confirmation (0 par exemple)
			-> Sinon, on retourne une information sur la taille disponible restante
		-> Si le tag est 3 : c'est une demande d'écriture forcée
			-> On écrit ce qu'on peut dans la mémoire
			-> On indique la taille du sous tableau que l'on a réussi à écrire en mémoire
		-> Si le tag est 2 : c'est une demande de libération
			 -> On libère la mémoire associée à la variable
	

	Pour chaque variable allouée, on lui ajoute la date à laquelle a été envoyée la requête.
 	Lorsque deux requêtes arrivent en même temps et veulent chacune modifier la même variable,
 	on décide de ne concerver que la requête la plus récente, et on renvoit false à la requête
 	la plus vielle pour lui indiquer que la modification n'a pas eu lieu.
 	De la même façon, si une requête arrive, avec un temps plus vieux que la dernière màj de la
 	variable, on l'ignore et on renvoit false.
*/ 

/** ----------------     NODE    -------------------- **/

Node::Node(int rank)
: _rank(rank)
{} 


/** ---------------- MASTER NODE  ------------------- **/

MasterNode::MasterNode(int rank)
: Node(rank)
{}

void MasterNode::run()
{
	std::cout << "[master " << this->_rank << "] \t" << "start running" << std::endl;


	int val = 42;
	for (;;)
	{
		MPI_Send(&val, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
		MPI_Send(&val, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
		MPI_Send(&val, 1, MPI_INT, 3, 1, MPI_COMM_WORLD);
		usleep(1000);
	}
}

/** ----------------  CHILD NODE  ------------------- **/

ChildNode::ChildNode(int rank)
: Node(rank)
, _free_space(MAX_SIZE)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		this->_data[i] = 0;
		this->_allocated_elements[i] = false;
	}

}

void ChildNode::run()
{
	std::cout << "[child " << this->_rank << "] \t" << "start running" << std::endl;

	for (;;)
	{
		MPI_Status status;

		// Get informations on incomming message before getting its data (source, tag and data length)
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int source 		= status.MPI_SOURCE;
		int tag 		= status.MPI_TAG;
		int data_length;
    	MPI_Get_count(&status, MPI_INT, &data_length);

    	std::cout << "[child " << this->_rank << "] \t" << "received " << data_length << " integer(s) from node " << source << std::endl;

    	// Getting dynamically the message
    	int *message 	=	(int *)malloc(sizeof(int) * data_length);
		MPI_Recv(message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


		if 		(tag == 0)		// Reading a variable
		{
			*message 	=	this->_allocated_elements[*message] ? this->_data[*message] : 0;
			MPI_Send(message, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
		}
		else if (tag == 1)		// Try to allocate variable
		{
			if (data_length <= this->_free_space)
			{
				for (int i = 0; i < data_length; )
				{
					for (int j = 0; j < MAX_SIZE; j++)
					{
						if (!this->_allocated_elements[j])
						{
							this->_allocated_elements[j] 	= true;
							this->_data[j] 					= *(message + i);
							this->_free_space			   -= 1;
							*(message + i)					= j;
							i++;
						}
					}
				}
				MPI_Send(message, data_length, MPI_INT, source, tag, MPI_COMM_WORLD);
			}
			else
			{
				*message = this->_free_space;
				MPI_Send(message, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
			}
		} 
		else if (tag == 2)		// Force to allocate a variable
		{
			int i;
			for ( ; i < data_length; )
			{
				for (int j = 0; j < MAX_SIZE; j++)
				{
					if (!this->_allocated_elements[j])
					{
						this->_allocated_elements[j] 	= true;
						this->_data[j] 					= *(message + i);
						this->_free_space			   -= 1;
						*(message + i)					= j;
						i++;					
					}
				}
			}
			MPI_Send(message, i, MPI_INT, source, tag, MPI_COMM_WORLD);
		}
		else if (tag == 3)		// Free space used by variables
		{
			for (int i = 0; i < data_length; i++)
			{
				this->_allocated_elements[*(message + i)]	= false
				this->_free_space					   	   += 1
			}
		}

		std::cout << "[child " << this->_rank << "] \t" << "read message from node " << source << std::endl;

		free(message);

	}
}



