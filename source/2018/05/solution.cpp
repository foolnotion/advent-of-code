#include <aoc.hpp>

template<>
auto advent2018::day05() -> result {
    auto lines = aoc::util::readlines("source/2018/05/input.txt");
    auto input = lines.front();

    auto opposites = [](char a, char b) {
        return a != b && std::tolower(a) == std::tolower(b);
    };

    auto react = [&](std::string const& polymer) {
        std::string left;
        for (auto c : polymer) {
            if (!left.empty() && opposites(left.back(), c)) { left.pop_back(); }
            else                                            { left += c; }
        }
        return left.size();
    };

    auto remove_reactant = [](std::string polymer, char c) {
        std::erase_if(polymer, [&](auto x) { return std::tolower(x) == c; });
        return polymer;
    };

    auto const p1 = react(input);
    auto const p2 = std::ranges::min(aoc::constants::alphabet | std::views::transform([&](auto c) {
        return react(remove_reactant(input, c));
    }));
    return aoc::result(p1, p2);
}