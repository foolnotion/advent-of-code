#include <aoc.hpp>

namespace {
struct instr {
    static constexpr u64 ACC{aoc::util::hash{}("acc")}; // NOLINT
    static constexpr u64 JMP{aoc::util::hash{}("jmp")}; // NOLINT
    static constexpr u64 NOP{aoc::util::hash{}("nop")}; // NOLINT

    u64 op;
    i64 val;
};
} // namespace

template<>
auto advent2020::day08() -> result {
    auto input = aoc::util::readlines("./source/2020/08/input.txt");
    aoc::util::hash h;

    auto parse = [](auto const& s) {
        auto [name, value] = scn::scan<std::string, i64>(s, "{} {}")->values();
        return instr{aoc::util::hash{}(name), value};
    };
    auto code = lz::map(input, parse).toVector();

    auto run = [](auto const& code, u64 acc = 0) -> std::pair<i64, bool> {
        std::vector<bool> visited(code.size(), false);
        i64 i{};
        for (i = 0; i < std::ssize(code) && !visited[i]; ++i) {
            visited[i] = true;
            auto& [op, value] = code[i];
            switch(op) {
                case instr::JMP:
                    {
                        i += value-1;
                        break;
                    }
                case instr::ACC:
                    {
                        acc += value;
                        break;
                    }
                default:
                    {
                        // do nothing
                        break;
                    }
            }
        }
        return { acc, code.size() == i };
    };
    auto p1 = run(code).first;

    i64 p2{};
    for (auto& [op, val] : code) {
        if (op == instr::JMP || op == instr::NOP) {
            auto opp = op;
            op = op == instr::JMP ? instr::NOP : instr::JMP;
            if (auto [acc, ok] = run(code); ok) {
                p2 = acc;
                break;
            }
            op = opp;
        }
    }
    return aoc::result(p1, p2);
}
