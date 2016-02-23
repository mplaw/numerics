#pragma once

// To do
//		- add operators that act on strings
//		- add swap, copy and move functions/constructors
//		- add std library overloads
//
//

#include <string>
#include "fraction.h"
#include <assert.h>
#include "macros_default.h"

#if DEBUG
#define CHECK_EQUAL(lhs, rhs) assert(lhs == rhs);
#else
#define CHECK_EQUAL(lhs, rhs)
#endif

typedef std::string string;

// base_unit
//	A single base unit and it's power, e.g. kg^5
//
//	Addition/Subtraction	:	do nothing to unit, although in Debug mode it will check for unit
//								homogenity by asserting that both units are indeed the same.
//	Multiplication/Division	:	Add or subtract the powers of the units respectively.
class base_unit {
private:
	string unit;
	fraction<int> power;

public:
	base_unit() : unit(), power() {}
	base_unit(const string &unit, const fraction<int> &power) : unit(unit), power(power) {}
	base_unit(const string &unit, const int &power) : unit(unit), power(power, 1) {}
	base_unit(const string &str_unit)
	{
		int pos_pow = str_unit.find("^");
		if (pos_pow == -1) { unit = str_unit; power = 1; }
		else {
			unit = str_unit.substr(0, pos_pow);
			string p = str_unit.substr(pos_pow+1);	

			// The power could be a fraction or an int
			int pos_div = p.find("/");
			if (pos_div == -1) { power = std::stoi(p); }
			else {
				power = fraction<int>(std::stoi(p.substr(0, pos_div)), std::stoi(p.substr(pos_div+1)));
			}
		}
	}

	// Accessors

		  string& get_unit () { return unit ; }
	const string& get_unit () const { return unit ; }
		  fraction<int>& get_power() { return power; }
	const fraction<int>& get_power() const { return power; }

	void set(const string &Unit, const fraction<int> &Power) { unit = Unit; power = Power; }
	void set(const string &str_unit) {
		int pos_pow = str_unit.find("^");
		if (pos_pow == -1) { unit = str_unit; power = 1; }
		else {
			unit = str_unit.substr(0, pos_pow);
			string p = str_unit.substr(pos_pow + 1);

			// The power could be a fraction or an int
			int pos_div = p.find("/");
			if (pos_div == -1) { power = std::stoi(p); }
			else {
				power = fraction<int>(std::stoi(p.substr(0, pos_div)), std::stoi(p.substr(pos_div + 1)));
			}
		}
	}
	void set_unit(const string &Unit) { unit = Unit; }
	void set_power(const fraction<int> &Power) { power = Power; }

	// Functions

	void invert() {
		power.negate();
	}
	template <class T> base_unit& pow(T puhpuhpuh__POWER)
	{
		power *= puhpuhpuh__POWER;
		return *this;
	}
	base_unit& sqrt()
	{
		power /= 2;
		return *this;
	}

	// Type casts

	string to_string() const
	{
		if (power == 0) return "";
		else if (power == 1) return unit;
		else return unit + "^" + power.to_string();
	}
	
	// Operator overloads

	bool operator == (const base_unit& rhs) const { return (unit == rhs.unit) && (power == rhs.power); }
	bool operator != (const base_unit& rhs) const { return !operator==(rhs); }
	bool operator <  (const base_unit& rhs) const { CHECK_EQUAL(unit, rhs.unit) return power < rhs.power; }
	bool operator >  (const base_unit& rhs) const { CHECK_EQUAL(unit, rhs.unit) return power > rhs.power; }
	bool operator <= (const base_unit& rhs) const { return !operator>(rhs); }
	bool operator >= (const base_unit& rhs) const { return !operator<(rhs); }

	base_unit operator-() const
	{
		base_unit f(unit, -power);
		return f;
	}

	base_unit& operator++() {
		++power;
		return *this;
	}
	base_unit& operator--() {
		--power;
		return *this;
	}
	base_unit operator++(int unused) {
		base_unit result(*this);
		++(*this);
		return result;
	}
	base_unit operator--(int unused) {
		base_unit result(*this);
		--(*this);
		return result;
	}

	base_unit& operator  = (base_unit& rhs) { unit = rhs.unit; power = rhs.power; return *this; }
	base_unit& operator += (const base_unit& rhs) {
		CHECK_EQUAL(unit, rhs.unit)
		return *this;
	}
	base_unit& operator -= (const base_unit& rhs) { *this += -rhs; return *this; }
	base_unit& operator *= (const base_unit& rhs) {
		CHECK_EQUAL(unit, rhs.unit)
		power += rhs.power;
		return *this;
	}
	base_unit& operator /= (const base_unit& rhs) { *this *= -rhs; return *this; }
};

// Operator overloads : rhs arithmetic

base_unit operator+ (base_unit lhs, const base_unit& rhs) { lhs += rhs; return lhs; }
base_unit operator- (base_unit lhs, const base_unit& rhs) { lhs -= rhs; return lhs; }
base_unit operator* (base_unit lhs, const base_unit& rhs) { lhs *= rhs; return lhs; }
base_unit operator/ (base_unit lhs, const base_unit& rhs) { lhs /= rhs; return lhs; }

std::ostream& operator << (std::ostream& os, const base_unit& rhs) {
	os << rhs.to_string();
	return os;
}
std::istream& operator >> (std::istream& in, base_unit& rhs) {
	string s;
	getline(in, s);

	int position = s.find("^");
	if (position == -1)
	{
		rhs.set_unit(s);
		rhs.set_power(fraction<int>(1, 1));
	}
	else {
		rhs.set_unit(s.substr(0, position));
		s = s.substr(position);

		position = s.find("/");
		if (position == -1) rhs.set_power(fraction<int>(std::stoi(s), 1));
		else
		{
			rhs.set_power(fraction<int>(std::stoi(s.substr(0, position)), std::stoi(s.substr(position))));
		}
	}

	return in;
}


namespace si {
	base_unit kg("kg");
	base_unit m("m");
	base_unit s("s");
	base_unit A("A");
	base_unit mol("mol");
	base_unit K("K");
}