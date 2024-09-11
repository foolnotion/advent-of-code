#include <aoc.hpp>

template<>
auto advent2017::day08() -> result {
    auto input = aoc::util::readlines("./source/2017/08/input.txt");
    aoc::util::hash hash;

    // conditions
    auto constexpr lt = hash("<");
    auto constexpr le = hash("<=");
    auto constexpr gt = hash(">");
    auto constexpr ge = hash(">=");
    auto constexpr eq = hash("==");
    auto constexpr ne = hash("!=");

    auto test = [=](i32 value, i32 test, std::string const& condition) {
        auto h = aoc::util::hash{}(condition);
        switch(h) {
            case lt: return value < test;
            case le: return value <= test;
            case gt: return value > test;
            case ge: return value >= test;
            case eq: return value == test;
            case ne: return value != test;
            default: throw std::runtime_error("unknown test condition");
        }
        return false;
    };

    auto part2{0};
    aoc::dense::map<std::string, i32> registers;

    using std::string;
    for (auto const& line : input) {
        // std::string target_register;
        // std::string test_register;
        // std::string test_condition;
        // std::string opcode;
        // i32 opcode_arg{};
        // i32 test_arg{};

        auto [target_register,
              opcode,
              opcode_arg,
              test_register,
              test_condition,
              test_arg] = scn::scan<string, string, i32, string, string, i32>(line, "{} {} {} if {} {} {}")->values();

        auto test_value = registers.insert({test_register, 0}).first->second;
        if (test(test_value, test_arg, test_condition)) {
            auto& target_value = registers.insert({target_register, 0}).first->second;

            if (opcode == "inc") {
                target_value += opcode_arg;
            } else {
                target_value -= opcode_arg;
            }
            part2 = std::max(part2, target_value);
        }
    }
    auto part1{0};
    for (auto [k, v] : registers) {
        part1 = std::max(part1, v);
    }

    return aoc::result(part1, part2);
}