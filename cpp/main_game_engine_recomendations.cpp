/*
    This is a text based wordle game.

    This version make recomendations i.e it cheats for you
*/

#include "game_engine.cpp"

int main()
{
    std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    //std::string fname = "../data/20k.txt";

    game_engine<6,10> wordle(fname);
    wordle.play();
}