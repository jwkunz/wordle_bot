/*
    A companion to an external wordle game.

    Give the users suggestions based on the feedback given
*/

#include "recomendation_engine.cpp"

int main()
{
    std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    //std::string fname = "../data/20k.txt";

    recomendation_engine advisor(fname);
    advisor.play();
}