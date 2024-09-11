#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace {
struct computer {
    std::array<u32, 2> reg;

    enum instr {
        hlf = aoc::util::hash{}("hlf"),
        inc = aoc::util::hash{}("inc"),
        jie = aoc::util::hash{}("jie"),
        jio = aoc::util::hash{}("jio"),
        jmp = aoc::util::hash{}("jmp"),
        tpl = aoc::util::hash{}("tpl"),
    };

    auto run(std::vector<std::string>& instructions) {
        aoc::util::hash h;

        using std::string;

        for (auto i = 0; i < std::ssize(instructions); ++i) {
            auto& s = instructions[i];
            aoc::util::remove_all(s, ",");
            auto tokens = fplus::split(' ', false, s);
            auto a = tokens[0];
            auto b = tokens[1];
            auto c = tokens.size() > 2 ? tokens[2] : "";

            switch(h(a)) {
                case instr::jmp: // jump
                    {
                        i += scn::scan_value<i32>(b)->value()-1;
                        break;
                    }
                case instr::jio: // jump if one
                    {
                        auto r = reg[b[0]-'a'];
                        if (r == 1) { i += aoc::util::read<i32>(c)-1; }
                        break;
                    }
                case instr::jie: // jump if even
                    {
                        auto r = reg[b[0]-'a'];
                        if (r % 2 == 0) { i += aoc::util::read<i32>(c)-1; }
                        break;
                    }
                case instr::hlf: // half the register
                    {
                        reg[b[0]-'a'] /= 2;
                        break;
                    }
                case instr::tpl: // triple the register
                    {
                        reg[b[0]-'a'] *= 3;
                        break;
                    }
                case instr::inc: // increment the register
                    {
                        reg[b[0]-'a'] += 1;
                        break;
                    }
            }
        }
        return reg;
    }
};
} // namespace

template<>
auto advent2015::day23() -> result {
    auto input = aoc::util::readlines("./source/2015/23/input.txt");
    auto p1 = computer{{0,0}}.run(input)[1];
    auto p2 = computer{{1,0}}.run(input)[1];
    return aoc::result(p1, p2);
}
