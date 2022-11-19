#include <aoc.hpp>

using std::ranges::any_of;
using std::ranges::none_of;

template <>
auto advent2016::day07() -> result {
    auto input = aoc::util::readlines("./source/2016/07/input.txt");

    auto abba = [](std::string_view s) {
        auto const sz = std::ssize(s);
        for (auto i = 0; i < sz/2; ++i) {
            if (s[i] != s[sz-i-1]) { return false; }
        }
        return s[0] != s[1];
    };

    auto has_abba = [&](std::string_view s) {
        auto const sz = std::ssize(s);
        for (auto i = 0; i < sz-3; ++i) {
            std::string_view v{&s[i], 4};
            if (abba(v)) { return true; }
        }
        return false;
    };

    auto aba = [](std::string_view s) {
        return s.front() == s.back() && s[0] != s[1];
    };

    auto bab = [](std::string_view s) {
        return std::string{s[1], s[0], s[1]};
    };

    auto part1{0};
    auto part2{0};

    constexpr auto npos{std::string::npos};
    for (auto const& s : input) {
        std::vector<std::string> normal_sequences;
        std::vector<std::string> hypernet_sequences;
        std::string word;

        for (auto c : s) {
            if (c == '[') {
                normal_sequences.push_back(word);
                word.clear();
            } else if (c == ']') {
                hypernet_sequences.push_back(word);
                word.clear();
            } else {
                word.push_back(c);
            }
        }
        if (!word.empty()) { normal_sequences.push_back(word); }

        part1 += any_of(normal_sequences, has_abba) && none_of(hypernet_sequences, has_abba);
        part2 += any_of(normal_sequences, [&](auto s) {
            for (auto i = 0; i < std::ssize(s)-2; ++i) {
                std::string_view v{&s[i], 3};
                if (aba(v)) {
                    auto w = bab(v);
                    if (any_of(hypernet_sequences, [&](auto q) { return q.find(w) != npos; })) {
                        return true;
                    }
                }
            }
            return false;
        });
    }

    return aoc::result(part1, part2);
}
