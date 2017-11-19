/*
 * Edge.h
 *
 *  Created on: 23 Dec, 2014
 \*      Author: Mehrdad Tahernia
 */
#ifndef EDGE_H_
#define EDGE_H_
#include "Message.h"
class variable_node;
class check_node;

class edge {
private:
	variable_node *left_node;
	check_node *right_node;
public:
	message LeftBoundMessage, RightBoundMessage;
	GFq label;
public:
	edge():left_node(NULL),right_node(NULL) {label.RandomSelect();}
	variable_node &LeftNode() {	return *left_node; }
	check_node &RightNode() { return *right_node; }
	void set_nodes(variable_node *p_left_node, check_node *p_right_node);
	void Disconnect();
};
#endif /* EDGE_H_ */
