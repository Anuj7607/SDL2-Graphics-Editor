# Computer Graphics Editor (C++ / SDL2)

## Objective
A lightweight, high-performance computer graphics editor engineered from scratch to demonstrate low-level system operations, core rendering algorithms, and efficient memory management. This project intentionally bypasses high-level standard library abstractions in favor of manual data structure implementations and foundational loop structures to optimize execution speed and maintain strict resource control.

## Technologies Used
*   **Language:** C++ (`main.cpp`)
*   **Graphics API:** SDL2 (Hardware-accelerated 2D rendering)
*   **Build Environment:** Windows Batch Scripting (`build.bat`)

## Key Architectural Features
*   **Low-Level Memory Management:** Custom implementation of fundamental data structures, ensuring deterministic memory allocation and deallocation without the overhead of the standard library.
*   **Algorithmic Rendering:** Core rasterization, pixel manipulation, and rendering logic built using basic, highly-optimized control flow.
*   **Minimalist Architecture:** Direct integration with the `SDL2.dll` to handle window creation, event polling, and pixel pushing with minimal dependencies.

## Getting Started

### Prerequisites
*   A Windows environment.
*   A standard C++ compiler (e.g., GCC/MinGW or MSVC) configured in your system PATH.

### Build and Run
1. Clone the repository to your local machine.
2. Compile the source code using the included build script:
   ```bash
   build.bat