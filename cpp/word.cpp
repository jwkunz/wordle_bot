#pragma once
#include <array>
#include <iostream>

/*
    word_t:  A container for holding words
*/

const size_t word_size = 5;
typedef std::array<char, word_size> word_t;

// Printer

std::ostream &operator<<(std::ostream &os, const word_t &arg)
{
    for (auto ii : arg)
    {
        os << ii;
    }
    return os;
}