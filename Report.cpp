/*
 * Report.cpp
 *
 *  Created on: 18 Dec, 2014
 *      Author: Mehrdad Tahernia
 */

#include "Report.h"	   //reportbuf
#include "Functions.h" //isdoubledigit
#include <string.h>    //strcmp
////////////////////////////////////////////////////////////////////////////
// Cout
////////////////////////////////////////////////////////////////////////////
reportbuf ReportBuf;
std::ostream ReportOut(&ReportBuf);

//=========================================================================================
void reportbuf::OpenFile(char *FileName) {
		// Make lowercase copy of FileName
		char buffer[1000];
		int i;
		for (i = 0; FileName[i] != '\0'; i++)
			buffer[i] = tolower(FileName[i]);
		buffer[i] = '\0';

		// If Filename is "err"
		if (strcmp(buffer, "err") == 0)
			IsDirectedToErr = /*true*/1;
		else
			fp = fopen(FileName, "w");
};
//=========================================================================================
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
//=========================================================================================
int reportbuf::underflow() {
	if (!IsDirectedToErr)
		fflush(fp);
	return 0;
}

//=========================================================================================
//=========================================================================================
// These operator overloads are used in reading configurations from file
// Operator Overload for >> (reads the first number in file and writes it in d)
std::ifstream &operator>>(std::ifstream &file, double &d) {
	static char buffer[100];
	int i = 0;

	// Skip until you reach to a number
	while (!is_double_digit(file.peek()))
		file.get();
	// Read the number until end of that number
	do
		file >> buffer[i++];
	while (is_double_digit(file.peek()));

	// Specify end of the string read in previous part
	buffer[i] = '\0';


	d = atof(buffer);  //Convert string to double
	return file;
}
//=========================================================================================
// Operator Overload for >> (reads the first number in file and writes it in d) for integers
std::ifstream &operator>>(std::ifstream &file, int &num) {
	static char buffer[100];
	int i = 0;

	while (!is_double_digit(file.peek()) || (file.peek() == 'e'))
		file.get();

	do
		file >> buffer[i++];
	while (is_double_digit(file.peek()));

	buffer[i] = '\0';

	num = atoi(buffer);

	return file;
}
//=========================================================================================
