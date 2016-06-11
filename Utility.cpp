#include <cmath>
#include "Utility.hpp"

double Heaviside(const double x) {
	return (x <= 0.0 ? 0.0 : x);
}

double min3 (const double a, const double b, const double c) {
	if (a <= b) {
		if (a <= c) return a;
		else return c;
	}
	else {
		if (b <= c) return b;
		else return c;
	}
}

double Distance(double x1, double y1, double x2, double y2) {
	x1 -= x2;
	y1 -= y2;
	return sqrt(x1 * x1 + y1 * y1);
}
