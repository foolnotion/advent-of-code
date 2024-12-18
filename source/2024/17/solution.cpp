#include <aoc.hpp>

namespace {
    auto pow2(u64 e) -> u64 {
        auto x = 1UL;
        for (auto i = 0; i < e; ++i) { x *= 2; }
        return x;
    };

    enum op : u8 { adv = 0, bxl, bst, jnz, bxc, out, bdv, cdv };

    struct computer {
        enum : u8 { a = 0, b, c };
        // registers
        std::array<u64, 3> r{};

        [[nodiscard]] auto combo(u64 b) const {
            if (b < 4) { return b; }
            if (b < 7) { return r[b-4]; }
            throw std::runtime_error(fmt::format("invalid value b = {}\n", b));
        }

        auto adv(u64 x) -> void {
            r[a] /= pow2(combo(x));
            ip += 2;
        }

        auto bxl(u64 x) -> void {
            r[b] ^= x;
            ip += 2;
        }

        auto bst(u64 x) -> void {
            r[b] = combo(x) % 8;
            ip += 2;
        }

        auto jnz(u64 x) -> void {
            ip = r[a] == 0 ? ip+2 : x;
        }

        auto bxc(u64) -> void {
            r[b] ^= r[c];
            ip += 2;
        }

        auto out(u64 x) -> void {
            buf.push_back(combo(x) % 8);
            ip += 2;
        }

        auto bdv(u64 x) -> void {
            r[b] = r[a] / pow2(combo(x));
            ip += 2;
        }

        auto cdv(u64 x) -> void {
            r[c] = r[a] / pow2(combo(x));
            ip += 2;
        }

        auto dispatch(u64 x, u64 y) {
            switch(x) {
                case op::adv: adv(y); return;
                case op::bxl: bxl(y); return;
                case op::bst: bst(y); return;
                case op::jnz: jnz(y); return;
                case op::bxc: bxc(y); return;
                case op::out: out(y); return;
                case op::bdv: bdv(y); return;
                case op::cdv: cdv(y); return;
                default: throw std::runtime_error(fmt::format("unknown opcode: {}", x));
            }
        }

        auto step(std::ranges::sized_range auto&& code) {
            auto x = code[ip];
            auto y = code[ip+1];
            dispatch(x, y);
        }

        auto run(std::ranges::sized_range auto&& code) {
            for(; ip < code.size(); ) {
                step(code);
            }
        }

        auto run_once(std::ranges::sized_range auto&& code) {
            for(; ip < code.size(); ) {
                step(code);
                if (ip == 0) { break; }
            }
        }

        std::vector<u64> buf;
        u64 ip{0};
    };
} // namespace

template<>
auto advent2024::day17() -> result {
    constexpr std::array code{2,4,1,1,7,5,4,6,1,4,0,3,5,5,3,0};
    constexpr auto a = 59397658UL;
    constexpr auto b = 8UL;
    computer comp{.r={a,0,0}};
    comp.run(code);

    auto f = [&](u64 v) {
        computer comp{.r={v,0,0}};
        comp.run_once(code);
        return comp.buf.back();
    };

    std::vector<u64> n{code.rbegin(), code.rend()};
    auto best = std::numeric_limits<u64>::max();

    std::function<void(u64, u64)> dfs = [&](u64 index, u64 number) -> void {
        if (number > best) { return; }
        if (index == n.size()) {
            best = std::min(best, number >> 3U);
            return;
        }
        for (auto i = 0UL; i < b; ++i) {
            if (f(number+i) != n[index]) { continue; }
            dfs(index+1, (number+i) << 3U);
        }
    };
    dfs(0L, 0UL);
    return aoc::result(comp.buf, best);
}