#pragma once

/* To do
		- remove all derived classes dependency on bound, so that you can remove the virtual 
		keyord from the "void check()" function. (virtual slows things down a little and 
		prevents compiler inlining).

		- finish type casts

		- add compiler optimisations, turn off excepptions

		- std library overloads (or maybe let the user cast)
*/

#include <iostream>

/*	Represents a number bound between a minium and maximum:
		min <= value <= max
	All operators act exclusively on value (so bound += 5 is really value += 5). With the notable exception of = (assignment),
	which sets all (value, min, max) to the assigned value. If only 1 value is provided for assignment, then it replaces value.

	Example use:
		bound<double> b(5.0, 1.0, 10.0);
		b += 10.0; // b = (1.0, 10.0, 10.0) Notice that value is capped at maximum (and is not 15.0 as it otherwise would be).
		b == 10.0; // true (as value = 10.0)

	Can be used with any type that supports arithmetic and comparisons, e.g. bound<int>, bound<float>, bound<MyCustomType>.
*/
template <class T>
class bound {
protected:
	T min, val, max;

	virtual void check() {
		if (val > max) { val = max; }
		else if (val < min) { val = min; }
	}
	void swap(bound& rhs) {
		using std::swap;
		swap(min, rhs.min);
		swap(val, rhs.val);
		swap(max, rhs.max);
	}

public:
	bound() : min(0), val(0), max(100) {}
	bound(const T &val, const T &min, const T &max) : min(min), val(val), max(max) {}
	bound(const bound& b) : min(b.min), val(b.val), max(b.max) {}
	bound(bound&& b) : bound() { swap(b); }
	~bound() {}

	std::ostream& print(std::ostream& os) const { os << min << " <= " << val << " <= " << max; return os; }
	
	T range  () const { return max - min; }
	T ratio	 () const { return (val - min) / (range()); }

	// Accessors

	T& get    () { return val; }
	T& get_min() { return min; }
	T& get_max() { return max; }

	const T& get	() const { return val; }
	const T& get_min() const { return min; }
	const T& get_max() const { return max; }

	void set	(const T &new_val) { val = new_val; check(); }
	void set_min(const T &new_min) { min = new_min; check(); }
	void set_max(const T &new_max) { max = new_max; check(); }
	void set_all(const T &new_val, const T &new_min, const T &new_max) { val = new_val; min = new_min; max = new_max; check(); }

	// Type casts 

	int			to_int   () const { return (int   )val; }
	float		to_float () const { return (float )val; }
	double		to_double() const { return (double)val; }
	std::string to_string() const { return std::to_string(val); }

	operator int   () const { return (int   )val; }
	operator float () const { return (float )val; }
	operator double() const { return (double)val; }

	// Operator overloads 
	
	bound& operator  = (const T &rhs) { val  = rhs; check(); return *this; }
	bound& operator += (const T &rhs) { val += rhs; check(); return *this; }
	bound& operator -= (const T &rhs) { val -= rhs; check(); return *this; }
	bound& operator *= (const T &rhs) { val *= rhs; check(); return *this; }
	bound& operator /= (const T &rhs) { val /= rhs; check(); return *this; }

	bound& operator  = (bound  rhs) { swap(rhs); return *this; } // copy-swap idiom
	bound& operator += (const bound& rhs) { val += rhs.val; check(); return *this; }
	bound& operator -= (const bound& rhs) { val -= rhs.val; check(); return *this; }
	bound& operator *= (const bound& rhs) { val *= rhs.val; check(); return *this; }
	bound& operator /= (const bound& rhs) { val /= rhs.val; check(); return *this; }

	bool   operator == (const T &rhs) const { return     (val == rhs); }
	bool   operator != (const T &rhs) const { return !operator==(rhs); }
	bool   operator <  (const T &rhs) const { return       val < rhs ; }
	bool   operator >  (const T &rhs) const { return       val > rhs ; }
	bool   operator <= (const T &rhs) const { return !operator >(rhs); }
	bool   operator >= (const T &rhs) const { return !operator <(rhs); }

	bool   operator == (const bound& rhs) const { return (min == rhs.min) && (val == rhs.val) && (max == rhs.max); }
	bool   operator != (const bound& rhs) const { return !operator==(rhs); }
	bool   operator <  (const bound& rhs) const { return val < rhs.val; }
	bool   operator >  (const bound& rhs) const { return val > rhs.val; }
	bool   operator <= (const bound& rhs) const { return !operator>(rhs); }
	bool   operator >= (const bound& rhs) const { return !operator<(rhs); }

	bound& operator++() {
		++val; check();
		return *this;
	}
	bound& operator--() {
		--val; check();
		return *this;
	}
	const bound operator++(int unused) {
		bound result(*this);
		++(*this);
		return result;
	}
	const bound operator--(int unused) {
		bound result(*this);
		--(*this);
		return result;
	}
};

template <class T> bound<T> operator+ (bound<T> lhs, const T& rhs) { lhs += rhs; return lhs; }	// copies in lhs, adds to lhs, then returns lhs
template <class T> bound<T> operator- (bound<T> lhs, const T& rhs) { lhs -= rhs; return lhs; }
template <class T> bound<T> operator* (bound<T> lhs, const T& rhs) { lhs *= rhs; return lhs; }
template <class T> bound<T> operator/ (bound<T> lhs, const T& rhs) { lhs /= rhs; return lhs; }

template <class T> bound<T> operator+ (const T& lhs, bound<T> rhs) { rhs += lhs; return rhs; }
template <class T> bound<T> operator- (const T& lhs, bound<T> rhs) { rhs -= lhs; return rhs; }
template <class T> bound<T> operator* (const T& lhs, bound<T> rhs) { rhs *= lhs; return rhs; }
template <class T> bound<T> operator/ (const T& lhs, bound<T> rhs) { rhs /= lhs; return rhs; }

template <class T> bound<T> operator+ (bound<T> lhs, const bound<T> & rhs) { lhs += rhs; return lhs; }
template <class T> bound<T> operator- (bound<T> lhs, const bound<T> & rhs) { lhs -= rhs; return lhs; }
template <class T> bound<T> operator* (bound<T> lhs, const bound<T> & rhs) { lhs *= rhs; return lhs; }
template <class T> bound<T> operator/ (bound<T> lhs, const bound<T> & rhs) { lhs /= rhs; return lhs; }

template <class T> std::ostream& operator << (std::ostream& os, const bound<T>& a) { a.print(os); return os; }

// expects input in the form of : val, min, max
template <class T> std::istream& operator >> (std::istream& in, bound<T>& b) {
	T x, y, z;

	// read in value
	in >> x;
	b.set(x);

	// skip delimiter
	in.ignore(2);

	// read minimum
	in >> y;
	b.set_min(y);

	// skip delimiter
	in.ignore(2);

	// read in maximum
	in >> z;
	b.set_max(z);

	return in;
}

namespace std {

	template <class T> std::string to_string(const bound<T> &num) { return num.to_string();	}

}
