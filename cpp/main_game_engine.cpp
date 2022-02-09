/*
    This is a text based wordle game.

    This version does not make recomendations
*/

#include "game_engine.cpp"

int main()
{
    std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    //std::string fname = "../data/20k.txt";

    game_engine wordle(fname);
    wordle.play();
}