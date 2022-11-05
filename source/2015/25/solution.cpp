#include <aoc.hpp>

template<>
auto advent2015::day25() -> void {
    // puzzle input
    constexpr auto row{2981};
    constexpr auto col{3075};
    constexpr auto multiplier{252533UL};
    constexpr auto divisor{33554393UL};
    constexpr auto first{20151125UL};

    // worked out the indexing using pen & paper:
    auto index = [](u32 a, u32 b) -> u64 { return b + (a+b-1) * (a+b-2) / 2; };
    fmt::print("part 1: {}\n", first * aoc::math::pow_mod(multiplier, index(row,col)-1, divisor) % divisor);
}
