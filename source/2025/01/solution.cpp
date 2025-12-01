#include <aoc.hpp>

namespace {
    constexpr auto N = 100;
    constexpr auto D = 50;

    auto mod(auto x, auto y) {
        return x < 0 ? x % y + y : x % y;
    }

    auto apply_rotation(u8 n, i32 amount) -> std::tuple<u8, i32, i32> {
        auto s = aoc::util::sgn(amount);
        auto a = std::abs(amount);

        if (a > N) {
            auto [m, z1, z2] = apply_rotation(n, s*(a % N));
            return {m, z1, z2 + a / N};
        }

        auto m = mod(n+amount, N);
        auto p1 = m == 0;
        auto p2 = n != 0 && (m == 0 || m*s < n*s);
        return {m, p1, p2};
    }
} // namespace

template<>
auto advent2025::day01() -> result
{
    auto const lines = aoc::util::readlines("source/2025/01/input.txt");

    i8 n = D;
    i32 p1 = 0;
    i32 p2 = 0;
    for (auto const& line : lines) {
        auto amount = scn::scan<i32>(std::string_view{line.begin()+1, line.end()}, "{}")->value();
        if (line[0] == 'L') {
            amount = -amount;
        }
        auto [m, z1, z2] = apply_rotation(n, amount);
        n = m;
        p1 += z1;
        p2 += z2;
    }

    return aoc::result(p1, p2);
}
