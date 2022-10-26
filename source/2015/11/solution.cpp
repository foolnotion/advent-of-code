#include <aoc.hpp>

template<>
auto advent<2015>::day11() const -> void {

    std::string input{"vzbxkghb"};

    struct increment_result {
        char c;
        bool ok;
    };

    auto increment_char = [](char c) -> increment_result {
        if (c == 'z') { return { 'a', true }; }
        return { static_cast<char>(c+1), false };
    };

    auto increment = [&](auto& s) {
        for (auto it = s.rbegin(); it < s.rend(); ++it) {
            auto [c, ok] = increment_char(*it);
            *it = c;
            if (!ok) { break; }
        }
    };

    auto contains_increasing_straight = [](auto const& s) {
        // must include one increasing straight of at least three letters
        auto sz = std::ssize(s);
        return std::ranges::any_of(lz::range(0L, sz-3), [&](auto i) {
            return 1+s[i] == s[i+1] && 1+s[i+1] == s[i+2]; });
    };

    auto contains_non_overlapping_pairs = [](auto const& s) {
        // must contain at least two different, non-overlapping pairs of letters, like aa, bb, or zz
        auto sz = std::ssize(s);
        for (auto i = 0; i < sz-1; ++i) {
            if (s[i] != s[i+1]) { continue; }
            for (auto j = i+2; j < sz-1; ++j) {
                if (s[j] == s[j+1]) { return true; }
            }
        }
        return false;
    };

    auto isvalid = [&](auto const& s) {
        return (std::ranges::none_of(s, &aoc::equals<'i', 'o', 'l'>)
                && contains_increasing_straight(s)
                && contains_non_overlapping_pairs(s));
    };

    auto s = input;
    do { increment(s); } while (!isvalid(s));
    fmt::print("part1: {}\n", s);
    do { increment(s); } while (!isvalid(s));
    fmt::print("part2: {}\n", s);
}
