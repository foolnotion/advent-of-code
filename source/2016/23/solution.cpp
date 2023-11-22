#include <aoc.hpp>
#include <2016/asmbunny/asmbunny.hpp>

template<>
auto advent2016::day23() -> result {
    std::vector<std::string> example = {
        "cpy 2 a",
        "tgl a",
        "tgl a",
        "tgl a",
        "cpy 1 a",
        "dec a",
        "dec a"
    };

    std::vector<std::string> input {
        "cpy a b",
        "dec b",
        "cpy a d",
        "cpy 0 a",
        "cpy b c",
        "inc a",
        "dec c",
        "jnz c -2",
        "dec d",
        "jnz d -5",
        "dec b",
        "cpy b c",
        "cpy c d",
        "dec d",
        "inc c",
        "jnz d -2",
        "tgl c",
        "cpy -16 c",
        "jnz 1 c",
        "cpy 95 c",
        "jnz 99 d",
        "inc a",
        "inc d",
        "jnz d -2",
        "inc c",
        "jnz c -5"
    };

    using aoc::interpreters::asmbunny::registers;

    aoc::interpreters::asmbunny::interpreter interpreter{input};
    auto part1 = interpreter(registers{7,0,0,0});

    interpreter = decltype(interpreter){input};
    auto part2 = interpreter(registers{12,0,0,0});

    return aoc::result(part1, part2);
}
