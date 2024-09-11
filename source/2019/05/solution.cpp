#include <aoc.hpp>
#include <2019/intcode/intcode.hpp>

template<>
auto advent2019::day05() -> result {
    auto input = aoc::util::readlines("./source/2019/05/input.txt");
    auto read_value = aoc::util::read<i64>;
    auto intcode = lz::map(lz::split(input.front(), ','), read_value).toVector();

    using aoc::interpreters::intcode::interpreter;

    auto run_diagnostic = [](auto intcode, auto in) {
        interpreter t(intcode);
        t.input = in;
        t();
        return t.output.value();
    };

    auto const p1 = run_diagnostic(intcode, 1);
    auto const p2 = run_diagnostic(intcode, 5);
    return aoc::result(p1, p2);
}