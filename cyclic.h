#pragma once

#include "bound.h"

/*	Derives from bound<T>, but instead of being capped between a min and max it cycles between them.
E.g.	Cyclic<double> c(0.0, 1.0, 3.0);
		c += 1;		// (0.0, 2.0, 3.0)
		c += 1;		// (0.0, 3.0, 3.0)
		c += 1;		// (0.0, 0.0, 3.0)	<-- returns back to min
		c += 1;		// (0.0, 1.0, 3.0)  Repeats the cycle.
*/
template <class T>
class cyclic : public bound<T> {

protected:
	void check() {
		if (val > max) {
			T range_plus_one = range() + 1;
			val -= int((val - min) / (range_plus_one)) * (range_plus_one);
		}
		else if (val < min) {
			T range_plus_one = range() + 1;
			val += int((max - val) / (range_plus_one)) * (range_plus_one);
		}
	}

public:
	cyclic() : bound<T>() {}
	cyclic(const T &val, const T &min=0, const T &max=100) : bound<T>(val, min, max) {}
	cyclic(const cyclic& b) : bound<T>(b) {}
	cyclic(cyclic&& b) : cyclic() { swap(b); }
	~cyclic() {}

};

namespace std {

	template <class T>
	std::string to_string(const cyclic<T> &num) {
		return num.to_string();
	}

}