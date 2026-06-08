#pragma once
#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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
		int N = x.size();

		std::vector<std::complex<double>> y(N);

		// optimized loop
		for (int n = 0; n < N; n++)
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
	 * @brief Rounds up to 15 decimal places and sets values ​​close to 0 directly to 0.
	 * @param c complex-valued number
	 * @param decimals amount of decimal places to which rounding is performed (default: 15)
	 * @return passed complex number (rounded)
	 */
	inline std::complex<double> round_complex(const std::complex<double>& c, int decimals = 15)
	{
		double factor = std::pow(10.0, decimals);
		double re = std::round(c.real() * factor) / factor;
		double im = std::round(c.imag() * factor) / factor;
		if (std::abs(re) < 1e-15)
			re = 0.0;
		if (std::abs(im) < 1e-15)
			im = 0.0;
		return { re, im };
	}

	//==========================================================================================================================//
	//															p r i n t														//
	//==========================================================================================================================//
	/**
	 * @brief Outputs the identifying text of a "signal" and its samples ​​to the console
	 * @param label the identifying text of a "signal"
	 * @param v the samples of a complex-valued "signal"
	 * @param decimals amount of decimal places to which rounding is performed (default: 15)
	 */
	inline void print(const std::string& label, const std::vector<std::complex<double>>& v, int decimals = 15)
	{
		std::cout << label << ": ";
		for (auto& c : v)
			std::cout << round_complex(c, decimals) << " ";
		std::cout << std::endl;
	}
}
