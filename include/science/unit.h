#pragma once


// To do
//		- do some cleverness to make the argument of set a constant reference.
//		- cut down on extra includes
//		- overload subscript operator, to access single unit.
//		- add base_unit overloads

#include <string>
#include "fraction.h"
#include "base_unit.h"
#include <vector>
#include "overloads.h"

#include "macros.h"

typedef std::vector<base_unit> container;

/* 	A unit, e.g. kg m^2 s^-3
	Technically, it's an iterable of base_unit objects.

	Uses:
		Addition/Subtraction	:	asserts that the units are the same.
		Multiplication/Division	:	new base units are appended, and existing base_units
									have their powers added or subtract respectively.
		N.B. if DEBUG flag is False, then asserts are removed.

	Examples:
		kg m^-3 + kg m^-3	-> OK
		kg m^-3 + kg^2 s^4	-> NOT OK
		kg * kg				-> kg^2
		kg^6 m^2 * m s^-2	-> kg^6 m^3 s^-2
*/
class unit 
{
private:
	container units;
	
	void swap(unit& rhs) {
		using std::swap;
		swap(units, rhs.units);
	}

public:

	// Construction 
	
	unit();
	unit(const container &units);
	unit(const std::initializer_list<base_unit> &units);
	unit(str str_units);
	template <size_t SIZE> unit(std::array<base_unit, SIZE> &units) : units(units) {}
	unit(const unit& u);
	unit(unit&& u);

	// Accessors

	void set(str str_units);

	// Type casts
	
	str to_string() const;

	// Functions

	auto contains_type(const str &unit_name);
	void mul_units(const unit &unit);
	void invert();
	template <class T> unit& pow(T power) {
		auto first = units.begin();
		auto end = units.end();
		for (; first != end; ++first)
		{
			*first = (*first).pow(power);
		}
		return *this;
	}
	unit& sqrt();
	
	// Operator overloads

	bool operator  == (const unit& rhs) const;
	bool operator  != (const unit& rhs) const;

	unit  operator  -() const;
	unit& operator ++();
	unit& operator --();
	const unit  operator ++(int unused);
	const unit  operator --(int unused);
	
	unit& operator  = (unit rhs) noexcept;
	unit& operator += (const unit& rhs);
	unit& operator -= (const unit& rhs);
	unit& operator *= (const unit& rhs);
	unit& operator /= (const unit& rhs);
};

unit operator + (unit lhs, const unit& rhs);
unit operator - (unit lhs, const unit& rhs);
unit operator * (unit lhs, const unit& rhs);
unit operator / (unit lhs, const unit& rhs);

std::ostream& operator << (std::ostream& os, const unit& rhs);
std::istream& operator >> (std::istream& in, unit& rhs);

