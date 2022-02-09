#pragma once
#include <array>
#include <iostream>
#include <map>

#include "word.cpp"
#include "feedback.cpp"

/*
    count_rule : A dictionary of letter frquency constraints
*/

typedef std::map<char, uint8_t> count_rule;

// Counts the frequency of letters in a word

count_rule count_frequency(
    const word_t &arg)
{
    count_rule result;
    for (auto ii : arg)
    {
        auto status = result.insert({ii, 1});
        if (status.second == false)
        {
            status.first->second += 1;
        }
    }
    return result;
}

count_rule make_count_rule(
    const word_t &word,
    const feedback_t &state_array)
{
    count_rule result;
    size_t counter = 0;
    for (auto ii : state_array)
    {
        auto status = result.insert({word[counter], 0});
        if (ii != missed)
        {
            status.first->second++;
        }
        counter++;
    }
    return result;
}

// Printer

std::ostream &operator<<(std::ostream &os, const count_rule &arg)
{
    for (auto ii : arg)
    {
        os << "{" << ii.first << " : " << int(ii.second) << "}\n";
    }
    return os;
}

/*
    position_rule : A container of postion constraints
*/

typedef enum
{
    force,
    open,
    prevent
} position_constraint_t;

// ex: 'c' 1 'fix'
typedef std::array<std::pair<char, position_constraint_t>, word_size> position_rule;

position_rule make_position_rule(
    const word_t &word,
    const feedback_t &state_array)
{
    position_rule result;
    size_t counter = 0;
    for (auto ii : state_array)
    {
        switch (ii)
        {
        case fixed:
            result[counter] = {word[counter], force};
            break;
        case shifted:
            result[counter] = {word[counter], prevent};
            break;
        case missed:
            result[counter] = {word[counter], open};
            break;
        default:
            break;
        }
        counter++;
    }
    return result;
};

// Printer

std::ostream &operator<<(std::ostream &os, const position_rule &arg)
{
    for (auto ii : arg)
    {
        os << "{" << ii.first << " : ";
        switch (ii.second)
        {
        case force:
            os << "force";
            break;
        case open:
            os << "open";
            break;
        case prevent:
            os << "prevent";
            break;
        default:
            break;
        }
        os << "}\n";
    }
    return os;
}
