#pragma once

#include <iostream>
#include <string>
#include "macros_default.h"

typedef std::string string;

//	A fraction, e.g. 5/3
//		Stores the numerator and denominator of the fraction: 5 and 3.
//	To do		
//		- test all functions
//		- optimise
//		- add getters/setters?
template <class T = int>
class fraction {
private:
	T num, den;				// "num"erator and "den"ominator
	void swap(fraction &rhs)
	{
		std::swap(num, rhs.num);
		std::swap(den, rhs.den);
	}

public:
	fraction() : num(0), den(0) {}
	fraction(const T &numerator, const T &denominator) : num(numerator), den(denominator) {}
	fraction(const fraction& f) : num(f.num), den(f.den) {}
	fraction(fraction&& f) { swap(f); }

	// Accessors



	// Functions

	void simplify()
	{
		int max = std::ceil(num / 2);
		for (int i = 2; i < (max+1); ++i)
		{
			while (num % i == 0 && den % i == 0)
			{
				num /= i; den /= i;
			}
			max = std::ceil(num / 2);
		}
	}
	fraction& invert() { std::swap(num, den); return *this;	}
	fraction& negate() { num = -num; return *this; }
	fraction& power(const fraction n)
	{
		num = std::pow(num, n.num);
		num = std::pow(num, n.den);
		den = std::pow(den, n.num);
		den = std::pow(den, n.den);
	}
	template <class number>	fraction& power(const number n)
	{
		num = std::pow(num, n);
		den = std::pow(den, n);
	}

	// Type casts

	string to_string() const
	{
		if (den == 1) return std::to_string(num);
		else {
			return std::to_string(num) + "/" + std::to_string(den);
		}
	}
	double to_double() const { return (double)num / (double)den; }
	float  to_float () const { return (float )num / (float )den; }
	int    to_int   () const { return (int   )num / (int   )den; }
	
	operator int   () const { return to_int   (); }
	operator float () const { return to_float (); }
	operator double() const { return to_double(); }
	
	// Operator overloads

	bool operator == (const fraction& rhs) const { return (num == rhs.num) && (den == rhs.den); }
	bool operator != (const fraction& rhs) const { return !operator==(rhs); }
	bool operator <  (const fraction& rhs) const { return to_float() < rhs.to_float(); }
	bool operator >  (const fraction& rhs) const { return to_float() > rhs.to_float(); }
	bool operator <= (const fraction& rhs) const { return !operator>(rhs); }
	bool operator >= (const fraction& rhs) const { return !operator<(rhs); }

	bool operator == (const int& rhs) const { return to_float() == (float)rhs; }
	bool operator != (const int& rhs) const { return !operator==(rhs); }
	bool operator <  (const int& rhs) const { return to_float() < (float)rhs; }
	bool operator >  (const int& rhs) const { return to_float() > (float)rhs; }
	bool operator <= (const int& rhs) const { return !operator>(rhs); }
	bool operator >= (const int& rhs) const { return !operator<(rhs); }

	bool operator == (const float& rhs) const { return to_float() == rhs; }
	bool operator != (const float& rhs) const { return !operator==(rhs); }
	bool operator <  (const float& rhs) const { return to_float() < rhs; }
	bool operator >  (const float& rhs) const { return to_float() > rhs; }
	bool operator <= (const float& rhs) const { return !operator>(rhs); }
	bool operator >= (const float& rhs) const { return !operator<(rhs); }

	fraction operator-() const
	{
		fraction<T> f;
		f.num = -num;
		f.den = den;
		return f;
	}
	fraction& operator++() {
		++num; ++den;
		return *this;
	}
	fraction& operator--() {
		--num; --den;
		return *this;
	}
	const fraction operator++(int unused) {
		fraction result(*this);
		++(*this);
		return result;
	}
	const fraction operator--(int unused) {
		fraction result(*this);
		--(*this);
		return result;
	}

	fraction& operator  = (fraction& rhs) { num = rhs.num; den = rhs.den; return *this; }
	fraction& operator += (const fraction& rhs) {
		T rhs_den = rhs.den;
		if (den == rhs_den) num += rhs.num;
		else
		{
			num *= rhs_den;
			num += rhs.num * den;
			den *= rhs_den;
		}
		return *this; 
	}
	fraction& operator -= (const fraction& rhs) { *this += -rhs; return *this; }
	fraction& operator *= (const fraction& rhs) { num *= rhs.num; den *= rhs.den; return *this; }
	fraction& operator /= (const fraction& rhs) { num *= rhs.den; den *= rhs.num; return *this; }

	fraction& operator  = (const T &rhs) { num = rhs; den = 1; return *this; }
	fraction& operator *= (const T &rhs)
	{
		num *= rhs;
		return *this;
	}
	fraction& operator /= (const T &rhs)
	{
		num /= rhs;
		return *this;
	}
	fraction& operator += (const T &rhs)
	{
		num += rhs*den;
		return *this;
	}
	fraction& operator -= (const T &rhs)
	{
		*this += -rhs;
		return *this;
	}

};

// Operator overloads : rhs arithmetic

template <class T> fraction<T> operator+ (fraction<T> lhs, const T& rhs) { lhs += rhs; return lhs; }
template <class T> fraction<T> operator- (fraction<T> lhs, const T& rhs) { lhs -= rhs; return lhs; }
template <class T> fraction<T> operator* (fraction<T> lhs, const T& rhs) { lhs *= rhs; return lhs; }
template <class T> fraction<T> operator/ (fraction<T> lhs, const T& rhs) { lhs /= rhs; return lhs; }

template <class T> fraction<T> operator+ (const T& lhs, fraction<T> rhs) { rhs += lhs; return rhs; }
template <class T> fraction<T> operator- (const T& lhs, fraction<T> rhs) { rhs -= lhs; return rhs; }
template <class T> fraction<T> operator* (const T& lhs, fraction<T> rhs) { rhs *= lhs; return rhs; }
template <class T> fraction<T> operator/ (const T& lhs, fraction<T> rhs) { rhs /= lhs; return rhs; }

template <class T> fraction<T> operator+ (fraction<T> lhs, const fraction<T> & rhs) { lhs += rhs; return lhs; }
template <class T> fraction<T> operator- (fraction<T> lhs, const fraction<T> & rhs) { lhs -= rhs; return lhs; }
template <class T> fraction<T> operator* (fraction<T> lhs, const fraction<T> & rhs) { lhs *= rhs; return lhs; }
template <class T> fraction<T> operator/ (fraction<T> lhs, const fraction<T> & rhs) { lhs /= rhs; return lhs; }

template <class T> std::ostream& operator << (std::ostream& os, const fraction<T>& rhs) {
	os << rhs.to_string();
	return os;
}
template <class T> std::istream& operator >> (std::istream& in, fraction<T>& rhs) {
	T x, y;

	// read in x
	in >> x;
	num = x;

	// skip delimiter
	in.ignore(1);

	// read y
	in >> y;
	den = y;

	return in;
}

