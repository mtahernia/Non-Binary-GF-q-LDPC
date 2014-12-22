/*
 * Functions.h
 *
 *  Created on: 20 Dec, 2014
 *      Author: Mehrdad Tahernia
 *  Here are the simple functions used through program. They are currently inline to increase performance
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


//#include <math.h>
//#include <iostream>
//#include <string.h>
//#include <ctype.h>
#include "Report.h"
#include "Definitions.h"
/************************************************************************
 * Portability include file
 ************************************************************************/
#include "Portability.h"


/************************************************************************
 *
 * Functions
 *
 ************************************************************************/
// Returns an integer between 0 and p_max - 1
inline int uniform_random(int p_max)
{
	return (int) floor(my_rand() * (double) p_max);
}
//=========================================================================================
// Log-Likelihood Ratio
inline double LLR(double x) {
	return log((1 - x) / x);
}
//=========================================================================================
// Max of two integers
inline int max(int x, int y) {
	return (x > y) ? x : y;
}
//=========================================================================================
// Max of to doubles
inline double max(double x, double y) {
	return (x > y) ? x : y;
}
//=========================================================================================
// Definition of log function, including returning -INF instead of error for 0
inline double mylog(double x) {
	return (x != 0) ? log(x) : -INF;
}
//=========================================================================================
// Sum bits of one BYTE
inline int SumBits(BYTE b) {
	int sum = 0;
	for (; b != 0; b >>= 1)
		sum += b & 1;

	return sum;
}
//=========================================================================================
// Return the XOR of bits in one BYTE
inline int XORBits(BYTE b) {
	int sum = 0;
	for (; b != 0; b >>= 1)
		sum ^= b & 1;

	return sum;
}
//=========================================================================================
inline int SumBitsLong(long l) {
	int sum = 0;
	for (; l != 0; l >>= 1)
		sum += l & 1;

	return sum;
}
//=========================================================================================
inline double log2(double x) {
	if (x > 0)
		return log(x) / log(2.);
	else
		return -INF;
}
//=========================================================================================
// Integer log2
inline int Intlog2(int x) {
	int acc = 0;

	while (x > 1) {
		x >>= 1;
		acc += 1;
	}

	return acc;
}
//=========================================================================================
// Check whether num is a power of 2 or not
inline BOOLEAN IsPowerOfTwo(int num) {
	BOOLEAN Reply = TRUE;

	for (int i = num; i > 2; i = i / 2) {
		if ((i % 2) != 0) {
			Reply = FALSE;
			break;
		}
	}

	return Reply;
}
//=========================================================================================
// Checks if char c is a digit or not
inline BOOLEAN isnumber(char c) {
	return (('0' <= c) && (c <= '9'));
}
//=========================================================================================
// X mod Y
inline double mod(double x, double y) {
	return x - floor(x / y) * y;
}
//=========================================================================================
// Minimum of two doubles x and y
inline double min_double(double x, double y) {
	return (x < y) ? x : y;
}
//=========================================================================================
inline BOOLEAN is_double_digit(int c) {
	return (isdigit(c) || (c == '.') || (c == '-') || (c == '+') || (c == 'e'));
}
//=========================================================================================



#endif /* FUNCTIONS_H_ */
