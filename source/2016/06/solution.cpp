#include <aoc.hpp>

template<>
auto advent2016::day06() -> result {
    auto input = aoc::util::readlines("./source/2016/06/input.txt");

    auto get_most_least_frequent = [](std::string_view s) -> std::tuple<char, char> {
        auto letters = aoc::constants::alphabet;
        std::array<u32, aoc::constants::alphabet.size()> counts{};
        for (auto c : s) { ++counts[c-'a']; }
        std::ranges::sort(letters, std::less{}, [&](auto x){ return counts[x-'a']; });
        return {letters.front(), letters.back()};
    };

    using Eigen::Array;
    using Eigen::Map;

    Array<char, -1, -1> code(std::ssize(input), std::ssize(input.front()));
    for (auto&& [i, s] : lz::enumerate(input)) {
        code.row(i) = Map<Array<char, -1, 1>>(s.data(), std::ssize(s));
    }
    std::string part1;
    std::string part2;
    for (auto c : code.colwise()) {
        auto [l, m] = get_most_least_frequent({c.data(), input.size()});
        part1.push_back(m);
        part2.push_back(l);
    }
    return aoc::result(part1, part2);
};
