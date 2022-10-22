#include <aoc.hpp>

template<>
auto advent<2017>::day04() const -> void {

    std::fstream f("./source/2017/04/input.txt");
    std::vector<std::vector<u64>> words;
    std::vector<std::vector<u64>> anagrams;

    for (std::string line; std::getline(f, line); ) {
        std::vector<u64> word;
        std::vector<u64> anagram;
        for (auto s : lz::split(line, ' ')) {
            auto h = XXH_INLINE_XXH3_64bits(s.data(), s.size());
            word.push_back(h);

            std::string str(s.begin(), s.end());
            std::ranges::sort(str);
            anagram.push_back(util::hash{}(str));
        }
        std::ranges::sort(word);
        words.push_back(word);

        std::ranges::sort(anagram);
        anagrams.push_back(anagram);
    }

    auto is_valid = [](auto const& vec) {
        for (auto i = 0L; i < std::ssize(vec)-1; ++i) {
            if (vec[i] == vec[i+1]) { return false; }
        }
        return true;
    };
    fmt::print("part 1: {}\n", std::ranges::count_if(words, is_valid));
    fmt::print("part 2: {}\n", std::ranges::count_if(anagrams, is_valid));
}
