#include "Signal.h"


// an execution example
int main()
{
	// for comparison
	Signal a = { 'a', { 1, 2, 0, 0 } };
	a.to_freq();

	// for comparison
	Signal b = { 'b', { 1, 1, 0, 0 } };
	b.to_freq();

	// DUAL Signal: Signal a @real, Signal b @imag
	Signal x = { 'x', { { 1, 1 }, { 2, 1 }, { 0, 0 }, { 0, 0 } } };

	// simultaneously transform both signals into the frequency domain and separate them
	auto [x_1, x_2] = x.split_dual();

	x_1.to_time();
	x_2.to_time();

	return 0;
}
