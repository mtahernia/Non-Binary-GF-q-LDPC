/*
 * Complex.hpp
 *
 *  Created on: 12 Sep, 2014
 *      Author: Mehrdad Tahernia
 */
#include <iostream>
#include <math.h>

#ifndef COMPLEX_HPP_
#define COMPLEX_HPP_

class complex{
public:
	double r;
	double i;
public:
	complex() : r(0),i(0) {}
	complex(const complex &c) : r(c.r),i(c.i){}
	complex(double d):r(d),i(0){}
	complex(double R, double I):r(R),i(I){}

	double mag(){
		return sqrt(r*r+i*i);
	}

	complex conjugate(){
		complex conj(*this);
		conj.i = -i;
		return conj;
	}

	complex &operator=(const complex &c){
			r = c.r;
			i = c.i;
			return *this;
		}

	complex &operator=(double d){
			r = d;
			i = 0;
			return *this;
		}

	complex &operator+(const complex &c){
			static complex result;
			result.r = r + c.r;
			result.i = i + c.i;
			return result;
		}

	complex &operator-(const complex &c){
			static complex result;
			result.r = r - c.r;
			result.i = i - c.i;
			return result;
		}

	complex &operator*(const complex &c){
			static complex result;
			result.r = r * c.r - i * c.i;
			result.i = i * c.r + r * c.i;
			return result;
		}

	complex &operator/(const complex &c){
			static complex result;
			result.r = (r*c.r+i*c.i) / (c.r*c.r+c.i*c.i);
			result.i = (i*c.r-r*c.i) / (c.r*c.r+c.i*c.i);
			return result;
		}


};

complex &operator+(double a1,complex a2){
	static complex result;
	result.r = a2.r+a1;
	result.i = a2.i;
	return result;
}



#endif /* COMPLEX_HPP_ */
