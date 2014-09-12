#include <iostream>
#include "Complex.hpp"

int main(){
	complex A(1,2);
	complex B,C,D;
	double a=5.0;

	B = A.conjugate();
	C = A*B;
	C = C/B;
	C = a * C ;
	std::cout << A.r << "," << A.i << "\n"
			  << B.r << "," << B.i << "\n"
			  << C.r << "," << C.i << std::endl;
	return 0;
}

