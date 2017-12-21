#ifndef _MASTER_NODE_H_
#define _MASTER_NODE_H_

#include <vector>
#include <tuple>
#include <climits>

#include "mpi.h"

#include "macros.h"
#include "node.h"
#include "types.h"

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

class MasterNode : Node
{
public:

	MasterNode(int rank, int total_nodes, int total_masters);
	~MasterNode();

	void 	read_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message);
	void 	allocate_memory_demand(int source, int data_length, char *data, int numbers_length, int* numbers, msg_t* message);

	void 	read_memory_result(int source, int numbers_length, int *numbers);
	void	allocate_memory_result(int source, int numbers_length, int *numbers);

	int 	get_unique_id();
	bool 	are_children_busy();

	virtual void run();

private:

	int* 	_children_available_space;
	int 	_n_children;
	int 	_total_masters;
	int 	_total_nodes;
	int 	_total_children;

	std::vector<std::vector<std::tuple<int, int>> *> _children_allocations_table;
	bool* _busy_children;
	std::vector<reqsaf_t *> _current_requests;

	int 	_unique_id;
};

#endif 	/* _MASTER_NODE_H_ */
