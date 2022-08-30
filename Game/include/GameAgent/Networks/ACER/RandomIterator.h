#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <stdexcept>

/*
  RandomIterator
   
  A C++ class that can generate random, unique, and evenly-distributed 
  numbers with O(1) memory space.
 
  Author: Tyler Burdsall 2018
  Source: https://tylerburdsall.medium.com/generating-unique-random-and-evenly-distributed-numbers-in-ascending-order-with-c-and-in-o-1-9dd5be0c0fcd
*/
class RandomIterator
{
public:
    /* 
      Constructor for RandomIterator class.
      params:
            - amount: Amount of numbers to generate
            - min: Minimum number in range to generate
            - max: Maximum number in range to generate
     */
    RandomIterator(
        const unsigned long long& amount, 
        const unsigned long long& min, 
        const unsigned long long& max) : 
        gen((std::random_device())())
    {
        floor = min;
        num_left = amount;
        last_k = min;
        n = max;
    }

    /* 
      Returns a bool to determine if there are any numbers left to generate.
    */
    const bool has_next(void)
    {
        return num_left > 0;
    }

    /*
      Generates the next random number.
    */ 
    const unsigned long long next(void)
    {
        if (num_left > 0)
        {
            // Partition the range of numbers to generate from
            unsigned long long range_size = (n - last_k) / num_left;

            // Initialize random generator
            std::uniform_int_distribution<unsigned long long> rnd(floor, range_size);

            // Generate random number
            unsigned long long r = rnd(gen) + last_k + 1;

            // Set last_k to r so that r is not generated again
            last_k = r;
            num_left--;
            return r;
        }
        else
        {
            throw std::out_of_range("Exceeded amount of random numbers to generate.");
        }
    }

private:
    unsigned long long floor;
    unsigned long long n;
    unsigned long long last_k;
    unsigned long long num_left;
    std::mt19937_64 gen;
};
