#pragma once

/* To do
		- remove all derived classes dependency on bound, so that you can remove the virtual 
		keyord from the "void check()" function. (virtual slows things down a little and 
		prevents compiler inlining).

		- add no except where appropriate and possible

		- consdier replacing min, val and max with an array of type T, so that swap can just swap the pointers to the start element and thus be more efficient.
		note: this would likely increase look-up time of 2nd and 3rd elements, as it would require computing their position in the array.
*/

#include <iostream>

#define MANUAL_CHECKING	0		// Turns the check function off, so the bound object's value is no longer bounded automatically. You can manually enforce bounds by calling check().
								// This will allow you to chain-together lots of operations, and then check at the end (as opposed to check being called after each operation).

#if MANUAL_CHECKING
	#define REM_CHECK(x)
#else
	#define REM_CHECK(x) x
#endif

/*	Represents a number bound between a minium and maximum:
		min <= value <= max
	All operators act exclusively on value (so bound += 5 is really value += 5). With the notable exception of = (assignment),
	which sets all (value, min, max) to the assigned value. If only 1 value is provided for assignment, then it replaces value.

	Example use:
		bound<double> b(5.0, 1.0, 10.0);
		b += 10.0; // b = (1.0, 10.0, 10.0) Notice that value is capped at maximum (and is not 15.0 as it otherwise would be).
		b == 10.0; // true (as value = 10.0)

	Can be used with any type that supports arithmetic and comparisons, e.g. bound<int>, bound<float>, bound<MyCustomType>. */
template <class T>
class bound {
protected:
	T min, val, max;

	friend void swap(bound& lhs, bound& rhs) noexcept(noexcept(T()))		// friend allows this to be a binary function (opposed to unary), and thus can be called instead of std::swap by STL containers
	{
		using std::swap;		
		swap(lhs.min, rhs.min);
		swap(lhs.val, rhs.val);
		swap(lhs.max, rhs.max);
	}

public:
	bound() noexcept(noexcept(T())) : min(), val(), max()  {}
	bound(const T &val, const T &min, const T &max) noexcept(noexcept(T())) : min(min), val(val), max(max) { REM_CHECK(check()); }
	bound(const bound<T>& b) noexcept(noexcept(T())) : min(b.min), val(b.val), max(b.max) {}
	bound(bound<T>&& b) noexcept(noexcept(T())) : bound() { swap(*this, b); }	// alternatively: bound(bound&& b) noexcept : bound(std::move(b.val), std::move(b.min), std::move(b.max)) { }
	
	~bound() {}

	virtual void check()	// in order to offer no exceptions guarantee, need to guarantee > and < operators exist for type T.
	{
		if (val > max) { val = max; }
		else if (val < min) { val = min; }
	}

	std::ostream& print(std::ostream& os) const { os << min << " <= " << val << " <= " << max; return os; }
	
	T range() const { return max - min; }
	T ratio() const { return (val - min) / range(); }

	// Accessors

	T& get    () noexcept { return val; }
	T& get_min() noexcept { return min; }
	T& get_max() noexcept { return max; }

	const T& get	() const { return val; }
	const T& get_min() const { return min; }
	const T& get_max() const { return max; }

	void set    (const T &new_val) { val = new_val; REM_CHECK(check()); }
	void set_min(const T &new_min) { min = new_min; REM_CHECK(check()); }
	void set_max(const T &new_max) { max = new_max; REM_CHECK(check()); }
	void set_all(const T &new_val, const T &new_min, const T &new_max) { val = new_val; min = new_min; max = new_max; REM_CHECK(check()); }

	// Type casts 
	
	int		to_int		() const { return (int		)val; }
	long		to_long		() const { return (long		)val; }
	float		to_float	() const { return (float	)val; }
	double		to_double	() const { return (double	)val; }
	long long	to_long_long	() const { return (long long	)val; }
	std::string to_string   	() const { return std::to_string(val); }

	operator T() const { return val; }

	// Operator overloads 
	
	bound& operator  = (T rhs) {std::swap(val,rhs); REM_CHECK(check()); return *this; }   // copy-swap idiom
	bound& operator += (const T &rhs) { val += rhs; REM_CHECK(check()); return *this; }
	bound& operator -= (const T &rhs) { val -= rhs; REM_CHECK(check()); return *this; }
	bound& operator *= (const T &rhs) { val *= rhs; REM_CHECK(check()); return *this; }
	bound& operator /= (const T &rhs) { val /= rhs; REM_CHECK(check()); return *this; }

	bound& operator  = (bound  rhs) noexcept(noexcept(T())) { swap(*this, rhs); return *this; } // copy-swap idiom
	bound& operator += (const bound& rhs) { val += rhs.val; REM_CHECK(check()); return *this; }
	bound& operator -= (const bound& rhs) { val -= rhs.val; REM_CHECK(check()); return *this; }
	bound& operator *= (const bound& rhs) { val *= rhs.val; REM_CHECK(check()); return *this; }
	bound& operator /= (const bound& rhs) { val /= rhs.val; REM_CHECK(check()); return *this; }

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
		++val; REM_CHECK(if (val > max) { val = max; });  // no need to check minium boundary
		return *this;
	}
	bound& operator--() {
		--val; REM_CHECK(if (val < min) { val = min; });	// not necessary to check maximum boundary
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
template <class T> std::istream& operator >> (std::istream& in, bound<T>& b) {
	T x, y, z;
	in >> x; b.set(x);	// read val		:, min, max
	in.ignore(2);		// skip delimiter	:min, max
	in >> y; b.set_min(y);	// read minimum		:, max
	in.ignore(2);		// skip delimiter	:max
	in >> z; b.set_max(z);	// read maximum
	return in;
}
