#pragma once

#include "macros.h"
#include "typedefs.h"

//	To do		
//		- test all functions
//		- optimise simplify: it does additional work by checking multiples of already checked numbers,
//			e.g. it checks 2,3,4,5,6,7,8,9 separately, however 4,6,8,9 are redundant as 2 and 3 have been checked.
//			ultimately, it just needs to check the prime numbers. However obtaining the primes is difficult.
//		- re-evaluate noexcept

//	A fraction of integers, e.g. 5/3 or -27/4
//		Stores the numerator and denominator of the fraction: 5 and 3.
//		Implementation mostly in fraction.cpp
class fraction
{
private:
	int num, den;

public:

	// Construction

	fraction() noexcept;
	fraction(cint num, cint den) noexcept;
	fraction(const fraction&) noexcept = default;
	fraction(fraction&& f) noexcept = default;
	~fraction();

	// Accessors

	 int& get_num() noexcept;
	cint& get_num() const;
	 int& get_den() noexcept;
	cint& get_den() const;
	void  set_num(cint n) noexcept;
	void  set_den(cint d) noexcept;

	// Type casts

	int			to_int		() const;
	long		to_long		() const;
	float		to_float	() const;
	double		to_double	() const;
	long long	to_long_long() const;
	str to_string() const;

	operator int   () const;
	operator float () const;
	operator double() const;

	// Functions

	fraction& simplify();
	fraction& invert() noexcept;
	fraction& negate() noexcept;
	fraction& power(const fraction &n);
	template <class number>	fraction& power(const number n)
	{
		num = std::pow(num, n);
		den = std::pow(den, n);
		return *this;
	}

	// Operator overloads

	bool operator == (const fraction& rhs) const;
	bool operator != (const fraction& rhs) const;
	bool operator <  (const fraction& rhs) const;
	bool operator >  (const fraction& rhs) const;
	bool operator <= (const fraction& rhs) const;
	bool operator >= (const fraction& rhs) const;

	bool operator == (const int& rhs) const;
	bool operator != (const int& rhs) const;
	bool operator <  (const int& rhs) const;
	bool operator >  (const int& rhs) const;
	bool operator <= (const int& rhs) const;
	bool operator >= (const int& rhs) const;

	bool operator == (const float& rhs) const;
	bool operator != (const float& rhs) const;
	bool operator <  (const float& rhs) const;
	bool operator >  (const float& rhs) const;
	bool operator <= (const float& rhs) const;
	bool operator >= (const float& rhs) const;

	fraction operator-() const;
	fraction& operator++();
	fraction& operator--();
	const fraction operator++(int unused);
	const fraction operator--(int unused);

	fraction& operator  = (fraction rhs) noexcept;
	fraction& operator += (const fraction& rhs);
	fraction& operator -= (const fraction& rhs);
	fraction& operator *= (const fraction& rhs);
	fraction& operator /= (const fraction& rhs);

	fraction& operator  = (cint &rhs) noexcept;
	fraction& operator *= (cint &rhs);
	fraction& operator /= (cint &rhs);
	fraction& operator += (cint &rhs);
	fraction& operator -= (cint &rhs);

};

template <class T> fraction operator+ (fraction lhs, const T& rhs) { lhs += rhs; return lhs; }
template <class T> fraction operator- (fraction lhs, const T& rhs) { lhs -= rhs; return lhs; }
template <class T> fraction operator* (fraction lhs, const T& rhs) { lhs *= rhs; return lhs; }
template <class T> fraction operator/ (fraction lhs, const T& rhs) { lhs /= rhs; return lhs; }

template <class T> fraction operator+ (const T& lhs, fraction rhs) { rhs += lhs; return rhs; }
template <class T> fraction operator- (const T& lhs, fraction rhs) { rhs -= lhs; return rhs; }
template <class T> fraction operator* (const T& lhs, fraction rhs) { rhs *= lhs; return rhs; }
template <class T> fraction operator/ (const T& lhs, fraction rhs) { rhs /= lhs; return rhs; }

template <class T> fraction operator+ (fraction lhs, const fraction & rhs) { lhs += rhs; return lhs; }
template <class T> fraction operator- (fraction lhs, const fraction & rhs) { lhs -= rhs; return lhs; }
template <class T> fraction operator* (fraction lhs, const fraction & rhs) { lhs *= rhs; return lhs; }
template <class T> fraction operator/ (fraction lhs, const fraction & rhs) { lhs /= rhs; return lhs; }

std::ostream& operator << (std::ostream& os, const fraction& rhs);
std::istream& operator >> (std::istream& in, fraction& rhs);

