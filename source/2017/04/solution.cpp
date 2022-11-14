#include <aoc.hpp>

template<>
auto advent2017::day04() -> result {

    std::fstream f("./source/2017/04/input.txt");
    std::vector<std::vector<u64>> words;
    std::vector<std::vector<u64>> anagrams;

    for (std::string line; std::getline(f, line); ) {
        std::vector<u64> word;
        std::vector<u64> anagram;
        for (auto s : lz::split(line, ' ')) {
            auto h = aoc::util::hash{}(s);
            word.push_back(h);

            std::string str(s.begin(), s.end());
            std::ranges::sort(str);
            anagram.push_back(aoc::util::hash{}(str));
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
    auto p1 = std::ranges::count_if(words, is_valid);
    auto p2 = std::ranges::count_if(anagrams, is_valid);
    return aoc::result(p1, p2);
}
