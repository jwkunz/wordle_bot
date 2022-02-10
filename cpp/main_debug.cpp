#include <chrono>

#include "feedback.cpp"
#include "filters.cpp"
#include "recomendations.cpp"
#include "word_bank.cpp"
#include "word.cpp"

////////////////////////////

int main()
{
    std::cout << "Hello World" << std::endl;
    word_bank W;
    //std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    std::string fname = "../data/20k.txt";

    W.from_txt(fname);
    std::cout << "Imported: " << W.size() << " words" << std::endl;
    count_rule c1 = {{'a', 1}, {'e', 1}, {'l', 1}, {'p', 0}, {'n', 0}};
    //count_rule c1 = {{'a', 1},{'e',1},{'l',1}};
    auto f1 = W.matches_counts(c1);
    std::cout << "Matched f1: " << f1.size() << " words" << std::endl;
    std::cout << f1 << std::endl;

    position_rule p1 = {(std::pair<char, position_constraint_t>){'a', force}, (std::pair<char, position_constraint_t>){'l', open}, (std::pair<char, position_constraint_t>){'l', prevent}, (std::pair<char, position_constraint_t>){' ', open}, (std::pair<char, position_constraint_t>){' ', open}};
    auto f2 = f1.matches_position(p1);
    std::cout << "Matched f2: " << f2.size() << " words" << std::endl;
    std::cout << f2 << std::endl;

    word_t w = {'t', 'a', 'l', 'e', 's'};
    std::cout << "Word: " << w << std::endl;
    feedback_t s2 = {missed, fixed, shifted, missed, shifted};

    auto c2 = make_count_rule(w, s2);
    auto p2 = make_position_rule(w, s2);
    auto f3 = W.matches(p2, c2);
    std::cout << f3 << std::endl;

    set_of_feedback_arrays A;
    A.fill();
    std::cout << "All States: " << std::endl;
    std::cout << A << std::endl;
    A.prune_fixed({missed, fixed, missed, missed, missed});
    std::cout << "Pruned States" << std::endl;
    std::cout << A << std::endl;

    A.fill();
    std::cout << "Computing Entropy..." << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    double H = compute_word_entropy(w, W, A);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Elapsed Time: " << elapsed << " ms" << std::endl;
    std::cout << "Entropy of " << w << ": " << H << std::endl;
}
