#include <aoc.hpp>

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

        for (auto i = 0; i < std::ssize(instructions); ++i) {
            auto& s = instructions[i];
            aoc::util::remove_all(s, ",");
            std::string a;
            std::string b;
            std::string c;
            (void)scn::scan(s, "{} {} {}", a, b, c);

            switch(h(a)) {
                case instr::jmp: // jump
                    {
                        i += scn::scan_value<i32>(b).value()-1;
                        break;
                    }
                case instr::jio: // jump if one
                    {
                        auto r = reg[b[0]-'a'];
                        if (r == 1) { i += scn::scan_value<i32>(c).value()-1; }
                        break;
                    }
                case instr::jie: // jump if even
                    {
                        auto r = reg[b[0]-'a'];
                        if (r % 2 == 0) { i += scn::scan_value<i32>(c).value()-1; }
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


template<>
auto advent2015::day23() -> void {
    auto input = aoc::util::readlines("./source/2015/23/input.txt");
    fmt::print("part 1: {}\n", computer{{0,0}}.run(input)[1]);
    fmt::print("part 2: {}\n", computer{{1,0}}.run(input)[1]);
}
