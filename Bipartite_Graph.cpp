/*
 * Bipartite_Graph.cpp
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */
#include "Variable_Node.h"
#include "Check_Node.h"
#include "Edge.h"
#include "Bipartite_Graph.h"

/**********************************************************************************
 *
 * Graph
 *
 **********************************************************************************/

void bipartite_graph::Reset(int p_N,   // number of variable nodes
		int lambda_degs[],             // left-degrees, terminated by -1
		double lambda_wts[],           // weights corresponding to above degrees
		int rho_degs[],       		   // right-degree, terminated by NULL
		double rho_wts[],              // weights corresponding to above degrees
		mapping &MapInUse) {
	//-----------------------------------------------------------------------
	// Clean up
	//-----------------------------------------------------------------------
	Clear();

	//-----------------------------------------------------------------------
	// Calc M, N and E
	//-----------------------------------------------------------------------
	double Ratio;     // M/N
	double Nominator, Denominator;

	Nominator = 0;
	for (int i = 0; rho_degs[i] != -1; i++)
		Nominator += rho_wts[i] / rho_degs[i];

	Denominator = 0;
	for (int j = 0; lambda_degs[j] != -1; j++)
		Denominator += lambda_wts[j] / lambda_degs[j];

	Ratio = Nominator / Denominator;

	N = p_N;
	M = (int) ceil(Ratio * N);
	E = (int) floor(N / Denominator);

	//----------------------------------------------------------------------------------------
	// Create variables, checks and edges
	//----------------------------------------------------------------------------------------
	edges = new edge[E];
	variable_nodes = new variable_node[N];
	check_nodes = new check_node[M];
	EdgeStack = new edge*[E * 2]; // Buffer used to manage memory allocation for nodes

	if ((edges == NULL) || (variable_nodes == NULL) || (check_nodes == NULL)
			|| (EdgeStack == NULL)) {
		cout << "Error allocating memory\n";
		exit(1);
	}

	//--------------------------------------------------------------------------
	// Generate sockets
	//--------------------------------------------------------------------------
	variable_node **left_sockets = new variable_node*[E];
	check_node **right_sockets = new check_node*[E];
	edge **EdgeStackPointer = EdgeStack;         // Auxiliary pointer to stack
	int node_index, socket_index, count_nodes_of_degree;

	node_index = 0;
	socket_index = 0;
	for (int i = 0; lambda_degs[i] != -1; i++)  // Loop through all left-degrees
	{
		int count_nodes_of_degree = (int) floor(
				lambda_wts[i] * E / lambda_degs[i]); // No. nodes of required degree
		for (int j = 0; j < count_nodes_of_degree; j++) // Number of nodes with required left-degree
		{
			for (int k = 0; k < lambda_degs[i]; k++) // Number of sockets for each degree
				left_sockets[socket_index++] = &variable_nodes[node_index];

			variable_nodes[node_index].AllocateEdges(EdgeStackPointer,
					lambda_degs[i]);
			variable_nodes[node_index].SetMapInUse(MapInUse);
			variable_nodes[node_index].SetID(node_index);

			node_index++;
		}
	}

	// Record no. of left sockets, may be different than E
	// due to the fact that lambda * E may not be whole numbers
	int count_left_sockets = socket_index;

	// Modify N for same reason
	N = node_index;

	// Generate right sockets
	node_index = 0;
	socket_index = 0;
	for (int i = 0; rho_degs[i] != -1; i++)    // Loop through all right-degrees
	{
		int CurrentDegree = rho_degs[i];
		count_nodes_of_degree = (int) floor(rho_wts[i] * E / CurrentDegree); // No of nodes of required degree
		for (int j = 0; j < count_nodes_of_degree; j++) {
			for (int k = 0; k < CurrentDegree; k++) // Number of sockets for each degree
				right_sockets[socket_index++] = &check_nodes[node_index];

			check_nodes[node_index].AllocateEdges(EdgeStackPointer,
					CurrentDegree);
			check_nodes[node_index].SetID(node_index);

			node_index++;
		}
	}
	// Record no. of right sockets, may be different than E
	// due to the fact that lambda * E may not be whole numbers
	int count_right_sockets = socket_index;

	// Modify M,E for same reason
	M = node_index;
	E = min(count_left_sockets, count_right_sockets);

	//----------------------------------------------------------------------------------------
	// Generate permutations
	//----------------------------------------------------------------------------------------
	time_t SEED;
	SEED = time(NULL);
	srand(SEED); // Init random seed so that each call to function returns different set of values

	cout << "Starting bipartite graph... " << SEED << " ";

	int left_index, right_index;
	for (int i = 0; i < E; i++) {
		// Randomly select socket from first E - left_index (last left_index sockets represent
		// sockets that have already been selected)
		//      cout << " i = " << i << " E = " << E;
		int attempts = 0;

		do {
			// It is important to select left_index randomly and not only right_index,
			// because there is a significance to the order of bits within a code, sometimes with
			// first or last bits better protected.  If left_index is not selected randomly,
			// the result would be a random tree, but in which lower degree left-nodes are of
			// lower index within each constituent code, contrary to complete randomness.
			left_index = uniform_random(E - i);
			//left_index = i;
			right_index = uniform_random(E - i);
			if ((attempts++) > 100) {
				cout << "Warning: cycles\n";
				break;
			}
		} while (left_sockets[left_index]->IsRightConnectedTo(
				right_sockets[right_index]));

		if (right_index >= (E - i)) {
			cout << "right index or left_index exceed range\n"
					<< "right_index = " << right_index << " left_index = "
					<< left_index << " E = " << E << " left_index = "
					<< left_index << "\n";
			exit(1);
		}

		edges[i].set_nodes(left_sockets[left_index],
				right_sockets[right_index]);

		// Overwrite current sockets with last sockets, so that they are not selected again
		right_sockets[right_index] = right_sockets[E - i - 1];
		left_sockets[left_index] = left_sockets[E - i - 1];
	}

	cout << "Done\n";

	// Clean-up
	delete [] left_sockets;
	delete [] right_sockets;
}

void bipartite_graph::Clear() {
	if (variable_nodes != NULL)
		delete [] variable_nodes;
	if (check_nodes != NULL)
		delete [] check_nodes;
	if (edges != NULL)
		delete [] edges;
	if (EdgeStack != NULL)
		delete [] EdgeStack;
}

void bipartite_graph::PrintNodes(char *title = NULL) {
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

void bipartite_graph::SaveToFile(char *filename) {
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

