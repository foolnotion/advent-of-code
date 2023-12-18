#include <aoc.hpp>
#include <utility>

namespace aoc::interpreters::intcode {
enum opcode : u16 {
    add = 1U,
    mul = 2U,
    in  = 3U,
    out = 4U,
    jit = 5U,
    jif = 6U,
    lt  = 7U,
    eq  = 8U,
    end = 99U
};

constexpr auto base{10};

template<std::size_t N>
auto extract_modes(i64 v) {
    std::array<u8, N> modes = {0};
    for (auto i = 0L, x = v % base; v > 0; v /= base, x = v % base, i += 1) {
        modes[i] = x;
    }
    return modes;
}

struct interpreter {
    std::vector<i64> code;

    explicit interpreter(std::vector<i64> ints) : code{std::move(ints)} {}

    auto operator[](i64 i) -> i64& { return code[i]; }
    auto operator[](i64 i) const -> i64 { return code[i]; }

    std::optional<i64> input;
    std::optional<i64> output;

    auto operator()() {
        auto x{0UL};
        auto p{0L};

        std::span s{code.begin(), code.end()};

        auto read = [&](auto v, auto m) {
            return m == 0 ? s[v] : v;
        };

        auto apply_binary = [&](std::span<i64> s, std::span<i64 const> q, i64 v, auto&& op) {
            auto m = extract_modes<2>(v);
            auto a = read(q[0], m[0]);
            auto b = read(q[1], m[1]);
            auto c = q[2];
            s[c] = op(a, b);
        };

        auto apply_jump = [&](std::span<i64 const> q, i64 v, i64 p, auto&& cmp) {
            auto m = extract_modes<2>(v);
            auto a = read(q[0], m[0]);
            auto b = read(q[1], m[1]);
            return cmp(a, 0) ? b : p + 3;
        };

        while (x != opcode::end) {
            auto q = s.subspan(p, s.size()-p);
            auto v = q.front();
            q = q.subspan(1, q.size()-1);

            auto op = static_cast<opcode>(v % 100); // NOLINT
            if (op == opcode::end) { return; }
            v /= 100; // NOLINT

            switch(op) {
                case opcode::add: {
                    apply_binary(s, q, v, std::plus{});
                    p += 4;
                    break;
                }
                case opcode::mul: {
                    apply_binary(s, q, v, std::multiplies{});
                    p += 4;
                    break;
                }
                case opcode::lt: {
                    apply_binary(s, q, v, std::less{});
                    p += 4;
                    break;

                }
                case opcode::eq: {
                    apply_binary(s, q, v, std::equal_to{});
                    p += 4;
                    break;
                }
                case opcode::in: {
                    ENSURE(input);
                    auto a = q[0];
                    s[a] = input.value();
                    p += 2;
                    break;
                }
                case opcode::out: {
                    auto m = extract_modes<1>(v);
                    output = read(q[0], m[0]);
                    p += 2;
                    break;
                }
                case opcode::jit: {
                    p = apply_jump(q, v, p, std::not_equal_to{});
                    break;
                }
                case opcode::jif: {
                    p = apply_jump(q, v, p, std::equal_to{});
                    break;
                }
                default: {
                    return;
                }
            }
        }
    };

    static auto run(std::vector<i64> ints) {
        interpreter t{std::move(ints)};
        t();
        return t[0];
    }
};

} // namespace aoc::interpreters::intcode