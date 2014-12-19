#include "Utils_1.h"

#include <stdlib.h>



////////////////////////////////////////////////////////////////////////////
//
// General purpose functions
//
////////////////////////////////////////////////////////////////////////////



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


