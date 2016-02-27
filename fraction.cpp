#include "stdafx.h"
#include "fraction.h"

// Construction

fraction::fraction() noexcept : num(), den() {}
fraction::fraction(cint num, cint den) noexcept : num(num), den(den) {}
fraction::~fraction() {}

// Accessors

 int& fraction::get_num() noexcept { return num; }
cint& fraction::get_num() const { return num; }
 int& fraction::get_den() noexcept { return den; }
cint& fraction::get_den() const { return den; }
void  fraction::set_num(cint n) noexcept { num = n; }
void  fraction::set_den(cint d) noexcept { den = d; }

// Type casts

int			fraction::to_int() const { return			 num / den; }
long		fraction::to_long() const { return (long)num / (long)den; }
float		fraction::to_float() const { return (float)num / (float)den; }
double		fraction::to_double() const { return (double)num / (double)den; }
long long	fraction::to_long_long() const { return (long long)num / (long long)den; }
str fraction::to_string() const {
	if (den == 1) return std::to_string(num);
	else {
		return std::to_string(num) + "/" + std::to_string(den);
	}
}

fraction::operator int() const { return to_int(); }
fraction::operator float() const { return to_float(); }
fraction::operator double() const { return to_double(); }


// Functions

// The optimal solution would be to iterate through all precomputed prime numbers less than num, 
// which has a max of 2,147,483,647. However there are 60 million of them, so screw that! lolol.
fraction& fraction::simplify()
{
	int max = std::ceil(num / 2);

	// Divide by two until no longer possible
	while (num % 2 == 0 && den % 2 == 0)
	{
		num /= 2; den /= 2;
	}
	max = std::ceil(num / 2);

	// Check odd numbers onwards.
	for (int i = 3; i < (max + 1); i+=2)
	{
		while (num % i == 0 && den % i == 0)
		{
			num /= i; den /= i;
		}
		max = std::ceil(num / 2);
	}
	return *this;
}
fraction& fraction::invert() noexcept { std::swap(num, den); return *this; }
fraction& fraction::negate() noexcept { num *= -1; return *this; }
fraction& fraction::power(const fraction &n) 
{
	num = std::pow(num, n.num);
	num = std::pow(num, n.den);
	den = std::pow(den, n.num);
	den = std::pow(den, n.den);
	return *this;
}

// Operator overloads

bool fraction::operator == (const fraction& rhs) const { return (num == rhs.num) && (den == rhs.den); }
bool fraction::operator != (const fraction& rhs) const { return !operator==(rhs); }
bool fraction::operator <  (const fraction& rhs) const { return (num*rhs.den) < (rhs.num*den); }
bool fraction::operator >  (const fraction& rhs) const { return (num*rhs.den) > (rhs.num*den); }
bool fraction::operator <= (const fraction& rhs) const { return !operator>(rhs); }
bool fraction::operator >= (const fraction& rhs) const { return !operator<(rhs); }

bool fraction::operator == (const int& rhs) const { return num == rhs*den; }
bool fraction::operator != (const int& rhs) const { return !operator==(rhs); }
bool fraction::operator <  (const int& rhs) const { return num < rhs*den; }
bool fraction::operator >  (const int& rhs) const { return num > rhs*den; }
bool fraction::operator <= (const int& rhs) const { return !operator>(rhs); }
bool fraction::operator >= (const int& rhs) const { return !operator<(rhs); }

bool fraction::operator == (const float& rhs) const { return to_float() == rhs; }
bool fraction::operator != (const float& rhs) const { return !operator==(rhs); }
bool fraction::operator <  (const float& rhs) const { return to_float() < rhs; }
bool fraction::operator >  (const float& rhs) const { return to_float() > rhs; }
bool fraction::operator <= (const float& rhs) const { return !operator>(rhs); }
bool fraction::operator >= (const float& rhs) const { return !operator<(rhs); }

fraction fraction::operator-() const { return fraction(-num, den); }
fraction& fraction::operator++() { num += den; return *this; }
fraction& fraction::operator--() { num -= den; return *this; }
const fraction fraction::operator++(int unused) {
	fraction result(*this);
	++(*this);
	return result;
}
const fraction fraction::operator--(int unused) {
	fraction result(*this);
	--(*this);
	return result;
}

fraction& fraction::operator  = (fraction rhs) noexcept { std::swap(*this, rhs); return *this; }
fraction& fraction::operator += (const fraction& rhs) {
	int rhs_den = rhs.den;
	if (den == rhs_den) num += rhs.num;
	else
	{
		num *= rhs_den;
		num += rhs.num * den;
		den *= rhs_den;
	}
	return *this;
}
fraction& fraction::operator -= (const fraction& rhs) { *this += -rhs; return *this; }
fraction& fraction::operator *= (const fraction& rhs) { num *= rhs.num; den *= rhs.den; return *this; }
fraction& fraction::operator /= (const fraction& rhs) { num *= rhs.den; den *= rhs.num; return *this; }

fraction& fraction::operator  = (cint &rhs) noexcept { num = rhs; den = 1; return *this; }
fraction& fraction::operator *= (cint &rhs) { num *= rhs;	 return *this; }
fraction& fraction::operator /= (cint &rhs) { den *= rhs;	 return *this; }
fraction& fraction::operator += (cint &rhs) { num += rhs*den; return *this; }
fraction& fraction::operator -= (cint &rhs) { num -= rhs*den; return *this; }

std::ostream& operator << (std::ostream& os, const fraction& rhs) {
	os << rhs.to_string();
	return os;
}
std::istream& operator >> (std::istream& in, fraction& rhs) {
	int x, y;
	in >> x; rhs.set_num(x);		// read in x
	in.ignore(1);					// skip delimiter
	in >> y; rhs.set_den(y);		// read y
	return in;
}

