  
# Sieve of Atkin Implementation in C++

## Description

This project provides a C++ implementation of a modified version of the Sieve of Atkin algorithm using concurrent principles (is parallelized). The Sieve of Atkin is a modern algorithm for finding all prime numbers up to a specified integer.

  
## Installation
### Prerequisites

- C++ Compiler (e.g., GCC, Clang)

- CMake (optional for build automation)

  
### Steps

1. Clone the repository:

```bash

git clone [URL to the repository]

```

2. Navigate to the project directory:

```bash

cd [project-directory]

```

3. Compile the source code:

```bash

g++ -std=c++17 -o main main.cpp

```
  
## Usage Example

  1. Run the compiled program:

```bash

./main

```
This will execute the Sieve of Atkin algorithm using the thread pool to find primes up to a predefined limit.

  

2. Check the output:
The program generates a file `output.txt` containing all prime numbers found.

  