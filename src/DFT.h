#pragma once
#include <iostream>
#include <complex>
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


constexpr double  FACTOR = 1e12;		// Scaling factor used to isolate the first 12 decimal places for rounding
constexpr double EPSILON = 1e-12;		// Threshold to suppress residual floating-point noise below the 12th decimal


/**
 * @brief This namespace provides an optimized implementation of the complex-valued DFT (Discrete Fourier Transform) for both
 * forward and inverse transformations, based on academic lecture specifications.
 * Additionally, it handles output conditioning by rounding complex values during the print function to ensure a clean and
 * readable terminal display.
 * @note Includes a specialized single-pass separation routine for combined dual-signal transformations.
 * @author Ibrahim Ibram
 * @date April 2026 (refactored and optimized an older implementation)
 */
namespace DFT
{
	//==========================================================================================================================//
	//															  D F T															//
	//==========================================================================================================================//
	/**
	 * @brief Computes the complex-valued DFT based on academic lecture formulas, independently optimized for enhanced performance
	 * and reduced memory overhead.
	 * @param x input signal
	 * @param invert transformation direction (default: false, time domain -> frequency domain)
	 * @return the transformed signal
	 */
	inline std::vector<std::complex<double>> dft(std::vector<std::complex<double>>& x, bool invert = false)
	{
		uint32_t N = x.size();

		std::vector<std::complex<double>> y(N);

		// optimized loop
		for (uint32_t n = 0; n < N; n++)
		{
			double angle = 2.0 * M_PI * n / N;

			if (!invert)
				angle = -angle;

			std::complex<double> w(cos(angle), sin(angle));
			std::complex<double> wn(1, 0);

			for (int k = 0; k < N; k++)
			{
				y[n] += x[k] * wn;
				wn *= w;
			}
		}

		// scaling only during inverse transformation
		if (invert)
			for (auto& c : y)
				c /= N;

		return y;
	}

	//==========================================================================================================================//
	//													r o u n d _ c o m p l e x												//
	//==========================================================================================================================//
	/**
	 * @brief Rounds a complex number to a fixed precision of 12 decimal places. Clamps values extremely close to 0.0 directly
	 * to absolute zero to eliminate accumulated floating-point arithmetic noise in the terminal output.
	 * @param c the complex-valued number to be conditioned
	 * @return a new `std::complex<double>` with rounded and cleaned components
	 */
	inline std::complex<double> round_complex(const std::complex<double>& c)
	{
		double re = std::round(c.real() * FACTOR) / FACTOR;
		double im = std::round(c.imag() * FACTOR) / FACTOR;

		if (std::abs(re) < EPSILON) re = 0.0;
		if (std::abs(im) < EPSILON) im = 0.0;

		return { re, im };
	}

	//==========================================================================================================================//
	//															p r i n t														//
	//==========================================================================================================================//
	/**
	 * @brief Outputs the identifying text of a "signal" and its samples ​​to the console
	 * @param label the identifying text of a "signal"
	 * @param v the samples of a complex-valued "signal"
	 */
	inline void print(const std::string& label, const std::vector<std::complex<double>>& v)
	{
		std::cout << label << ": ";
		for (auto& c : v)
			std::cout << round_complex(c) << " ";
		std::cout << "\n";
	}
}
