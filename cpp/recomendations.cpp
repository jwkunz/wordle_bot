#pragma once

#include <queue>
#include <list>
#include <math.h>

#include "word.cpp"
#include "word_bank.cpp"
#include "feedback.cpp"
#include "filters.cpp"

/*
    This method is used to compute the entropy associated with a word 
    given the available states and word bank    
*/

double compute_word_entropy(
    const word_t &word,
    const word_bank &bank,
    const set_of_feedback_arrays &states)
{

    // This object is used to collect the parallel evaluations
    class worker
    {
    public:
        // Cache of shared variables
        std::vector<double> results;
        word_t word;
        word_bank bank;
        set_of_feedback_arrays states;

        worker(
            const word_t &word_,
            const word_bank &bank_,
            const set_of_feedback_arrays &states_)
        {
            word = word_;
            bank = bank_;
            states = states_;
            results.resize(states.size());
        }

        // This gets called by each thread
        void operator()(const feedback_t &other)
        {

            // Generate filters according to a candidate word
            position_rule pfilt = make_position_rule(word, other);
            count_rule cfilt = make_count_rule(word, other);

            // Apply filter
            word_bank filtered = bank.matches(pfilt, cfilt);

            // Size of matching state
            double N = filtered.size();
            // Probability
            double p = N / bank.size();
            // Entropy formula, handle the zero singularity
            double entropy = 0;
            if (N > 0)
            {
                entropy = -p * log2(p);
            }

            // Store result in vector (for asynchronous collection)
            size_t index = std::distance(states.begin(), states.find(other));
            results[index] = entropy;
        }
    };

    // Allocate memory
    worker temp(word, bank, states);

// Run the parallel execution
#pragma omp parallel
    {
        for (auto ii : states)
        {
            temp(ii);
        }
    }

    // Accumulate the parallel result after execution
    double sum = 0;
    for (auto ii : temp.results)
    {
        sum += ii;
    }

    // Final answer
    return sum;
}


/*
    A server that makes recommendations based on maximum entropy 
*/

void recommendation_generator(
    const word_t &last_guess,                             // The user's guess
    const feedback_t &feedback_given,                     // The game's feedback for the guess
    word_bank &filtered_word_bank,                        // A word bank of possibilities that reduces as the game plays
    set_of_feedback_arrays &available_states,             // Contains the states still in play
    std::list<std::pair<word_t, double>> &recommendations // The returned list of recommendations sorted best at front
)
{

    // Prune any fixed states
    available_states.prune_fixed(feedback_given);
    // Generate filters
    position_rule p = make_position_rule(last_guess, feedback_given);
    count_rule c = make_count_rule(last_guess, feedback_given);
    // Apply filter
    word_bank temp = filtered_word_bank.matches(p, c);
    // Keep the filtered word bank (deep copy)
    filtered_word_bank.clear();
    filtered_word_bank.reserve(temp.size());
    for(auto ii : temp){
        filtered_word_bank.push_back(ii);
    }

    // Compute entropy and sort the result
    std::priority_queue<std::pair<double, word_t>> sorter;
    for (auto ii : filtered_word_bank)
    {
        double H = compute_word_entropy(ii, filtered_word_bank, available_states);
        sorter.push({-H, ii});
    }

    // Fill up recomendations in sorted order
    recommendations.clear();
    while (sorter.empty() == false)
    {
        auto temp = sorter.top();
        sorter.pop();
        recommendations.push_front({temp.second, -temp.first});
    }

    // Done
}