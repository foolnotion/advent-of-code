#include <aoc.hpp>
#include <2016/asmbunny/asmbunny.hpp>

template<>
auto advent2016::day25() -> result {
    auto input = aoc::util::readlines("./source/2016/25/input.txt");

    using aoc::interpreters::asmbunny::interpreter;
    using aoc::interpreters::asmbunny::registers;

    auto part1{-1};
    for (auto i = 0; i < 10000; ++i) {
        registers r{i, 0, 0, 0};
        interpreter interpreter{input};
        interpreter(r);
        if (interpreter.status) {
            part1 = i;
            break;
        }
    }

    return aoc::result(part1, "");
}
