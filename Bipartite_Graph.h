/*
 * Bipartite_Graph.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef BIPARTITE_GRAPH_H_
#define BIPARTITE_GRAPH_H_
#include "Report.h"
#include "Variable_Node.h"
#include "Check_Node.h"
#include "Edge.h"



////////////////////////////////////////////////////////////////////////////
//
// Bipartite graph class
//
////////////////////////////////////////////////////////////////////////////
class bipartite_graph {
public:
	int N;                                // Number of variable nodes
	int M;                                // Number of check nodes
	long E;                                // Number of edges

	variable_node *variable_nodes;
	check_node *check_nodes;
	edge *edges;

	edge **EdgeStack;    // Pointer used to manage allocation of memory to nodes
	static BOOLEAN ShouldPermuteLDPCVariables;
public:
	void Reset(
				int p_N,              // number of variable nodes
				int lambda_degs[],
				double lambda_wts[],
				int rho_degs[],
				double rho_wts[],
				mapping &MapInUse);

	bipartite_graph() :
			N(0), M(0), E(0), variable_nodes(NULL), check_nodes(NULL), edges(
					NULL), EdgeStack(NULL) {
	}

	~bipartite_graph() { Clear();}

	void Clear() {
		if (variable_nodes != NULL)
			delete [] variable_nodes;
		if (check_nodes != NULL)
			delete [] check_nodes;
		if (edges != NULL)
			delete [] edges;
		if (EdgeStack != NULL)
			delete [] EdgeStack;
	}

	void PrintNodes(char *title = NULL) {
		if (title != NULL)
			cout << " --- " << title << "\n";
		cout << "Variable Nodes: ";
		for (int i = 0; i < N; i++)
			cout << variable_nodes[i].GetDegree() << " ";
		cout << "\n";

		cout << "Check Nodes: ";
		for (int i = 0; i < M; i++)
			cout << check_nodes[i].GetDegree() << " ";
		cout << "\n";
	}

	void SaveToFile(char *filename) {
		std::ofstream OutFile(filename);

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < check_nodes[i].GetDegree(); j++) {
				if (j != 0)
					OutFile << " ";
				OutFile << check_nodes[i].GetEdge(j).LeftNode().GetID();
			}
			OutFile << "\n";
		}

	}
};
#endif /* BIPARTITE_GRAPH_H_ */
