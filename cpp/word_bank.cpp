#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <string>

#include "word.cpp"
#include "filters.cpp"

/*
    word_bank : Object that hold a vector of word_t
*/

class word_bank : public std::vector<word_t>
{
public:
    // Initialize from a txt file in ASCII format "word\n"

    void from_txt(
        std::string fname)
    {
        // Reset
        this->clear();

        // Open file
        FILE *ifid = fopen(fname.c_str(), "r");
        if (ifid == NULL)
        {
            throw std::invalid_argument("Filename: " + fname + " does not exist");
        }

        // Parse char's
        word_t buffer;
        char c = 0;
        size_t counter = 0;
        do
        {
            // Get a char
            c = fgetc(ifid);
            // Check for newline
            bool is_end = (c == '\n');
            // Newline at the end of a word_t length
            if ((counter == word_size) && (is_end))
            {
                // Add to buffer
                this->push_back(buffer);
                counter = 0;
            }
            // Regular newline case
            else if (is_end)
            {
                counter = 0;
            }
            // Otherwise, store in the buffer
            else if (counter < word_size)
            {
                buffer[counter++] = c;
            }
            // Overfull buffer, but proceed to newline
            else
            {
                counter++;
            }
        } while (c != EOF);

        this->shrink_to_fit();
    }

    // Filter based on a position constraint
    // Return a reduced word bank

    word_bank matches_position(
        const position_rule &constraint) const
    {
        // Create result buffer
        word_bank result;
        
        // Preallocate
        result.reserve(this->size());

        // For every word
        for (auto ii : *this)
        {
            // Assume matching until proven otherwise
            bool match = true;

            // For each letter
            for (size_t jj = 0; jj < constraint.size(); jj++)
            {
                // Unpack
                char c = constraint[jj].first;
                position_constraint_t p = constraint[jj].second;

                switch (p)
                {
                case force:
                    // Letter must match
                    if (ii[jj] != c)
                    {
                        match = false;
                    }
                    break;
                case open:
                    // No constraint
                    continue;
                    break;
                case prevent:
                    // Letter must match
                    if (ii[jj] == c)
                    {
                        match = false;
                    }
                    break;
                default:
                    break;
                }

                // Cut out on first fail
                if (match == false)
                {
                    break;
                }
            }

            // If still a possible match, it is
            if (match)
            {
                // Store in new word bank
                result.push_back(ii);
            }
        }

        // Return
        result.shrink_to_fit();
        return result;
    }

    // Filter based on a count constraint
    // Return a reduced word bank

    word_bank matches_counts(
        count_rule &constraint) const
    {
        // Create result buffer
        word_bank result;
        
        // Preallocate
        result.reserve(this->size());

        // For every word
        for (auto ii : *this)
        {
            // Get the counts of this word
            count_rule word_counts = count_frequency(ii);

            // Assume matching until proven not
            bool match = true;

            for (auto it = constraint.begin(); it != constraint.end(); it++)
            {
                // Look for data of matching constraint
                auto search = word_counts.find(it->first);
                if (search != word_counts.end())
                {
                    // If found

                    // If the constraint requires zero
                    if (it->second == 0)
                    {
                        // Verify if zero
                        if (search->second == 0)
                        {
                            continue;
                        }
                        else
                        {
                            // Can't match
                            match = false;
                            break;
                        }
                    }
                    else
                    {
                        // Constraint is bigger than zero

                        // Are there enough counts to satisfy?
                        if (search->second >= it->second)
                        {
                            continue;
                        }
                        else
                        {
                            // Can't match
                            match = false;
                            break;
                        }
                    }
                }
                else
                {
                    // Not found
                    if (it->second == 0)
                    {
                        // Its ok, its not a pass
                        continue;
                    }
                    else
                    {
                        match = false;
                        break;
                    }
                }
            }

            // Add matching results
            if (match)
            {
                result.push_back(ii);
            }
        }
        
        // Return
        result.shrink_to_fit();
        return result;
    }

    // Check for both criteria at once, efficiently

    word_bank matches(
        const position_rule &position,
        count_rule &counts) const
    {
        word_bank temp = this->matches_position(position);
        return temp.matches_counts(counts);
    }

    // Printer

    friend std::ostream &operator<<(std::ostream &os, const word_bank &arg)
    {
        for (auto ii : arg)
        {
            os << ii << "\n";
        }
        os << "Count: " << arg.size() << std::endl;
        return os;
    }
};