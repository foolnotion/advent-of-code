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
        static constexpr auto tgl { hash{}("tgl") };
        static constexpr auto out { hash{}("out") };
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
        u64 op{}; // opcode
        std::variant<char, i32> lhs;
        std::variant<char, i32> rhs;
        i32 n{};  // arity

        auto toggle() {
            switch(n) {
                case 1: {
                    // inc becomes dec, all other unary instructions become inc
                    op = op == opcode::inc ? opcode::dec : opcode::inc;
                    break;
                }
                case 2: {
                    // jnz becomes cpy, all other binary instructions become jnz
                    op = op == opcode::jnz ? opcode::cpy : opcode::jnz;
                    break;
                }
                default: {
                    break;
                }
            }
        }

        [[nodiscard]] inline auto valid() const {
            return !( // NOLINT
                (op == opcode::cpy && std::holds_alternative<i32>(rhs))
            );
        }
    };

    struct interpreter {
        explicit interpreter(std::vector<std::string> const& input) {
            code_.reserve(input.size());

            for (auto const& line : input) {
                auto tokens = lz::split(line, ' ').toVector();

                instruction i{};
                i.op = aoc::util::hash{}(tokens[0]);
                i.n  = static_cast<int>(tokens.size()) - 1;

                if (std::isalpha(tokens[1].front())) {
                    i.lhs = tokens[1].front();
                } else {
                    i.lhs = scn::scan_value<i32>(tokens[1])->value();
                }

                if (tokens.size() > 2) {
                    if(std::isalpha(tokens[2].front())) {
                        i.rhs = tokens[2].front();
                    } else {
                        i.rhs = scn::scan_value<i32>(tokens[2])->value();
                    }
                }
                code_.push_back(i);
            }
        }

        auto operator()(registers reg, bool detect_cycles = false) {
            get_value_visitor get{reg};

            auto value = [&](auto var) { return std::visit(get, var); };
            aoc::dense::set<u64> seen;

            auto hash = [](auto i, auto s, auto r) {
                return aoc::util::hash{}(i, s, r[0], r[1], r[2], r[3]);
            };

            for (auto i = 0; i < std::ssize(code_) && status; ++i) {
                if (detect_cycles) {
                    auto h = hash(i, signal_, reg);
                    if (auto [it, ok] = seen.insert(h); !ok) {
                        break;
                    }
                }

                if (!code_[i].valid()) { continue; }
                auto instr = code_[i];

                switch(instr.op) {
                case opcode::tgl: {
                    auto j = value(instr.lhs) + i;
                    if (j >= 0 && j < std::ssize(code_)) {
                        code_[j].toggle();
                    }
                    break;
                }
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
                    reg[c - 'a'] = value(instr.lhs);
                    break;
                }
                case opcode::jnz: {
                    if (value(instr.lhs) != 0) {
                        i += value(instr.rhs) - 1;
                    }
                    break;
                }
                case opcode::out: {
                    // transmits the next value for the clock signal
                    auto v = value(instr.lhs);
                    if ((signal_ == -1 && v == 1) || (signal_ == v)) {
                        status = false;
                    } else {
                        signal_ = v;
                    }
                    break;
                }
                }
            }
            return reg.front();
        }

        bool status{true};

    private:
        i32 signal_{-1};
        std::vector<instruction> code_;
    };
} // namespace aoc::interpreters::asmbunny

#endif