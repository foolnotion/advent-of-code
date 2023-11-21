#ifndef ADVENT_OF_CODE_ASMBUNNY_HPP
#define ADVENT_OF_CODE_ASMBUNNY_HPP

#include <Lz/StringSplitter.hpp>
#include <aoc.hpp>
#include <variant>

namespace aoc::interpreters::asmbunny {
    struct opcode {
        using hash = aoc::util::hash;
        static constexpr auto cpy { hash{}("cpy") };
        static constexpr auto inc { hash{}("inc") };
        static constexpr auto dec { hash{}("dec") };
        static constexpr auto jnz { hash{}("jnz") };
    };

    using registers = std::array<i32, 4>;

    struct get_value_visitor {
        explicit get_value_visitor(registers const& reg)
            : reg_{reg.data(), reg.size()}
        { }

        auto operator()(i32 val) const -> i32 { return val; }
        auto operator()(char c) const -> i32 { return reg_[c - 'a']; }

        private:
        std::span<i32 const> reg_;
    };

    struct instruction {
        u64 op; // opcode
        std::variant<char, i32> lhs;
        std::variant<char, i32> rhs;

        [[nodiscard]] auto name() const -> std::string {
            aoc::util::hash hash;
            switch(op) {
                case hash("cpy"): return "cpy";
                case hash("inc"): return "inc";
                case hash("dec"): return "dec";
                case hash("jnz"): return "jnz";
                default: return "unknown";
            }
        }
    };

    struct interpreter {
        explicit interpreter(std::vector<std::string> const& input) {
            code_.reserve(input.size());

            for (auto const& line : input) {
                auto tokens = lz::split(line, ' ').toVector();

                instruction i{};
                i.op = aoc::util::hash{}(tokens[0]);

                if (std::isalpha(tokens[1].front())) {
                    i.lhs = tokens[1].front();
                } else {
                    i.lhs = scn::scan_value<i32>(tokens[1]).value();
                }

                if (tokens.size() > 2 && std::isalpha(tokens[2].front())) {
                    i.rhs = tokens[2].front();
                } else {
                    i.rhs = scn::scan_value<i32>(tokens[2]).value();
                }
                code_.push_back(i);
            }
        }

        auto operator()(registers reg) {
            aoc::util::hash h;
            get_value_visitor get{reg};

            for (auto i = 0; i < std::ssize(code_); ++i) {
                auto instr = code_[i];

                switch(instr.op) {
                case opcode::inc: {
                    auto c = std::get<char>(instr.lhs);
                    ++reg[c - 'a'];
                    break;
                }
                case opcode::dec: {
                    auto c = std::get<char>(instr.lhs);
                    --reg[c - 'a'];
                    break;
                }
                case opcode::cpy: {
                    auto c = std::get<char>(instr.rhs);
                    auto v = std::visit(get, instr.lhs);
                    reg[c - 'a'] = v;
                    break;
                }
                case opcode::jnz: {
                    auto v = std::visit(get, instr.lhs);
                    if (v != 0) {
                        i += std::get<i32>(instr.rhs) - 1;
                    }
                    break;
                }
                }
            }
            return reg.front();
        }
    
    private:
        std::vector<instruction> code_;
    };
} // namespace aoc::interpreters::asmbunny

#endif