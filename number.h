#pragma once

#include "unit.h"
#include "macros_default.h"

/*
	To do:
		- enforce STRICT_UNITS flag
		- add option to print relative uncertainty
		- maybe store fractional unc
		- negation operator
*/

// Flags

#define STRICT_UNITS		1		// Arguments of logs, powers and trigonometric functions must be unitless.
#define KILL_UNITS			1		// Remove all unit calculations and checks, thus speeding up the program.
#define KILL_UNCERTAINTY	0		// Remove all uncertainty calculations, thus speeding up the program.

// Macros

#if STRICT_UNITS
	#define CHECK_UNITS(unit, str) assert(unit.to_string() == str)
#else:
	#define CHECK_UNITS(unit, str)
#endif

#if KILL_UNITS
	#define HIDE_UNIT(x)
#else:
	#define HIDE_UNIT(x) x
#endif

#if KILL_UNCERTAINTY
	#define HIDE_UNC(x)
#else:
	#define HIDE_UNC(x) x
#endif


// A number which also keeps tracks of it's uncertainty and it's units.
//
//
template <class T>
class number {

private:
	T num;
	double unc;
	unit u;

public:
	number() : num(0), unc(0), u() {}
	number(const T &number, const double &uncertainty, const unit &unit) : num(number), unc(uncertainty), u(unit) {}
	number(const T &number, const double &uncertainty, const std::string &unit) : num(number), unc(uncertainty), u(unit) {}

	// Functions

	const T    & get_number	     () const { return num; }
	const float& get_uncertainty () const { return unc; }
	const unit & get_unit		 () const { return u; }
	double fractional_uncertainty() const { return unc / num; }

	// Type casts

	int			to_int   ()	{ return (int)	num; }
	float		to_float ()	{ return (float)num; }
	double		to_double()	{ return (double)num; }
	std::string to_string() const	{
		std::string s = std::to_string(num);
		HIDE_UNC(if (unc != 0) s += " +- " + std::to_string(unc));
		HIDE_UNIT(if (u.to_string() != "") s+= " " + u.to_string());
		return s;
	}

	// Standard library overloads

	number<T>& abs() { num = std::abs(num);	return *this; }
	
