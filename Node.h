/*
 * Node.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: Mehrdad Tahernia
 */
#ifndef NODE_H_
#define NODE_H_

#include <cstdlib>
#include "Report.h"

class edge;//#include "LDPC_2.h" // temporary declaration, to enable the use of the class in further declaration

class node {
public:
	int id;   			// number used for identification
	int degree;			// FIXME:
	int AuxDegree;      // Auxiliary variable for use in encoder generator
	edge **edges;
	int MaxEdges;

public:
	node() : id(-1), degree(-1), AuxDegree(-1), edges(NULL) ,MaxEdges(-1) {}
	virtual ~node()	{/*cout << "node destructor called\n";*/}

	void SetID(int p_id) { id = p_id; }
	int GetID() { return id; }
	void Disconnect();  // Disconnect all edges
	void DisconnectEdge(edge *e) {
		int index = 0;
		// Find the edge with given pointer
		for (; index < degree; index++)
			if (edges[index] == e)
				break;
		// if the above for loop exited normally, not with break, then the node was not found
		if (index == degree) {
			cout << "node::DisconnectEdge: Attempt to disconnect a nonexistent edge\n";
			exit(1);
		}
		// Remove e from the list
		for (int i = index + 1; i < degree; i++)
			edges[i - 1] = edges[i];
		degree--;
	}

	void add_edge(edge *e) {
		if (degree == -1) {
			cout << "Edges not yet allocated, aborting.  \n";
			exit(1);
		}
		edges[degree++] = e;
		if (degree > MaxEdges) {
			cout << "MaxEdges exceeded! " << " degree = " << degree
					<< " MaxEdges = " << MaxEdges << "\n";
		}
	}

	// This pops p_MaxEdges edges from Stack and assigns it to the node
	void AllocateEdges(edge **&EdgeStackPointer, int p_MaxEdges) {
		degree = 0;    							// FIXME:Why? Indicate edges have been allocated
		edges = EdgeStackPointer;
		EdgeStackPointer += p_MaxEdges;        // Advance allocation pointer
		MaxEdges = p_MaxEdges;
	}

	int GetDegree() {
		return degree;
	}
	edge &GetEdge(int index) {
		return *edges[index];
	}

	virtual node &AdjacentNode(int index) = 0;
};


#endif /* NODE_H_ */
