#ifndef _TYPES_H_
#define _TYPES_H_

enum Tag
{
	DONE 	=	0,
	READ	= 	1,
	ALLOC 	= 	2,
	WRITE	=	3,
	FREE 	=	4
};

typedef struct message
{
	int 	origin;
	int 	master_origin;
	int 	master_dest;
	int 	id;
	int 	element_idx;
	int 	loop_iterations;
} msg_t;

typedef struct request_safeguard
{
	int 	id;
	msg_t 	message;
	int 	n_affected_children;
	int 	n_completed_children;
	std::vector<std::tuple<int, int>> 	*children_affiliations;
} reqsaf_t;


#endif