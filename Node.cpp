/*
 * Node.cpp
 *
 *  Created on: 23 Dec, 2014
 *      Author: mehrdad
 */

#include "LDPC_2.h"
#include "Node.h"
/*************************************************************************
 *
 * Node
 *
 *************************************************************************/

void node::Disconnect() {
	// while edges remain, disconnect the first one
	while (degree > 0)
		edges[0]->Disconnect();
}


