#include <stdio.h>
#include <assert.h>

#define E 2.7182818284590452353602874713527

static double _fabs(double d) { return d > 0 ? d : -d; }

static double _pown(double base, unsigned int exponent)
{
	double acc = 1.0;

	for (int i = 0; i < exponent; ++i) {
		acc *= base;
	}

	return acc;
}

double ys_expi(int exponent)
{
	double result = _pown(E, _fabs(exponent));

	return exponent >= 0 ? result : 1.0 / result;
}

double ys_expq(int numerator, int denominator)
{
	double numerator_out = ys_expi(_fabs(numerator));

	double left = 1.0;
	double right = numerator_out;

	int positive_denominator = _fabs(denominator);

	do {
		double mid = (left + right) * 0.5;
		double mid_comp = _pown(mid, positive_denominator);

		if (mid_comp > numerator_out) {
			right = mid;
		} else {
			left = mid;
		}
	} while (right - left > 1e-6);

	double result = (left + right) * 0.5;
	result = numerator * denominator >= 0 ? result : 1.0 / result;

	return result; 
}

static long long _factorial(int order)
{
	assert(order >= 1);

	if (order == 1) {
		return 1;
	} else {
		return _factorial(order - 1) * order;
	}
}

double ys_exp(double exponent) {
	double estimate = 1 + exponent;
	double previous_estimate = 1;

	int order = 2;

	do {
		previous_estimate = estimate;

		estimate += _pown(exponent, order) / _factorial(order);

		order++;
	} while (_fabs(estimate - previous_estimate) > 1e-6);

	return estimate;
}

int main()
{
	printf("exp(%d) = %lf\n", 5, ys_expi(5));
	printf("exp(%d/%d) = %lf\n", 1, 2, ys_expq(1, 2));
	printf("exp(%lf) = %lf\n", E, ys_exp(E));

	return 0;
}
