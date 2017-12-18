#include "topologie.h"

#include <iostream>

			
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
}

/** ----------------  CHILD NODE  ------------------- **/

ChildNode::ChildNode(int rank)
: Node(rank)
, _free_space(0)
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
		
	}
}



