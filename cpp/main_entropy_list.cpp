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
    std::string fname = "../data/google-10000-english-usa-no-swears-medium.txt";
    //std::string fname = "../data/words_alpha.txt";
    //std::string fname = "../data/20k.txt";
    //std::string fname = "../data/short_test.txt";

    W.from_txt(fname);
    std::cout << "Imported: " << W.size() << " words" << std::endl;

    std::priority_queue<std::pair<double,word_t>> sorter;

    set_of_feedback_arrays A;
    size_t counter = 0;
    auto start_of_loop = std::chrono::high_resolution_clock::now();
    for (auto ii : W)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        double H = compute_word_entropy(ii, W, A);
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        double total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_of_loop).count() / 60000.0;
        std::cout << "Entropy: " << H << ", Word: " << ii << ", Time: " << elapsed << " ms, Progress: " << ++counter << " of " << W.size() << ", Total Time: " << total_elapsed << " mins" << std::endl;
        FILE *ofid = fopen("../data/20k_entropy.bin", "ab");
        fwrite(&ii[0], sizeof(word_t), 1, ofid);
        fwrite(&H, sizeof(double), 1, ofid);
        fclose(ofid);

        sorter.push({H,ii});
    }

    std::cout << "\n\nTop Results:\n" << std::endl;
    for(size_t ii = 0; ii < 10; ii++){
        auto top = sorter.top();
        std::cout << "Entropy: " << top.first << ", Word: " << top.second << std::endl;
        sorter.pop();
    }
}