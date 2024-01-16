// C++ program for implementation of Sieve of Atkin
// #include <bits/stdc++.h>
#include <atomic>
#include <iostream>
#include <fstream>
#include <vector>
#include "thread_pool.cpp"

// Setting up a limit for generating primes up to  
const size_t LIMIT = 99999999;

// Array of primes
std::vector<size_t> primes = {};

// Array of atomic bools, true for prime number and false for non-primes 
std::vector<std::atomic_bool> sieve(LIMIT);

// Function to generate primes till limit using Sieve of Atkin
void SieveOfAtkin(size_t limit)
{
    // Creating a threadpool in which we are going to submit the tasks(1st, 2nd and 3rd) down belows
    auto* tp= new thread_pool{};

    // Initialise the sieve array with initial false values
    for (int i = 0; i <= limit; i++)
        sieve[i] = false;

    // 2 and 3 are known to be prime
    if (limit > 2)
        sieve[2] = true;
    if (limit > 3)
        sieve[3] = true;

    // 1. The condition n = (4*x*x) + (y*y) yields an odd number of distinct (x, y) pairs that satisfy n % 12 = 1 or n % 12 = 5
    for (size_t x = 1; 4 * x * x <= limit; x++) {
        tp->enqueue(
       [=]() {
                for (size_t y = 1; (4 * x * x) + (y * y) <= limit; y++) {
                    size_t n = (4 * x * x) + (y * y);
                    if ((n % 12 == 1 || n % 12 == 5))
                        sieve[n] = !sieve[n];
                }
        }
    );
    }

    // 2. The condition n = (3*x*x)+(y*y) yields odd number of solutions and n % 12 = 7.
    for (size_t x = 1; (3 * x * x) <= limit; x++) {
        tp->enqueue(
    [=]() {
            for (size_t y = 1; (3 * x * x) + (y * y) <= limit; y++) {
                size_t n = (3 * x * x) + (y * y);
                if (n % 12 == 7)
                    sieve[n] = !sieve[n];
                }
        });
    }

    // 3. The condition n = (3*x*x)-(y*y) has odd number of solutions, x > y and n % 12 = 11 */
    for (size_t y = 1; y <= limit; y++) {
        tp->enqueue(
    [=]() {
           for (size_t x = y+1; (3 * x * x) - (y * y) <= limit; x++) {
               size_t n = (3 * x * x) - (y * y);
               if (n % 12 == 11)
                   sieve[n] = !sieve[n];
           }
        });
    }

    // Waiting for the threads in the pool to finish their job
    delete tp;

    // Initalizing the threadpool again to subumit the 4th task 
    tp = new thread_pool{};

    // Marking all multiples of squares as non-prime
    for (int r = 5; r * r <= limit; r++) {
        if (sieve[r]) {
            tp->enqueue(
            [=]() {
                for (size_t i = r * r; i <= limit; i += r * r)
                    sieve[i] = false;
            });
        }
    }

    // Waiting for the threads in the pool to finish their job
    delete tp;

    // Updating primes vector
    for (size_t a=1; a <= limit; a++) {
        if (sieve[a]) {
            primes.push_back(a);
        }
    }
}


// Function for writing the primes to the file 
void write_primes_to_file(const std::string& filename) {
    std::cout << "writing primes to file\n";

    // Opening the file for writing
    std::ofstream outputFile(filename);

    // Checking if the file is successfully opened
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        return;
    }

    // Writing each prime number to the file
    for (int prime : primes) {
        outputFile << prime << '\n';
    }

    // Closing the file
    outputFile.close();
}

int main(void)
{
    // Filling the sieve array with false
    std::fill(sieve.begin(),sieve.end(),false);

    // Calling the function SieveOfAtkin with the limit as the parameter 
    SieveOfAtkin(LIMIT);

    // Calling the function write_primes_to_file to write the generated primes into a file 
    write_primes_to_file("output.txt");

    return 0;
}
