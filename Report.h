/*
 * Report.h
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <math.h>
#include <iostream>
#include <fstream>
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
	reportbuf() : IsDirectedToErr(/*FALSE*/0), fp(NULL) {	}

	void OpenFile(char *FileName);
	virtual int overflow(int nCh);
	virtual int underflow();

	~reportbuf() {
		if (!IsDirectedToErr)
			fclose(fp);
	}

};


extern reportbuf ReportBuf;		// Already defined in .c file
extern std::ostream ReportOut;	// Already defined in .c file

#define cout ReportOut			// Redefine cout to report buffer that we defined

#endif /* REPORT_H_ */
