#include <aoc.hpp>

template<>
auto advent2018::day16() -> result {
    auto lines = aoc::util::readlines("source/2168/16/input.txt");
    auto values = lines | std::views::transform(aoc::util::read<i32>);

    auto const p1 = std::reduce(values.begin(), values.end());

    return aoc::result(p1, "");
}