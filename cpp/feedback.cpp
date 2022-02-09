#pragma once
#include <array>
#include <iostream>
#include <set>
#include <vector>

#include "word.cpp"

/*
    clue_t : The states of a word in the game
*/

typedef enum
{
    missed,
    shifted,
    fixed
} clue_t;

// Array of states

typedef std::array<clue_t, word_size> feedback_t;

// Printer

std::ostream &operator<<(std::ostream &os, const feedback_t &arg)
{
    for (auto ii : arg)
    {
        switch (ii)
        {
        case missed:
            os << "M";
            break;
        case shifted:
            os << "S";
            break;
        case fixed:
            os << "F";
            break;
        default:
            break;
        }
    }
    return os;
}

/*
    set_of_feedback_arrays : Hold a collection of unique state arrays
*/

class set_of_feedback_arrays : public std::set<feedback_t>
{
private:
    // This is used to fill up the state array with all combintations recursively
    void backtrack_fill(
        feedback_t &buffer,
        const size_t &depth)
    {
        if (depth == buffer.size())
        {
            // Base case aat depth
            this->insert(buffer);
            return;
        }
        else
        {
            // Branching

            buffer[depth] = missed;
            backtrack_fill(buffer, depth + 1);

            buffer[depth] = shifted;
            backtrack_fill(buffer, depth + 1);

            buffer[depth] = fixed;
            backtrack_fill(buffer, depth + 1);
            return;
        }
    }

public:
    // Default create full
    set_of_feedback_arrays()
    {
        fill();
    }

    // Recursive head for filling
    void fill()
    {
        this->clear();
        feedback_t buffer;
        backtrack_fill(buffer, 0);
    }

    // Printer
    friend std::ostream &operator<<(std::ostream &os, const set_of_feedback_arrays &arg)
    {
        for (auto ii : arg)
        {
            os << ii << "\n";
        }
        return os;
    }

    // This pruning is used to trim down the possible
    // states for the fixed states
    void prune_fixed(const feedback_t &other)
    {
        size_t counter = 0;
        for (auto ii : other)
        {
            // Look for fixed across all states
            if (ii == fixed)
            {
                // Erase all states that don't have this fixed location
                for (auto it = this->begin(); it != this->end();)
                {
                    if ((*it)[counter] != fixed)
                    {
                        it = this->erase(it);
                    }
                    else
                    {
                        it++;
                    }
                }
            }
            counter++;
        }
    }
};

/*
    Compare a gues with the truth and return the feedback state
*/

feedback_t score_guess(
    const word_t &guess,
    const word_t &truth)
{
    // Assume all are missed
    feedback_t result = {missed};
    // Mark all locations as unclaimed by a score
    std::vector<bool> claimed(guess.size(), false);

    // Go through the guess characters looking for fixed matches
    for (size_t ii = 0; ii < guess.size(); ii++)
    {
        // Unpack guess and truth
        char g = guess[ii];
        char t = truth[ii];

        // If equivalent
        if (g == t)
        {
            // Assign fixed state
            result[ii] = fixed;
            // Mark as claimed
            claimed[ii] = true;
        }
    }

    // Go through the guess characters looking for shifted matches
    for (size_t ii = 0; ii < guess.size(); ii++)
    {
        char g = guess[ii];
        // Seach through the characters in the answer
        for (size_t jj = 0; jj < guess.size(); jj++)
        {
            char t = truth[jj];

            // If found a match
            if (g == t)
            {
                // If this match has not already been claimed
                if (claimed[jj] == false)
                {
                    // Mark it as shifted
                    result[ii] = shifted;
                    // and claimed
                    claimed[jj] = true;
                }
            }
        }
    }
    // Finished
    return result;
}