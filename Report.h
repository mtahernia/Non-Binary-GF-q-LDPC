/*
 * Report.h
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <fstream>
/************************************************************************
 * Cout
 ************************************************************************/
// Operator Overloading Declaration
std::ifstream &operator>>(std::ifstream &file, double &d);
std::ifstream &operator>>(std::ifstream &file, int &num);

//=========================================================================================
class reportbuf: public std::streambuf {
public:
	int IsDirectedToErr; // bool has not yet been defined at this stage of compilation
	FILE *fp;
public:
	reportbuf() : IsDirectedToErr(/*false*/0), fp(NULL) {	}
	void OpenFile(char *FileName);
	virtual int overflow(int nCh);
	virtual int underflow();
	~reportbuf() {
		if (!IsDirectedToErr)
			fclose(fp);
	}
};
//=========================================================================================
// Here we redefine cout, and we have put ReportBuf and ReportOut into cpp file, so it won't be an inline thing
extern reportbuf ReportBuf;		// Already defined in .c file
extern std::ostream ReportOut;	// Already defined in .c file
#define cout ReportOut			// Redefine cout to report buffer that we defined
//=========================================================================================

#endif /* REPORT_H_ */
