# 📈 Dual-Signal Optimized DFT (Discrete Fourier Transform)

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg) ![Purpose](https://img.shields.io/badge/Purpose-Educational-orange.svg) ![Category](https://img.shields.io/badge/Category-Signal%20Processing-teal.svg) ![OS](https://img.shields.io/badge/OS-Windows%20%26%20Linux-purple.svg)

Welcome to a highly optimized, clean C++20 implementation of the **Discrete Fourier Transform (DFT)** and **Inverse Discrete Fourier Transform (IDFT)**.

This project represents a complete, high-level refactoring of a traditional academic DFT algorithm. By leveraging modern C++ design patterns and advanced signal processing mathematics, this engine is capable of packing **two distinct real-world signals into a single complex-valued stream** (Signal 1 as Real, Signal 2 as Imaginary). This allows the system to transform both signals into the frequency domain simultaneously in a single mathematical pass, decoupling them cleanly via a dedicated extraction routine.

## 🚀 Key Features

* **Dual-Signal Processing Optimization:** Pack two parallel signals into the Real and Imaginary components of a single complex array. Perform one DFT pass instead of two, and retrieve both fully decoupled signals in the frequency domain.
* **Unified Transform Engine:** The exact same core algorithm handles both the forward DFT (time -> frequency) and the inverse IDFT (frequency -> time) to keep the codebase dry and ultra-efficient.
* **High-Speed Micro-Rounding:** Uses a compile-time `FACTOR` and `EPSILON` combination to suppress residual floating-point noise and achieve lightning-fast rounding down to the 12th decimal place.
* **Zero-Knowledge API:** The user only interacts with an intuitive `Signal` interface. All raw mathematical heavy lifting and state management are tucked away safely under the hood.
* **Cross-Platform Readiness:** Written in standard, modern C++20, ensuring flawless compilation and identical performance on both Windows and Linux (e.g., native setups, WSL, or CachyOS).

---

## 🛠️ System Architecture & Code Structure

The project follows a clean, modern, header-only logic design separated into mathematical operations and state management:

```
DFT/
├── src/
│   ├── DFT.h         # Division algorithms & Square-and-Multiply (namespace)
│   ├── Signal.h      # RSA class declaration (interface)
│   └── main.cpp      # Application entry point (instantiation & execution only)
├── .gitignore        # Specifies intentionally untracked files to ignore
├── LICENSE           # MIT License File
└── README.md         # Project documentation and architecture overview
```

---

### Component Breakdown

#### 🧮 1. The Stateless `DFT` Namespace
The `DFT` namespace acts as the pure mathematical engine of the application. It is stateless and completely independent. It holds the core inline DFT/IDFT algorithm, custom high-speed rounding routines, and formatting helpers. To maximize precision while keeping an independent footprint, it implements its own PI fallback if not provided by the environment:
* `M_PI` defined as `3.14159265358979323846` if not already present.
* Bit-noise suppression constants: `FACTOR = 1e12` (isolates the first 12 decimal places) and `EPSILON = 1e-12` (cleans out residual floating-point noise).

#### 📡 2. The `Signal` Abstraction Layer
The `Signal` struct serves as a convenient wrapper and workflow manager designed specifically to work alongside the stateless DFT namespace. While the DFT engine independently handles the pure mathematical transformations, `Signal` manages the execution order, state transition (time/frequency domain), and automated console output, providing a clean and intuitive API for the user.

* **Smart Labeling & Dual Construction:** `Signal` tracks its own naming identifiers. It automatically handles naming states (lowercase for time-domain, uppercase for frequency-domain).
* **The Dual-Split Constructor:** When a user calls `split_dual()`, an internal secondary constructor is triggered. It automatically creates two isolated signals, appends a `_1` or `_2` suffix to the base identifier, and separates the mathematical components so the user can easily manage the decoupled results.

---

## 💻 Usage & Verification Example

The system is designed to be fully descriptive yet incredibly simple to execute. Here is how a dual-signal setup is initialized, simultaneously transformed, separated, and converted back into individual time-domain sequences:

```cpp
#include "Signal.h"

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
```

---

## 🔨 Compilation & Build

Since the entire logic is cleanly encapsulated within inline structures inside the header files, compiling the project is straightforward and requires no external dependencies.

### Prerequisites
* A modern C++ compiler supporting **C++20** (e.g., `g++` 11+, `clang` 13+, or MSVC latest).

### Build Command
To build the demonstration program on Linux or Windows (via MinGW/WSL), execute the following command in your terminal:

```bash
# Clone the repository
git clone https://github.com/iibram/DFT.git

# Change directory
cd DFT

# Build using g++ with O2 Optimization flag
g++ -std=c++20 -O2 src/main.cpp -o dft_demo

# Run the app
./dft_demo
```

---

## 🗺️ Future Roadmap

* [ ] **Interactive CLI Mode:** 🕹️
  * Upgrade the demonstration into an interactive console workflow allowing users to type in custom signals and manually trigger dual-processing modes via standard input.
* [ ] **Dynamic Signal Visualization:** 📈
  * Implement a lightweight ASCII-based terminal plotter to visualize the signals directly within the console before and after transformation.
