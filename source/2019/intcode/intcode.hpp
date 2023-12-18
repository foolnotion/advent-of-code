#include <aoc.hpp>
#include <utility>

namespace aoc::interpreters::intcode {
enum opcode : u16 {
    add = 1U,
    mul = 2U,
    end = 99U
};

struct interpreter {
    std::vector<i64> code;

    explicit interpreter(std::vector<i64> ints) : code{std::move(ints)} {}

    auto operator[](i64 i) -> i64& { return code[i]; }
    auto operator[](i64 i) const -> i64 { return code[i]; }

    auto operator()() {
        auto x{0UL};
        auto p{0L};

        std::span s{code.begin(), code.end()};

        while (x != opcode::end) {
            auto q = s.subspan(p, s.size()-p);
            auto op = static_cast<opcode>(q.front());
            if (op == opcode::end) { return; }
            auto [a, b, c] = std::tuple{q[1], q[2], q[3]};

            switch(op) {
                case opcode::add: {
                    s[c] = s[a] + s[b];
                    break;
                }
                case opcode::mul: {
                    s[c] = s[a] * s[b];
                    break;
                }
                default: {
                    return;
                }
            }
            p += 4;
        }
    };

    static auto run(std::vector<i64> ints) {
        interpreter t{std::move(ints)};
        t();
        return t[0];
    }
};

} // namespace aoc::interpreters::intcode