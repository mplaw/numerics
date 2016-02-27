#include "stdafx.h"
#include "base_unit.h"

base_unit::base_unit() noexcept : unit(), power() {}
base_unit::base_unit(const str &unit, const fraction &power) : unit(unit), power(power) {}
base_unit::base_unit(const str &unit, const int &power) : unit(unit), power(power, 1) {}
base_unit::base_unit(const str &str_unit)
{
	int pos_pow = str_unit.find("^");
	if (pos_pow == -1) { unit = str_unit; power = 1; }
	else {
		unit = str_unit.substr(0, pos_pow);
		str p = str_unit.substr(pos_pow + 1);

		// The power could be a fraction or an int
		int pos_div = p.find("/");
		if (pos_div == -1) { power = std::stoi(p); }
		else {
			power = fraction(std::stoi(p.substr(0, pos_div)), std::stoi(p.substr(pos_div + 1)));
		}
	}
}

// Accessors

	  str& base_unit::get_unit() { return unit; }
const str& base_unit::get_unit() const { return unit; }
	  fraction& base_unit::get_power() { return power; }
const fraction& base_unit::get_power() const { return power; }

void base_unit::set(const str &Unit, const fraction &Power) { unit = Unit; power = Power; }
void base_unit::set(const str &str_unit) {
	int pos_pow = str_unit.find("^");
	if (pos_pow == -1) { unit = str_unit; power = 1; }
	else {
		unit = str_unit.substr(0, pos_pow);
		str p = str_unit.substr(pos_pow + 1);

		// The power could be a fraction or an int
		int pos_div = p.find("/");
		if (pos_div == -1) { power = std::stoi(p); }
		else {
			power = fraction(std::stoi(p.substr(0, pos_div)), std::stoi(p.substr(pos_div + 1)));
		}
	}
}
void base_unit::set_unit(const str &Unit) { unit = Unit; }
void base_unit::set_power(const fraction &Power) { power = Power; }

// Functions

void base_unit::invert() { power.negate(); }
base_unit& base_unit::sqrt() { power /= 2;	return *this; }

// Type casts

str base_unit::to_string() const
{
	if (power == 0) return "";
	else if (power == 1) return unit;
	else return unit + "^" + power.to_string();
}

// Operator overloads

bool base_unit::operator == (const base_unit& rhs) const { return (unit == rhs.unit) && (power == rhs.power); }
bool base_unit::operator != (const base_unit& rhs) const { return !operator==(rhs); }
bool base_unit::operator <  (const base_unit& rhs) const { EQ(unit, rhs.unit); return power < rhs.power; }
bool base_unit::operator >  (const base_unit& rhs) const { EQ(unit, rhs.unit); return power > rhs.power; }
bool base_unit::operator <= (const base_unit& rhs) const { return !operator>(rhs); }
bool base_unit::operator >= (const base_unit& rhs) const { return !operator<(rhs); }

base_unit  base_unit::operator -() const { return base_unit(unit, -power); }
base_unit& base_unit::operator++() { ++power; return *this; }
base_unit& base_unit::operator--() { --power; return *this; }
const base_unit base_unit::operator++(int unused) {
	base_unit result(*this);
	++(*this);
	return result;
}
const base_unit base_unit::operator--(int unused) {
	base_unit result(*this);
	--(*this);
	return result;
}

base_unit& base_unit::operator  = (base_unit& rhs) { unit = rhs.unit; power = rhs.power; return *this; }
base_unit& base_unit::operator += (const base_unit& rhs) { EQ(unit, rhs.unit); return *this; }
base_unit& base_unit::operator -= (const base_unit& rhs) { EQ(unit, rhs.unit); return *this; }
base_unit& base_unit::operator *= (const base_unit& rhs) { EQ(unit, rhs.unit); power += rhs.power; return *this; }
base_unit& base_unit::operator /= (const base_unit& rhs) { EQ(unit, rhs.unit); power -= rhs.power; return *this; }

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
	str s;
	getline(in, s);

	int position = s.find("^");
	if (position == -1)
	{
		rhs.set_unit(s);
		rhs.set_power(fraction(1, 1));
	}
	else {
		rhs.set_unit(s.substr(0, position));
		s = s.substr(position);

		position = s.find("/");
		if (position == -1) rhs.set_power(fraction(std::stoi(s), 1));
		else
		{
			rhs.set_power(fraction(std::stoi(s.substr(0, position)), std::stoi(s.substr(position))));
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