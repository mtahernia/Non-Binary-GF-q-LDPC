#ifndef UTILS_1
#define UTILS_1

#include <math.h>
#include <iostream>
#include <string.h>

/************************************************************************
 *
 * Cout
 *
 ************************************************************************/

class reportbuf: public std::streambuf {
public:
	int IsDirectedToErr; // BOOLEAN has not yet been defined at this stage of compilation
	FILE *fp;
public:
	reportbuf() :
			IsDirectedToErr(/*FALSE*/0), fp(NULL) {
	}

	void OpenFile(char *FileName) {
		// Make lowercase copy of FileName
		char buffer[1000];
		int i;
		for (i = 0; FileName[i] != '\0'; i++)
			buffer[i] = tolower(FileName[i]);
		buffer[i] = '\0';

		// If Filename is "err"
		if (strcmp(buffer, "err") == 0)
			IsDirectedToErr = /*TRUE*/1;
		else
			fp = fopen(FileName, "w");
	}

	virtual int overflow(int nCh) {
		if (!IsDirectedToErr) {
			if (fp == NULL) {
				printf("Reportbuf::overflow:  file not opened\n");
				exit(1);
			}

			putchar(nCh);
			fputc(nCh, fp);

			fflush(fp);
		} else {
			fputc(nCh, stderr);
		}

		return 0;
	}

	virtual int underflow() {
		if (!IsDirectedToErr)
			fflush(fp);

		return 0;
	}

	~reportbuf() {
		if (!IsDirectedToErr)
			fclose(fp);
	}

};

extern std::ostream ReportOut;
extern reportbuf ReportBuf;
#define cout ReportOut

/************************************************************************
 *
 * Definitions
 *
 ************************************************************************/

#define INF             1e10
#define EPSILON         1e-10

#define TRUE   1
#define FALSE  0

typedef unsigned char BYTE;
typedef unsigned char BOOLEAN;

#define MAX_Q 	64  // Maximum allowed Q   (to be freely modified)

/************************************************************************
 *
 * Functions
 *
 ************************************************************************/

std::ifstream &operator>>(std::ifstream &file, double &d);
std::ifstream &operator>>(std::ifstream &file, int &num);
int uniform_random(int p_max);

// Log-Likelihood Ratio
inline double LLR(double x) {
	return log((1 - x) / x);
}

// Max of two integers
inline int max(int x, int y) {
	return (x > y) ? x : y;
}

// Max of to doubles
inline double max(double x, double y) {
	return (x > y) ? x : y;
}

// Definition of log function, including returning -INF instead of error for 0
inline double mylog(double x) {
	return (x != 0) ? log(x) : -INF;
}

// Sum bits of one BYTE
inline int SumBits(BYTE b) {
	int sum = 0;
	for (; b != 0; b >>= 1)
		sum += b & 1;

	return sum;
}

// Return the XOR of bits in one BYTE
inline int XORBits(BYTE b) {
	int sum = 0;
	for (; b != 0; b >>= 1)
		sum ^= b & 1;

	return sum;
}

inline int SumBitsLong(long l) {
	int sum = 0;
	for (; l != 0; l >>= 1)
		sum += l & 1;

	return sum;
}

inline double log2(double x) {
	if (x > 0)
		return log(x) / log(2.);
	else
		return -INF;
}

inline int Intlog2(int x) {
	int acc = 0;

	while (x > 1) {
		x >>= 1;
		acc += 1;
	}

	return acc;
}

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

inline BOOLEAN isnumber(char c) {
	return (('0' <= c) && (c <= '9'));
}

inline double mod(double x, double y) {
	return x - floor(x / y) * y;
}

inline double min_double(double x, double y) {
	return (x < y) ? x : y;
}

/************************************************************************
 *
 * Portability include file
 *
 ************************************************************************/

#include "Portability.h"

#endif /*for the if in the start of this file*/
