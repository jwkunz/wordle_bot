#pragma once

#include <random>
#include <time.h>
#include <string>

#include "feedback.cpp"
#include "filters.cpp"
#include "recomendations.cpp"
#include "word_bank.cpp"
#include "word.cpp"

/*
    The game server handeling user interaction 
*/

template <size_t num_rounds = 6, size_t num_suggestions = 0>
class game_engine
{
public:
    size_t round_number;
    word_bank full_word_bank;
    word_t answer;

    word_bank valid_words;
    set_of_feedback_arrays valid_states;

    game_engine(
        std::string word_file_name,
        size_t random_seed = time(NULL))
    {
        srand(random_seed);
        full_word_bank.from_txt(word_file_name);
        round_number = 0;
    }

    void init_game()
    {
        for(size_t ii = 0; ii < 5; ii++){
            std::cout << std::endl;
        }
        std::cout << "Initializing the game..." << std::endl;
        auto item = full_word_bank.begin();
        std::advance(item, rand() % full_word_bank.size());
        answer = *item;

        valid_words = full_word_bank;
        valid_states.fill();

        std::cout << "Ready to play" << std::endl;
    }

    feedback_t get_feedback(
        const word_t &guess)
    {
        return score_guess(guess, answer);
    }

    word_t get_user_input()
    {
        std::string buffer;
        word_t result;

        // Get user input
        std::getline(std::cin, buffer);

        // Sanatize length
        if(buffer.size() != word_size){
            std::cout << "Guess is incorrect size.  Try again." << std::endl;
            return get_user_input();
        }
        // Sanatize characters
        for(size_t ii = 0; ii < result.size(); ii++){
            if(islower(buffer[ii])){
                result[ii] = buffer[ii];
            }else{
                std::cout << "Use only lowercase letters.  Try again." << std::endl;
                return get_user_input();
            }
        }
        
        // Return result
        return result;
    }

    // The game state machine
    void play()
    {

        while (true)
        {
            // Start of game
            if (round_number == 0)
            {
                init_game();
            }

            // End of game
            if (round_number == num_rounds){
                lose();
                return;
            }

            // Prompt
            std::cout << "Round: " << round_number + 1  << std::endl;
            std::cout << "Plase make a guess:" << std::endl;
            word_t guess = get_user_input();
            
            // Did they get it?
            if(guess == answer){
                win();
                return;
            }            

            // Nope, give feedback

            round_number++;
            feedback_t F = get_feedback(guess);
            std::cout << F << std::endl;

            // Give recomendations, if asked
            if(num_suggestions > 0){
                std::cout << "Analyzing Recomendations..." << std::endl;
                std::list<std::pair<word_t, double>> recommendations;
                recommendation_generator(guess,F,valid_words,valid_states,recommendations);
                std::cout << "\n Top suggestions: " << std::endl;
                auto it = recommendations.begin();
                for(size_t ii = 0; ii < num_suggestions; ii++,it++){
                    if(it == recommendations.end()){
                        break;
                    }
                    std::cout << it->first << " : " << it->second << std::endl;
                }
                std::cout << std::endl;
            }

        }
    }

    void win(){
        std::cout << "Congratulations! You WIN!" << std::endl;
        return;
    }

    void lose(){
        std::cout << "Game Over." << std::endl;
        std::cout << "The correct answer was " << answer << std::endl;
        return;
    }

};