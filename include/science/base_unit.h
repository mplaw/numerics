#pragma once

// To do
//		- add operators that act on strings
//		- add swap, copy and move functions/constructors
//		- add std library overloads

#include <string>
#include "fraction.h"
#include "macros.h"
#include "typedefs.h"


//	A single base unit and it's power, e.g. kg^5
//
//	Addition/Subtraction	:	do nothing to unit, although in Debug mode it will check for unit
//								homogenity by asserting that both units are indeed the same.
//	Multiplication/Division	:	Add or subtract the powers of the units respectively.
class base_unit {
private:
	str unit;
	fraction power;

public:

	// Constructors

	base_unit() noexcept;
	base_unit(const str &unit, const fraction &power);
	base_unit(const str &unit, const int &power);
	base_unit(const str &str_unit);

	// Accessors

	str& get_unit();
	const str& get_unit() const;
	fraction& get_power();
	const fraction& get_power() const;

	void set(const str &Unit, const fraction &Power);
	void set(const str &str_unit);
	void set_unit(const str &Unit);
	void set_power(const fraction &Power);

	// Functions

	void invert();
	template <class T> base_unit& pow(T power) {	power *= power;	return *this; }
	base_unit& sqrt();

	// Type casts

	str to_string() const;
	
	// Operator overloads

	bool operator == (const base_unit& rhs) const;
	bool operator != (const base_unit& rhs) const;
	bool operator <  (const base_unit& rhs) const;
	bool operator >  (const base_unit& rhs) const;
	bool operator <= (const base_unit& rhs) const;
	bool operator >= (const base_unit& rhs) const;

	base_unit  operator -() const;
	base_unit& operator++();
	base_unit& operator--();
	const base_unit operator++(int unused);
	const base_unit operator--(int unused);

	base_unit& operator  = (base_unit& rhs);
	base_unit& operator += (const base_unit& rhs);
	base_unit& operator -= (const base_unit& rhs);
	base_unit& operator *= (const base_unit& rhs);
	base_unit& operator /= (const base_unit& rhs);
};

// Operator overloads : rhs arithmetic

base_unit operator+ (base_unit lhs, const base_unit& rhs);
base_unit operator- (base_unit lhs, const base_unit& rhs);
base_unit operator* (base_unit lhs, const base_unit& rhs);
base_unit operator/ (base_unit lhs, const base_unit& rhs);

std::ostream& operator << (std::ostream& os, const base_unit& rhs);
std::istream& operator >> (std::istream& in, base_unit& rhs);
