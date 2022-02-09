#include <chrono>

#include "feedback.cpp"
#include "filters.cpp"
#include "recomendations.cpp"
#include "word_bank.cpp"
#include "word.cpp"

////////////////////////////
int main()
{
    word_bank W;
    //std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    std::string fname = "../data/20k.txt";

    W.from_txt(fname);
    std::cout << "Imported: " << W.size() << " words" << std::endl;

    set_of_feedback_arrays A;
    size_t counter = 0;
    for (auto ii : W)
    {
        double H = compute_word_entropy(ii, W, A);
        std::cout << "Entropy: " << H << ", Word:" << ii << ", Progress: " << counter++ << " of " << W.size() << std::endl;
        FILE *ofid = fopen("../data/20k_entropy.bin", "ab");
        fwrite(&ii[0], sizeof(word_t), 1, ofid);
        fwrite(&H, sizeof(double), 1, ofid);
        fclose(ofid);
    }
}