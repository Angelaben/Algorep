#include "child_node.h"

ChildNode::ChildNode(int rank)
: Node(rank)
, _free_space(CHILD_MEMORY_SIZE)
{
	for (int i = 0; i < CHILD_MEMORY_SIZE; i++)
	{
		this->_data[i] = 0;
		this->_allocated_elements[i] = false;
	}
}

void ChildNode::run()
{
	for (;;)
	{
		MPI_Status status;

		// Get informations on incomming message before getting its data (source, tag and data length)
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int source 		= status.MPI_SOURCE;
		int tag 		= status.MPI_TAG;
		int data_length;
    	MPI_Get_count(&status, MPI_INT, &data_length);

    	// Getting dynamically the message
    	int *data 	=	(int *)malloc(sizeof(int) * data_length);
    	int request_id 	=	data[data_length - 1];
		MPI_Recv(data, data_length, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		if 		(tag == Tag(READ))		// Reading a variable
		{
			*data 	=	this->_allocated_elements[*data] ? this->_data[*data] : 0;
			MPI_Send(data, data_length, MPI_INT, source, tag, MPI_COMM_WORLD);
		}
		else if (tag == Tag(WRITE))
		{
			this->_data[data[1]] = data[0];
			MPI_Send(data, data_length, MPI_INT, source, tag, MPI_COMM_WORLD);
		}
		else if (tag == Tag(ALLOC))		// Allocate a variable
		{
			int i = 0;
			for (int j = 0; j < CHILD_MEMORY_SIZE && i < data_length - 1; j++)
			{
				if (!this->_allocated_elements[j])
				{
					this->_allocated_elements[j] 	= true;
					this->_data[j] 					= *(data + i);
					this->_free_space			   -= 1;
					*(data + i)						= j;
					i++;					
				}
			}

			MPI_Send(data, data_length, MPI_INT, source, tag, MPI_COMM_WORLD);
		}
		else if (tag == Tag(FREE))		// Free space used by variables
		{
			this->_allocated_elements[*data]	= false;
			this->_free_space					   	   += 1;
		}

		free(data);
	}
}