	number<T>& sin () { CHECK_UNITS(u, ""); HIDE_UNC(unc *= std::abs(std::cos(num))); num = std::sin(num);	return *this; }
	number<T>& cos () { CHECK_UNITS(u, ""); HIDE_UNC(unc *= std::abs(std::sin(num))); num = std::cos(num);	return *this; }
	number<T>& tan () { CHECK_UNITS(u, ""); num = std::tan(num); HIDE_UNC(unc *= (1+std::pow(num, 2)));		return *this; }
	number<T>& asin() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= std::sqrt(1 - std::pow(num, 2))); num = std::asin(num);	return *this; }
	number<T>& acos() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= std::sqrt(1 - std::pow(num, 2))); num = std::acos(num);	return *this; }
	number<T>& atan() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= (1 + std::pow(num, 2))); num = std::atan(num); 			return *this; }
	
	number<T>& sinh () { CHECK_UNITS(u, ""); HIDE_UNC(unc = std::abs(std::cosh(num))*unc); num = std::sinh(num); 	return *this; }
	number<T>& cosh () { CHECK_UNITS(u, ""); HIDE_UNC(unc = std::abs(std::sinh(num))*unc); num = std::cosh(num);	return *this; }
	number<T>& tanh () { CHECK_UNITS(u, ""); num = std::tanh(num); HIDE_UNC(unc /= std::pow(std::cosh(num), 2));	return *this; }
	number<T>& asinh() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= std::sqrt(1 + std::pow(num, 2)));	num = std::asinh(num); 	return *this; }
	number<T>& acosh() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= std::sqrt(std::pow(num, 2) - 1));	num = std::acosh(num);	return *this; }
	number<T>& atanh() { CHECK_UNITS(u, ""); HIDE_UNC(unc /= 1 - std::pow(num, 2));				num = std::atanh(num); 	return *this; }

	number<T>& ceil() { HIDE_UNC(double f = fractional_uncertainty()); num = std::ceil(num) ; HIDE_UNC(unc = num*f);	return *this; }
	number<T>& floor(){ HIDE_UNC(double f = fractional_uncertainty()); num = std::floor(num); HIDE_UNC(unc = num*f);	return *this; }

	number<T>& exp(const T &exponential) { CHECK_UNITS(u, ""); num = std::pow(exponential, num); HIDE_UNC(unc *= num * ln10); return *this; }
	number<T>& pow(const T &power)
	{
		u.pow(power);
		HIDE_UNC(double fractional_unc = fractional_uncertainty());
		num = std::pow(num, power);
		HIDE_UNC(unc = std::abs(num * power * fractional_unc));
		return *this;
	}
	
	number<T>& log() {
		CHECK_UNITS(u, "");			// Cannot log quantities with units
		HIDE_UNC(unc /= num);
		num = std::log(num);
		return *this;
	}
	number<T>& log10() {
		CHECK_UNITS(u, "");			// Cannot log quantities with units
		HIDE_UNC(unc /= (num * ln10_inverse));
		num = std::log(num);
		return *this;
	}

	number<T>& sqrt() {
		num = std::sqrt(num);
		HIDE_UNC(unc /= (2 * num));
		u = u.sqrt();
		return *this;
	}

	// Operator overloads : number comparisons

	bool operator == (const number& rhs) const { return HIDE_UNIT((u == rhs.u) &&) (num == rhs.num) HIDE_UNC(&& (unc == rhs.unc)); }
	bool operator != (const number& rhs) const { return !operator==(rhs); }
	bool operator <  (const number& rhs) const { return num < rhs.num; }
	bool operator >  (const number& rhs) const { return num > rhs.num; }
	bool operator <= (const number& rhs) const { return !operator>(rhs); }
	bool operator >= (const number& rhs) const { return !operator<(rhs); }
	
	// Operator overloads : all other comparison

	bool operator == (const T& rhs) const { return num == rhs; }
	bool operator != (const T& rhs) const { return !operator==(rhs); }
	bool operator <  (const T& rhs) const { return num < rhs; }
	bool operator >  (const T& rhs) const { return num > rhs; }
	bool operator <= (const T& rhs) const { return !operator>(rhs); }
	bool operator >= (const T& rhs) const { return !operator<(rhs); }

	// Operator overloads : increment and decrement

	number& operator ++ () {
		++num;
		return *this;
	}
	number& operator -- () {
		--num;
		return *this;
	}
	const number operator ++ (int unused) {
		number result(*this);
		++(*this);
		return result;
	}
	const number operator -- (int unused) {
		number result(*this);
		--(*this);
		return result;
	}

	// Operator overloads : number arithmetic
	
	number& operator  = (const number &rhs) { HIDE_UNIT(u  = rhs.u); HIDE_UNC(unc = rhs.unc); num = rhs.num; return *this; }
	number& operator += (const number &rhs) { HIDE_UNIT(u += rhs.u); HIDE_UNC(unc = std::sqrt(unc*unc + rhs.unc*rhs.unc)); num += rhs.num; return *this; }	// unit addition is 1st to assert that the unit types are indeed the same (so that one can't add different units!)
	number& operator -= (const number &rhs) { HIDE_UNIT(u -= rhs.u); HIDE_UNC(unc = std::sqrt(unc*unc + rhs.unc*rhs.unc)); num -= rhs.num;  return *this; }
	number& operator *= (const number &rhs) { HIDE_UNIT(u *= rhs.u); HIDE_UNC(unc = std::sqrt(std::pow((unc / num), 2) + std::pow((rhs.unc / rhs.num), 2))); num *= rhs.num; HIDE_UNC(unc *= num); return *this; }
	number& operator /= (const number &rhs) { HIDE_UNIT(u /= rhs.u); HIDE_UNC(unc = std::sqrt(std::pow((unc / num), 2) + std::pow((rhs.unc / rhs.num), 2))); num /= rhs.num; HIDE_UNC(unc *= num); return *this; }
	
	// Operator overloads : scalar arithmetic

	number& operator  = (const T &rhs) { num  = rhs; return *this; }
	number& operator += (const T &rhs) { num += rhs; return *this; }
	number& operator -= (const T &rhs) { num -= rhs; return *this; }
	number& operator *= (const T &rhs) { num *= rhs; HIDE_UNC(unc *= std::abs(rhs)); return *this; }
	number& operator /= (const T &rhs) { num /= rhs; HIDE_UNC(unc /= std::abs(rhs)); return *this; }

};

