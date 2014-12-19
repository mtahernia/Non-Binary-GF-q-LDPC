/*
 * Report.cpp
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */
#include "Report.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////
//
// Cout
//
////////////////////////////////////////////////////////////////////////////

reportbuf ReportBuf;
std::ostream ReportOut(&ReportBuf);

void reportbuf::OpenFile(char *FileName) {
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
};

int reportbuf::overflow(int nCh) {
	if (!IsDirectedToErr) {
		if (fp == NULL) {
			printf("Reportbuf::overflow:  file not opened\n");
			exit(1);
		}
		putchar(nCh);	// Display the output
		fputc(nCh, fp);	// Write the output to file buffer
		fflush(fp);		// Flush the buffer to file
	}
	else
	{			// Just write output to stderr
		fputc(nCh, stderr);
	}

	return 0;
};

int reportbuf::underflow() {
	if (!IsDirectedToErr)
		fflush(fp);
	return 0;
}
