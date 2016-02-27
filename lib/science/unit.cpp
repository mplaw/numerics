#include "stdafx.h"
#include "unit.h"

// Construction 

unit::unit() : units() {}
unit::unit(const container &units) : units(units) {}
unit::unit(const std::initializer_list<base_unit> &units) : units(units) {}
unit::unit(str str_units) { set(str_units); }
unit::unit(const unit& u) : units(u.units) {}			// copy constructor
unit::unit(unit&& u) = default;							// move constructor

// Accessors
			
void unit::set(str str_units)
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

// Type casts

str unit::to_string() const
{
	str s;
	auto start = units.begin();
	auto end = units.cend();
	while (start != end)
	{
		s += (*start).to_string() + " ";
		++start;
	}
	return s;
}

// Functions

auto unit::contains_type(const str &unit_name)
{
	auto first = units.begin();
	auto end = units.end();

	for (; first != end; ++first)
	{
		if ((*first).get_unit() == unit_name)
		{
			return first;
		}
	}
	return end;

}
void unit::mul_units(const unit &unit)
{
	auto it_rhs = unit.units.begin();
	auto end = unit.units.cend();

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
void unit::invert() {
	auto first = units.begin();
	auto end = units.cend();
	for (; first != end; ++first) {
		(*first).invert();
	}
}
unit& unit::sqrt()
{
	auto first = units.begin();
	auto end = units.end();
	for (; first != end; ++first)
	{
		*first = (*first).sqrt();
	}
	return *this;
}

// Operator overloads : logical

bool unit::operator  == (const unit& rhs) const { return equal<container>(units, rhs.units); }
bool unit::operator  != (const unit& rhs) const { return !operator==(rhs); }

// Operator overloads : increment and decrement

unit  unit::operator  -() const
{
	unit f(units);
	f.invert();
	return f;
}
unit& unit::operator ++() {
	inc(units.begin(), units.cend());
	return *this;
}
unit& unit::operator --() {
	dec(units.begin(), units.cend());
	return *this;
}
const unit unit::operator ++(int unused) {
	unit result(*this);
	++(*this);
	return result;
}
const unit unit::operator --(int unused) {
	unit result(*this);
	--(*this);
	return result;
}

// Operator overloads : arithmetic

unit& unit::operator  = (unit rhs) noexcept { std::swap(*this, rhs); return *this; }
unit& unit::operator += (const unit& rhs) { EQ(units, rhs.units); return *this; }
unit& unit::operator -= (const unit& rhs) { EQ(units, rhs.units); return *this; }
unit& unit::operator *= (const unit& rhs) { mul_units(rhs); return *this; }
unit& unit::operator /= (const unit& rhs) { *this *= -rhs; return *this; }

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
	str s;
	getline(in, s);
	rhs.set(s);
	return in;
}

