#include <aoc.hpp>
#include <2016/asmbunny/asmbunny.hpp>

template <>
auto advent2016::day12() -> result
{
    auto input = aoc::util::readlines("./source/2016/12/input.txt");
    aoc::interpreters::asmbunny::interpreter interpreter{input};
    using aoc::interpreters::asmbunny::registers;

    auto part1 = interpreter(registers{0,0,0,0});
    auto part2 = interpreter(registers{0,0,1,0});
    return aoc::result(part1, part2);
}
