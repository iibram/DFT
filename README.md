# Optimized DFT according to lecture

#### Signal Abstraction Layer

The Signal struct serves as a convenient wrapper and workflow manager designed specifically to work alongside the stateless DFT namespace. While the DFT engine independently handles the pure mathematical transformations, Signal manages the execution order, state transition (time/frequency domain), and automated console output, providing a clean and intuitive API for the user.
