#include <aoc.hpp>

template<>
auto advent2022::day03() -> result {
    std::array<i32, aoc::constants::alphabet.size()> priorities{};
    std::iota(priorities.begin(), priorities.end(), 1);
    auto get_priority = [&](auto c) {
        return priorities[std::tolower(c)-'a'] + (std::isupper(c) ? std::ssize(priorities) : 0);
    };
    constexpr auto groupsize{3};
    auto part1{0L};
    auto part2{0L};

    aoc::dense::set<char> q;
    std::vector<char> v;
    auto input = aoc::util::readlines("./source/2022/03/input.txt");

    for (auto&& [i, line] : lz::enumerate(input)) {
        auto const z = line.size() / 2;
        std::string_view a{&line[0], z}; // NOLINT
        std::string_view b{&line[z], z};
        aoc::dense::set<char> s(b.begin(), b.end());

        auto const* p = std::ranges::find_if(a, [&](auto c) { return s.contains(c); });
        part1 += get_priority(*p);

        if (q.empty()) {
            q = aoc::dense::set<char>(line.begin(), line.end());
        } else {
            v.clear();
            std::ranges::copy_if(line, std::back_inserter(v), [&](auto c) { return q.contains(c); });
            q = aoc::dense::set<char>(v.begin(), v.end());
        }
        if ((i+1) % groupsize == 0) {
            part2 += get_priority(q.values().front());
            q.clear();
        }
    }

    return aoc::result(part1, part2);
}
