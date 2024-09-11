#include <aoc.hpp>

template<>
auto advent2023::day09() -> result {
    auto input = aoc::util::readlines("./source/2023/09/input.txt");
    auto seq = lz::map(input, [](auto const& line){
        return lz::map(lz::split(line, ' '), aoc::util::read<i64>).toVector();
    }).toVector();

    auto extrapolate = [](auto s, bool reverse = false) {
        i64 c{1};
        i64 v{0};
        if (reverse) { std::ranges::reverse(s); }
        while(std::ranges::any_of(std::span{s.begin(), s.end()-c}, std::identity{})) {
            std::adjacent_difference(s.begin()+1, s.end()-c+1, s.begin());
            v += *(s.end()-c);
            c += 1;
        }
        return v;
    };

    auto fwd = [&](auto const& q) { return extrapolate(q, false); };
    auto rev = [&](auto const& q) { return extrapolate(q, true); };
    auto const p1 = std::transform_reduce(seq.begin(), seq.end(), 0L, std::plus{}, fwd);
    auto const p2 = std::transform_reduce(seq.begin(), seq.end(), 0L, std::plus{}, rev);
    return aoc::result(p1, p2);
}