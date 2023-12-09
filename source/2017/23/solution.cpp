#include <aoc.hpp>
#include <utility>
#include <variant>

namespace rs = std::ranges;

namespace {
    struct instruction {
        // opcodes
        static constexpr auto set = aoc::util::hash{}("set");
        static constexpr auto sub = aoc::util::hash{}("sub");
        static constexpr auto mul = aoc::util::hash{}("mul");
        static constexpr auto jnz = aoc::util::hash{}("jnz");

        u64 opcode{};
        std::variant<char, i64> a;
        std::variant<char, i64> b;
    };

    struct program {
        static constexpr auto regsize{8};

        std::array<i64, regsize> reg = {0};
        std::vector<instruction> code;

        explicit program(std::vector<instruction> c) : code{std::move(c)} { }

        auto operator()(char c) const -> i64 {
            return reg[c-'a'];
        }

        auto operator()(i64 v) const -> i64 {
            return v;
        }

        template<typename OP>
        auto apply_binary(instruction const& in) {
            auto c = std::get<char>(in.a);
            auto v = std::visit(*this, in.b);
            reg[c-'a'] = OP{}(reg[c-'a'], v);
        }

        auto operator()() -> i64 {
            i64 mulcnt{0};
            for (auto i = 0L; i < std::ssize(code); ++i) {
                auto const& in = code[i];

                switch(in.opcode) {
                    case instruction::set: {
                        auto c = std::get<char>(in.a);
                        auto v = std::visit(*this, in.b);
                        reg[c-'a'] = v;
                        break;
                    }
                    case instruction::sub: {
                        apply_binary<std::minus<>>(in);
                        break;
                    }
                    case instruction::mul: {
                        ++mulcnt;
                        apply_binary<std::multiplies<>>(in);
                        break;
                    }
                    case instruction::jnz: {
                        auto x = std::visit(*this, in.a);
                        auto y = std::visit(*this, in.b);
                        if (x != 0) { i += y-1; }
                        break;
                    }
                }
            }
            return mulcnt;
        }

        // part 2 function obtained by decoding/translating the assembly instructions
        static auto optimized() {
            constexpr auto a{108400L};
            constexpr auto b{125400L};
            constexpr auto c{17L};

            auto not_prime = [](auto p) -> bool {
                auto n = static_cast<i64>(std::sqrt(p));
                return rs::any_of(lz::range(2L, n), [p](auto q) { return p % q == 0; });
            };

            auto const r = lz::range(a, b+1, c);
            return std::transform_reduce(r.begin(), r.end(), 0UL, std::plus{}, not_prime);
        }
    };

    auto parse(auto const& input) {
        std::vector<instruction> instructions;
        instructions.reserve(input.size());

        auto fill_value = [](std::variant<char, i64>& var, std::string const& str) {
            if (std::isalpha(str.front())) {
                var = scn::scan_value<char>(str).value();
            } else {
                var = scn::scan_value<i64>(str).value();
            }
        };

        for (auto const& s : input) {
            auto vec = lz::map(lz::split(s, ' '), [](auto x) { return std::string(x.begin(), x.end()); }).toVector();
            instruction in;
            in.opcode = aoc::util::hash{}(vec.front());
            fill_value(in.a, vec[1]);
            fill_value(in.b, vec[2]);
            instructions.push_back(in);
        }
        return instructions;
    }
} // namespace

template<>
auto advent2017::day23() -> result {
    auto input = aoc::util::readlines("./source/2017/23/input.txt");
    auto instructions = parse(input);
    program program1(instructions);
    auto const p1 = program1();
    auto const p2 = program::optimized();
    return aoc::result(p1, p2);
}
