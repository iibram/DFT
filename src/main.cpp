#include "Signal.h"


// An execution example
int main()
{
	// for comparison
	Signal a = { 'a', { 1, 2, 3, 4 } };
	a.to_freq();

	// for comparison
	Signal b = { 'b', { 5, 6, 7, 8 } };
	b.to_freq();

	// DUAL Signal: Signal a @real, Signal b @imag
	Signal x = { 'x', { { 1, 5 }, { 2, 6 }, { 3, 7 }, { 4, 8 } } };

	// simultaneously transform both signals into the frequency-domain and separate them
	auto [x_1, x_2] = x.split_dual();

	// transform both separated signals back to the time-domain
	x_1.to_time();
	x_2.to_time();

	return 0;
}
