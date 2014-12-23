/*
 * Check_Node.h
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#ifndef CHECK_NODE_H_
#define CHECK_NODE_H_
#include "Node.h"
#include "Message.h"
#include "Mapping.h"
class channel;

message &CalcLeftboundMessage(message Vectors[], int left_index, int degree);
// --------- Check Node ----------------------
class check_node: public node {
public:
	check_node(){}
	~check_node(){/*cout<<"check_node destructor called\n";*/}
	void CalcAllLeftboundMessages();
	BOOLEAN DoesFinalEstimateViolate();
	virtual node &AdjacentNode(int index);  // returnes the adjacent variable  node to edge i
	GFq &Element(int i); // For use in encoding - treats check like row of matrix
	GFq &Value();
};

#endif /* CHECK_NODE_H_ */
