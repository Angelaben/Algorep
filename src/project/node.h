#ifndef _NODE_H_
#define _NODE_H_

class Node
{
public:
	Node(int rank);

	virtual void run() = 0;
protected:
	int 	_rank;
};

#endif /* _NODE_H_ */