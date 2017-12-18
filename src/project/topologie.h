#pragma once

#include "mpi.h"

/*         

	Architecture de la topologie
	-> Noeuds maître répartis en anneau
	-> Noeuds fils répartis en arbre de profondeur 1

         o o o  o o o
		  \|/    \|/	
	       O ---- O
	  o	  /        \   o  
	   \ /          \ /     
	o - O            O - o
	   / \          / \ 
	  o	  \        /   o
	       O ---- O
          /|\    /|\
         o o o	o o o


*/

class Node
{
public:
	Node(int rank);

	virtual void run() = 0;
protected:
	int 	_rank;
};


class MasterNode : Node
{
public:

	MasterNode(int rank);

	virtual void run();

// private:

};


#define MAX_SIZE 1000	// Memory size of each child node

class ChildNode : Node
{
public:

	ChildNode(int rank);
	
	virtual void run();

private:
	int 	_free_space;
	int 	_data[MAX_SIZE];
	bool 	_allocated_elements[MAX_SIZE];
};






void init_topologie(int masters, int children);