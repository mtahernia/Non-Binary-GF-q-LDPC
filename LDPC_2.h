#ifndef LDPC_2
#define LDPC_2

//#include <cstring>  // bzero
//#include <algorithm>  // std::copy needs this but it works without it. It must have been included in other heades

//#include "GFq.h"
//#include "Matrix.h" // matrix, column vector
//#include "Utils_2.h" // vector, array
#include "Channel.h"
#include "Encoding.h" // Node lists
#include "Mapping.h"
#include "Message.h"
#include "Node.h"
#include "Variable_Node.h"
#include "Check_Node.h"




/*****************************************************************************
 *
 * Graphs
 *
 *****************************************************************************/



//class complex_vector;  // FIXME: Where is the definition?




class edge {
private:
	variable_node *left_node;
	check_node *right_node;
public:
	message LeftBoundMessage, RightBoundMessage;
	GFq label;
public:
	edge() {label.RandomSelect();}

	variable_node &LeftNode() {	return *left_node; }
	check_node &RightNode() { return *right_node; }

	void set_nodes(variable_node *p_left_node, check_node *p_right_node) {
		left_node = p_left_node;
		right_node = p_right_node;
		left_node->add_edge(this);
		right_node->add_edge(this);
	}

	void Disconnect() {
		left_node->DisconnectEdge(this);
		right_node->DisconnectEdge(this);
	}
};
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

	~bipartite_graph() {
		Clear();
	}

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

/************************************************************************
 *
 * TopList - for use in greedy
 *
 ************************************************************************/
// TODO: I don't Understand
class TopList {
public:
	GFq *BestChange;
	int *VariableIndex;
	double *DistortionImprovement;

	int MaxSize, CurrentSize;
public:
	TopList() :
			MaxSize(-1), CurrentSize(-1) {
	}

	~TopList() {
		DeAllocate();
	}

	void DeAllocate() {
		if (MaxSize > -1) {
			delete BestChange;
			delete VariableIndex;
			delete DistortionImprovement;
		}
		MaxSize = -1;
	}

	void Init(int p_MaxSize) {
		if (MaxSize != p_MaxSize) {
			DeAllocate();

			BestChange = new GFq[p_MaxSize];
			VariableIndex = new int[p_MaxSize];
			DistortionImprovement = new double[p_MaxSize];
		}

		MaxSize = p_MaxSize;
		CurrentSize = 0;
	}

	void Add(GFq p_BestChange, int p_VariableIndex,
			double p_DistortionImprovement) {
		// Find place for new distortion
		int Place;
		for (Place = 0; Place < CurrentSize; Place++)
			if (p_DistortionImprovement > DistortionImprovement[Place])
				break;

		// Shift all the rest one place up
		if (CurrentSize < MaxSize)
			CurrentSize++;
		for (int i = CurrentSize - 1; i > Place; i--) {
			BestChange[i] = BestChange[i - 1];
			VariableIndex[i] = VariableIndex[i - 1];
			DistortionImprovement[i] = DistortionImprovement[i - 1];
		}

		// Place new entry
		if (Place < CurrentSize)      // If not passed last place in TopList
				{
			BestChange[Place] = p_BestChange;
			VariableIndex[Place] = p_VariableIndex;
			DistortionImprovement[Place] = p_DistortionImprovement;
		}
	}
};

#endif
