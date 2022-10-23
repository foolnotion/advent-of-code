#include <aoc.hpp>

template<>
auto advent<2015>::day05() const -> void {
    std::fstream f("./source/2015/05/input.txt");

    auto is_nice_part1 = [&](auto const& str) {
        static constexpr std::array forbidden = { "ab", "cd", "pq", "xy" };
        auto v = std::ranges::count_if(str, &aoc::equals<'a', 'e', 'i', 'o', 'u'>);
        if (v < 3) { return false; }

        bool contains_double{false};
        bool contains_forbidden{false};
        for (auto i = 0UL; i < str.size()-1; ++i) {
            std::string_view sv{str.data()+i, 2};
            contains_double |= sv[0] == sv[1];
            contains_forbidden |= std::ranges::any_of(forbidden, [&](auto const& f) {
                return f == sv;
            });
        }
        return contains_double && !contains_forbidden;
    };

    auto has_non_overlapping_pair = [](auto const& str) {
        std::vector<std::string_view> pairs;
        pairs.reserve(str.size()-1);
        for (auto i = 0L; i < std::ssize(str)-1; ++i) {
            pairs.emplace_back(str.data()+i, 2);
        }
        for (auto i = 0L; i < std::ssize(pairs)-2; ++i) {
            for (auto j = i+2; j < std::size(pairs); ++j) {
                if (pairs[i] == pairs[j]) { return true; }
            }
        }
        return false;
    };

    auto has_repeated_letter = [](auto const& str) {
        for (auto i = 0L; i < std::ssize(str)-2; ++i) {
            if (str[i] == str[i+2]) { return true; }
        }
        return false;
    };

    auto is_nice_part2 = [&](auto const& str) {
        return has_repeated_letter(str) && has_non_overlapping_pair(str);
    };

    u64 nice1{0};
    u64 nice2{0};
    for (std::string s; std::getline(f, s); ) {
        nice1 += is_nice_part1(s);
        nice2 += is_nice_part2(s);
    }
    fmt::print("part 1: {}\n", nice1);
    fmt::print("part 2: {}\n", nice2);
}
