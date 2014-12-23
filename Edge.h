/*
 * Edge.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "Variable_Node.h"
#include "Check_Node.h"



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
#endif /* EDGE_H_ */
