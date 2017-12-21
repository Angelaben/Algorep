#ifndef _CHILD_NODE_H_
#define _CHILD_NODE_H_

#include <vector>
#include <tuple>
#include <climits>

#include "mpi.h"

#include "macros.h"
#include "node.h"
#include "types.h"

class ChildNode : Node
{
public:

	ChildNode(int rank);
	
	virtual void run();

private:
	int 	_free_space;
	int 	_data[CHILD_MEMORY_SIZE];
	bool 	_allocated_elements[CHILD_MEMORY_SIZE];
};


#endif /* _CHILD_NODE_H_ */