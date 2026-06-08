#pragma once
#include "DFT.h"



// =======================================================================================================================================
// 																Declaration
// =======================================================================================================================================

/**
 * @brief Represents a complex-valued signal and acts as a high-level interface for the DFT engine.
 *
 * This struct encapsulates signal data and orchestrates the correct sequence of transformations and logging,
 * abstracting the underlying DFT logic away from the user.
 */
struct Signal
{
	Signal(char t_label, std::vector<std::complex<double>> data);
	Signal(char t_label, std::vector<std::complex<double>> data, std::string xtra);

	void to_freq();
	void to_time();
	std::pair<Signal, Signal> split_dual();

private:
	std::string xtra = "";							// for the expansion of the identifier of this signal (if needed, e.g. when splitting)
	std::vector<std::complex<double>> t_data;		// this signal's data @time-domain
	std::vector<std::complex<double>> f_data;		// this signal's data @frequency-domain
	char t_label = 'x';								// this signal's label @time-domain
	char f_label;									// this signal's label @frequency-domain
};



// =======================================================================================================================================
// 															 inline Definition
// =======================================================================================================================================

/**
 * @brief Custom constructor for real/complex-valued signals @time domain
 * @param t_label the label of the signal in the time domain (e.g., 'x', 'h').
 * Will be automatically converted to lowercase if it is a letter and also the label @frequency domain is generated
 * @param data real/complex-valued samples of the signal
 */
inline Signal::Signal(char t_label, std::vector<std::complex<double>> data) : t_data(std::move(data))
{
	if (std::isalpha(static_cast<unsigned char>(t_label)))
		this->t_label = static_cast<char>(std::tolower(static_cast<unsigned char>(t_label)));

	f_label = this->t_label - 32;

	DFT::print(std::string(1, t_label) + xtra, t_data);
}

/**
 * @brief Custom constructor for complex-valued signals @frequency domain
 * @param t_label the label of the signal in the time domain (e.g., 'x', 'h').
 * Will be automatically converted to lowercase if it is a letter and also the
 * label @frequency domain is generated
 * @param data the complex-valued samples of the signal @frequency domain
 * @param xtra additional identifier to append to the label (default = "") - to use especially when splitting a signal
 */
inline Signal::Signal(char t_label, std::vector<std::complex<double>> data, std::string xtra) : f_data(std::move(data)), xtra(xtra)
{
	if (std::isalpha(static_cast<unsigned char>(t_label)))
		this->t_label = static_cast<char>(std::tolower(static_cast<unsigned char>(t_label)));

	f_label = this->t_label - 32;

	DFT::print(std::string(1, f_label) + xtra, f_data);
}

/**
 * @brief Transforms this signal into the frequency-domain by using the underlaying DFT namespace accordingly and prints the result
 */
inline void Signal::to_freq()
{
	f_data = DFT::dft(t_data);
	DFT::print(std::string(1, f_label), f_data);

	std::cout << "\n";
}

/**
 * @brief Transforms this signal into the time-domain by using the underlaying DFT namespace accordingly and prints the result
 */
inline void Signal::to_time()
{
	if (f_data.size() != 0)
		t_data = DFT::dft(f_data, true);

	DFT::print(std::string(1, t_label) + xtra, t_data);
}

/**
 * @brief Performs a highly efficient simultaneous DFT computation of two real-valued signals packed into a single complex-valued signal
 * (1 @real, 2 @imag). After transformation, this function mathematically separates the interleaved spectra, appends identifiers to their
 * labels, and returns them as an independent pair of Signal objects.
 * @return A `std::pair` containing the two decoupled frequency-domain signals.
 */
inline std::pair<Signal, Signal> Signal::split_dual()
{
	// to_freq() the dual signal
	f_data = DFT::dft(t_data);

	size_t N = f_data.size();
	const std::complex<double> j(0.0, 1.0);

	// temporary containers for both signal samples, reserving space upfront
	std::vector<std::complex<double>> f_d1;
	std::vector<std::complex<double>> f_d2;
	f_d1.reserve(N);
	f_d2.reserve(N);

	// separating the signals (highly efficient single-pass loop)
	for (size_t k = 0; k < N; ++k)
	{
		size_t k_flipped = (k == 0) ? 0 : N - k;
		std::complex<double> X_k = f_data[k];
		std::complex<double> X_k_conj = std::conj(f_data[k_flipped]);

		f_d1.emplace_back((X_k + X_k_conj) / 2.0);
		f_d2.emplace_back((X_k - X_k_conj) / (2.0 * j));
	}

	// instantiating both signals on-the-fly (moving temporary samples to avoid copies)
	Signal s1(f_label, std::move(f_d1), "_1");
	Signal s2(f_label, std::move(f_d2), "_2");

	std::cout << "\n";

	return { s1, s2 };	// returned by value (exploiting NRVO for maximum efficiency)
}
