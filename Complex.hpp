/*
 * Complex.hpp
 *
 *  Created on: 12 Sep, 2014
 *      Author: Mehrdad Tahernia
 */
#ifndef COMPLEX
#define COMPLEX


#include <iostream>
#include <math.h>
class complex{
public:
	double r;
	double i;
public:

	complex() : r(0),i(0) {}
	complex(const complex &c) : r(c.r),i(c.i){}
	complex(double d):r(d),i(0){}
	complex(int in):r(in),i(0){}
	complex(double R, double I):r(R),i(I){}
	complex(int R, int I):r(R),i(I){}
	operator double() { return mag(); }

	/*
	 * Complex related functions
	 */
	double mag(){
		return sqrt(r*r+i*i);
	}

	complex conjugate(){
		complex conj(*this);
		conj.i = -i;
		return conj;
	}


	/*
	 * Complex related operator overloading
	 */

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

	complex &operator=(int in){
		r = in;
		i = 0;
		return *this;
	}




	complex &operator+(const complex &c){
		static complex result;
		result.r = r + c.r;
		result.i = i + c.i;
		return result;
	}

	complex &operator+(double d){
		static complex result;
		result.r = r + d;
		result.i = i;
		return result;
	}

	complex &operator+(int in){
		static complex result;
		result.r = r + in;
		result.i = i;
		return result;
	}



	complex &operator-(const complex &c){
		static complex result;
		result.r = r - c.r;
		result.i = i - c.i;
		return result;
	}
	complex &operator-(double d){
		static complex result;
		result.r = r - d;
		result.i = i;
		return result;
	}

	complex &operator-(int in){
		static complex result;
		result.r = r - in;
		result.i = i;
		return result;
	}




	complex &operator*(const complex &c){
		static complex result;
		result.r = r * c.r - i * c.i;
		result.i = i * c.r + r * c.i;
		return result;
	}


	complex &operator*(double d){
		static complex result;
		result.r = r * d;
		result.i = i * d;
		return result;
	}

	complex &operator*(int in){
		static complex result;
		result.r = r * in;
		result.i = i * in;
		return result;
	}



	complex &operator/(const complex &c){
		static complex result;
		result.r = (r*c.r+i*c.i) / (c.r*c.r+c.i*c.i);
		result.i = (i*c.r-r*c.i) / (c.r*c.r+c.i*c.i);
		return result;
	}

	complex &operator/(double d){
		static complex result;
		result.r = r/d;
		result.i = i/d;
		return result;
	}

	complex &operator/(int in){
		static complex result;
		result.r = r/(double) in;
		result.i = i/(double) in;
		return result;
	}


	complex &operator+=(complex c) {
		*this = *this + c;
		return *this;
	}

	complex &operator+=(double d) {
		*this = *this + d;
		return *this;
	}

	complex &operator+=(int in) {
		*this = *this + in;
		return *this;
	}

	complex &operator-=(complex c) {
		*this = *this - c;
		return *this;
	}

	complex &operator-=(double d) {
		*this = *this - d;
		return *this;
	}

	complex &operator-=(int in) {
		*this = *this - in;
		return *this;
	}

	complex &operator*=(complex c) {
		*this = *this * c;
		return *this;
	}

	complex &operator*=(double d) {
		*this = *this * d;
		return *this;
	}

	complex &operator*=(int in) {
		*this = *this * in;
		return *this;
	}

	complex &operator/=(complex c) {
		*this = *this / c;
		return *this;
	}

	complex &operator/=(double d) {
		*this = *this / d;
		return *this;
	}

	complex &operator/=(int in) {
		*this = *this /(double)in;
		return *this;
	}


	bool operator>(complex c) {
		return this->mag() > c.mag();
	}

	bool operator<(complex c) {
		return this->mag() < c.mag();
	}

	bool operator>=(complex c) {
		return this->mag() >= c.mag();
	}

	bool operator<=(complex c) {
		return this->mag() <= c.mag();
	}
	bool operator==(complex c){
		return (this->r == c.r)&&(this->i == c.i);
	}

	bool operator!=(complex c){
		return (this->r != c.r)||(this->i != c.i);
	}



	bool operator>(double d) {
		return this->mag() > d;
	}

	bool operator<(double d) {
		return this->mag() < d;
	}

	bool operator>=(double d) {
		return this->mag() >= d;
	}

	bool operator<=(double d) {
		return this->mag() <= d;
	}

	bool operator==(double d){
		return (this->mag() == d);
	}

	bool operator!=(double d){
		return (this->mag() != d);
	}


};







inline complex &operator+(double a1,complex a2){
	static complex result;
	result.r = a2.r+a1;
	result.i = a2.i;
	return result;
}

inline complex &operator+(int a1,complex a2){
	static complex result;
	result.r = a2.r+a1;
	result.i = a2.i;
	return result;
}




inline complex &operator-(double a1,complex a2){
	static complex result;
	result.r = a2.r+a1;
	result.i = a2.i;
	return result;
}
inline complex &operator-(int a1,complex a2){
	static complex result;
	result.r = a2.r+a1;
	result.i = a2.i;
	return result;
}




inline complex &operator*(double a1,complex a2){
	static complex result;
	result.r = a1 * a2.r;
	result.i = a1 * a2.i;
	return result;
}

inline complex &operator*(int a1,complex a2){
	static complex result;
	result.r = (double)a1 * a2.r;
	result.i = (double)a1 * a2.i;
	return result;
}





inline complex &operator/(double a1,complex a2){
	return a1 * a2.conjugate()/(a2.r*a2.r+a2.i*a2.i);
}
inline complex &operator/(int a1,complex a2){
	return (double)a1*a2.conjugate()/(a2.r*a2.r+a2.i*a2.i);
}


inline bool operator>(double d, complex c) {
	return d > c.mag();
}

inline bool operator<(double d, complex c) {
	return d < c.mag();
}

inline bool operator>=(double d, complex c) {
	return d >= c.mag();
}

inline bool operator<=(double d, complex c) {
	return d <= c.mag();
}


inline bool operator==(double d, complex c){
	return (c.mag() == d);
}

inline bool operator!=(double d, complex c){
	return (c.mag() != d);
}


inline std::ostream &operator<<(std::ostream &s, complex c) {
	if (c.i>=0 )
		s << c.r << "+"<< c.i << "i ";
	else
		s << c.r << ""<< c.i << "i ";

	return s;
}
#endif /* COMPLEX */
