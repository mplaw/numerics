#pragma once


// To do
//		- do some cleverness to make the argument of set a constant reference.
//
//

#include <string>
#include "fraction.h"
#include <assert.h>
#include "base_unit.h"
#include <vector>
#include "overloads.h"
#include <algorithm>

#include "macros_default.h"
#if DEBUG
	#define CHECK_EQUAL(lhs, rhs) assert(lhs == rhs);
#else
	#define CHECK_EQUAL(lhs, rhs)
#endif

typedef std::string string;
typedef std::vector<base_unit> container;

/* unit

	Description:
		A unit, e.g. kg m^2 s^-3
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
class unit {

private:
	container units;
	
	void swap(unit& rhs) {
		using std::swap;
		swap(units, rhs.units);
	}

public:

	// Construction 
	
	unit() : units() {}
	unit(const container &units) : units(units) {}
	unit(const std::initializer_list<base_unit> &units) : units(units) {}
	unit(string str_units) { set(str_units); }
	template <size_t SIZE> unit(std::array<base_unit, SIZE> &units) : units(units) {}
	unit(const unit& u) : units(u.units) {}			// copy constructor
	unit(unit&& u) { swap(u); }						// move constructor

	// Functions

	void set(string str_units)
	{
		int pos_space = str_units.find(" ");
		if (pos_space == -1) units = { base_unit(str_units) };			// no space in units, therefore assume is a single unit
		else
		{
			while (pos_space != -1)
			{
				units.push_back(base_unit(str_units.substr(0, pos_space)));
				str_units = str_units.substr(pos_space + 1);
				pos_space = str_units.find(" ");
			}
			units.push_back(base_unit(str_units));
		}
	}
	auto contains_type(const string &unit_name)
	{
		auto first = units.begin();
		auto end   = units.end  ();

		for (; first != end; ++first)
		{
			if ((*first).get_unit() == unit_name)
			{
				return first;
			}
		}
		return end;

	}
	void mul_units(const unit &unit)
	{
		auto it_rhs = unit.units.begin();
		auto end    = unit.units.cend ();

		while (it_rhs != end) {
			auto it = contains_type((*it_rhs).get_unit()); //std::find(units.begin(), units.end(), *it_rhs);
			if (it != units.end()) {
				(*it) *= *it_rhs;
			}
			else {
				units.push_back(*it_rhs);
			}

			++it_rhs;
		}
	}
	void invert() {
		auto first = units.begin();
		auto end = units.cend();
		for (; first != end; ++first) {
			(*first).invert();
		}
	}
	template <class T> unit& pow(T power)
	{
		auto first = units.begin();
		auto end = units.end();
		for (; first != end; ++first)
		{
			*first = (*first).pow(power);
		}
		return *this;
	}
	unit& sqrt()
	{
		auto first = units.begin();
		auto end = units.end();
		for (; first != end; ++first)
		{
			*first = (*first).sqrt();
		}
		return *this;
	}
	
	// Type casts
	
	string to_string() const
	{
		string s;
		auto start = units.begin();
		auto end = units.cend();
		while (start != end)
		{
			s += (*start).to_string() + " ";
			++start;
		}
		return s;
	}

	// Operator overloads : logical

	bool operator  == (const unit& rhs) const { return equal<container>(units, rhs.units); }
	bool operator  != (const unit& rhs) const { return !operator==(rhs); }

	// Operator overloads : increment and decrement

	unit  operator  -() const
	{
		unit f(units);
		f.invert();
		return f;
	}
	unit& operator ++() {
		inc<container, base_unit>(units);
		return *this;
	}
	unit& operator --() {
		dec<container, base_unit>(units);
		return *this;
	}
	const unit  operator ++(int unused) {
		unit result(*this);
		++(*this);
		return result;
	}
	const unit  operator --(int unused) {
		unit result(*this);
		--(*this);
		return result;
	}
	
	// Operator overloads : arithmetic

	unit& operator  = (const unit& rhs) { units = rhs.units; return *this; }
	unit& operator += (const unit& rhs) { CHECK_EQUAL(units, rhs.units); return *this; }
	unit& operator -= (const unit& rhs) { *this += rhs; return *this; }
	unit& operator *= (const unit& rhs) { mul_units(rhs); return *this;	}
	unit& operator /= (const unit& rhs) { *this *= -rhs; return *this; }
};

// Operator overloads : rhs arithmetic

unit operator + (unit lhs, const unit& rhs) { lhs += rhs; return lhs; }
unit operator - (unit lhs, const unit& rhs) { lhs -= rhs; return lhs; }
unit operator * (unit lhs, const unit& rhs) { lhs *= rhs; return lhs; }
unit operator / (unit lhs, const unit& rhs) { lhs /= rhs; return lhs; }

std::ostream& operator << (std::ostream& os, const unit& rhs) {
	os << rhs.to_string();
	return os;
}
std::istream& operator >> (std::istream& in, unit& rhs) {
	string s;
	getline(in, s);
	rhs.set(s);
	return in;
}