// Operator overloads : rhs arithmetic

template <class T> number<T> operator + (number<T> lhs, const T& rhs) { lhs += rhs; return lhs; }
template <class T> number<T> operator - (number<T> lhs, const T& rhs) { lhs -= rhs; return lhs; }
template <class T> number<T> operator * (number<T> lhs, const T& rhs) { lhs *= rhs; return lhs; }
template <class T> number<T> operator / (number<T> lhs, const T& rhs) { lhs /= rhs; return lhs; }

template <class T> number<T> operator + (const T& lhs, number<T> rhs) { rhs += lhs; return rhs; }
template <class T> number<T> operator - (const T& lhs, number<T> rhs) { rhs -= lhs; return rhs; }
template <class T> number<T> operator * (const T& lhs, number<T> rhs) { rhs *= lhs; return rhs; }
template <class T> number<T> operator / (const T& lhs, number<T> rhs) { rhs /= lhs; return rhs; }

template <class T> number<T> operator + (number<T> lhs, const number<T> & rhs) { lhs += rhs; return lhs; }
template <class T> number<T> operator - (number<T> lhs, const number<T> & rhs) { lhs -= rhs; return lhs; }
template <class T> number<T> operator * (number<T> lhs, const number<T> & rhs) { lhs *= rhs; return lhs; }
template <class T> number<T> operator / (number<T> lhs, const number<T> & rhs) { lhs /= rhs; return lhs; }

// Operator overloads : streams

template <class T> std::ostream& operator << (std::ostream& os, const number<T>& n) {
	os << n.to_string();
	return os;
}
template <class T> std::istream& operator >> (std::istream& in, number<T>& p) {
	// ...
	return in;
}





/* Operator overloads : standard library */
namespace std {

	template <class T> number<T> abs(number<T> n) { n.abs(); return n; }

	template <class T> number<T> sin(number<T> n) { n.sin(); return n; }
	template <class T> number<T> cos(number<T> n) { n.cos(); return n; }
	template <class T> number<T> tan(number<T> n) { n.tan(); return n; }

	template <class T> number<T> asin(number<T> n) { n.asin(); return n; }
	template <class T> number<T> acos(number<T> n) { n.acos(); return n; }
	template <class T> number<T> atan(number<T> n) { n.atan(); return n; }

	template <class T> number<T> sinh(number<T> n) { n.sinh(); return n; }
	template <class T> number<T> cosh(number<T> n) { n.cosh(); return n; }
	template <class T> number<T> tanh(number<T> n) { n.tanh(); return n; }

	template <class T> number<T> asinh(number<T> n) { n.asinh(); return n; }
	template <class T> number<T> acosh(number<T> n) { n.acosh(); return n; }
	template <class T> number<T> atanh(number<T> n) { n.atanh(); return n; }

	template <class T> number<T> pow(number<T> n, T power) { n.pow(power); return n; }
	template <class T> number<T> exp(number<T> n, T exponential) { n.exp(exponential); return n; }
	
	template <class T> number<T> log  (number<T> n, T power) { n.log(power)  ; return n; }
	template <class T> number<T> log10(number<T> n, T power) { n.log10(power); return n; }

	template <class T> number<T> ceil (number<T> n) { n.ceil() ; return n; }
	template <class T> number<T> floor(number<T> n) { n.floor(); return n; }

	template <class T> number<T> sqrt(number<T> n) { n.sqrt(); return n; }

}

