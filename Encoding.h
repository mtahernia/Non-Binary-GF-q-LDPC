#ifndef ENCODING
#define ENCODING

#include <cstdlib>
/*********************************************************************************
 *
 * Node lists
 *
 *********************************************************************************/

class node;
class variable_node;
class check_node;

class NodeListWithoutID {
public:
	int CurrentLength;
	int MaxLength;
	node **Nodes;

public:
	NodeListWithoutID() :
			CurrentLength(0), MaxLength(0), Nodes(NULL) {
	}

	void Allocate(int p_MaxLength) {
		if (Nodes != NULL)
			delete Nodes;

		Nodes = new node *[p_MaxLength];
		MaxLength = p_MaxLength;
		CurrentLength = 0;
	}

	void Add(node &Node) {
		if (CurrentLength >= MaxLength) {
			cout << "NodeList:Add: Attempt to exceed list size\n";
			exit(1);
		}
		Nodes[CurrentLength++] = &Node;
	}
	// Extract the last node from the list
	node &ExtractAnyNode() {
		if (CurrentLength <= 0) {
			cout
					<< "NodeList:ExtractAnyNode: Attempt to extract from an empty list\n";
			exit(1);
		}
		return *Nodes[--CurrentLength];
	}

	~NodeListWithoutID() {
		if (MaxLength > 0) {
			delete Nodes;
			MaxLength = 0;
			CurrentLength = 0;
		}
	}

	int GetLength() {
		return CurrentLength;
	}

	node &operator[](int i) {
		return *Nodes[i];
	}
};


class NodeListWithID: public NodeListWithoutID {
public:
	int Systematic;
	int Gap;
public:
	void SwitchNodes(int i1, int i2);
	void Reverse()
	// Reverse the lists
	{
		for (int i1 = 0, i2 = GetLength() - 1; i1 < i2; i1++, i2--)
			SwitchNodes(i1, i2);
	}
};

class VariableNodeList: public NodeListWithID {
public:
	void Init(variable_node VariableNodeArray[], int p_Length);
	variable_node &operator[](int i) {
		return *(variable_node *) Nodes[i];
	}
};

class CheckNodeList: public NodeListWithID {
public:
	void Init(check_node CheckNodeArray[], int p_Length);
	check_node &operator[](int i) {
		return *(check_node *) Nodes[i];
	}
};

void UrbankeAlgorithmAH(NodeListWithID &Columns, NodeListWithID &Rows);

#endif
