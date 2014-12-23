/*
 * Edge.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */
#include "Edge.h"

void edge::Disconnect() {
	left_node->DisconnectEdge(this);
	right_node->DisconnectEdge(this);
}

void edge::set_nodes(variable_node *p_left_node, check_node *p_right_node) {
	left_node = p_left_node;
	right_node = p_right_node;
	left_node->add_edge(this);
	right_node->add_edge(this);
}
