/*
 * Definitions.h
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


/************************************************************************
 *
 * Definitions
 *
 ************************************************************************/

#define INF             1e10	// Infinity
#define EPSILON         1e-10	// It's Obvious!

typedef unsigned char BYTE;


#define MAX_Q 	32  // Maximum allowed Q   (to be freely modified)

// It is apparently not used
#define MAX_BUF 10000

#define MAX_LAMBDAS 1000//!< Maximum number of Lambdas
#define MAX_RHOS    1000//!< Maximum number of Rhos
// If gap ends up too big, it might be necessary to modify the following 3 to 300 or larger
#define MIN_GAP   3
#define MAX_GAP   10

#define MAX_DEGREE	1000

#endif /* DEFINITIONS_H_ */
