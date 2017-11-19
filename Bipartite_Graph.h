/*
 * Bipartite_Graph.h
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */

#ifndef BIPARTITE_GRAPH_H_
#define BIPARTITE_GRAPH_H_
//#include "Report.h"


class variable_node;//#include "Variable_Node.h"
class check_node;//#include "Check_Node.h"
class edge;//#include "Edge.h"

////////////////////////////////////////////////////////////////////////////
//
// Bipartite graph class
//
////////////////////////////////////////////////////////////////////////////
class bipartite_graph {
public:
	//Variables:
	int N;                                // Number of variable nodes
	int M;                                // Number of check nodes
	long E;                                // Number of edges
	variable_node *variable_nodes;
	check_node *check_nodes;
	edge *edges;
	edge **EdgeStack;    // Pointer used to manage allocation of memory to nodes
	static bool ShouldPermuteLDPCVariables;

public:
	//Constructors:
	bipartite_graph() :
			N(0), M(0), E(0),
			variable_nodes(NULL),
			check_nodes(NULL),
			edges(NULL),
			EdgeStack(NULL) {	}
	// Destructor:
	~bipartite_graph() { Clear();}

	void Reset(	int p_N,              // number of variable nodes
				int lambda_degs[],
				double lambda_wts[],
				int rho_degs[],
				double rho_wts[],
				mapping &MapInUse);

	void Clear();
	void PrintNodes(char *title);
	void SaveToFile(char *filename);
};

#endif /* BIPARTITE_GRAPH_H_ */
