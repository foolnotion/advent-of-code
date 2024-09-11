#include <aoc.hpp>
#include <2019/intcode/intcode.hpp>
#include <fplus/fplus.hpp>


template<>
auto advent2019::day02() -> result {
    auto input = aoc::util::readlines("./source/2019/02/input.txt");
    auto read_value = aoc::util::read<i64>;
    auto intcode = lz::map(lz::split(input.front(), ','), read_value).toVector();

    intcode[1] = 12;
    intcode[2] = 2;

    using aoc::interpreters::intcode::interpreter;
    auto const p1 = interpreter::run(intcode);

    auto find = [](auto const& intcode) {
        constexpr auto target_output{19690720};
        constexpr auto n{100};
        for (auto i = 0; i < n; ++i) {
            for (auto j = 0; j < n; ++j) {
                auto code = intcode;
                code[1] = i;
                code[2] = j;
                if (interpreter::run(code) == target_output) {
                    return i * n + j;;
                }
            }
        }
        return 0;
    };
    auto const p2 = find(intcode);

    return aoc::result(p1, p2);
}