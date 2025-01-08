#include <aoc.hpp>

namespace vs = std::views;
namespace rs = std::ranges;

namespace {
constexpr auto a { 16777216U };
constexpr auto b { 6U };
constexpr auto c { 5U };
constexpr auto d { 11U };

constexpr auto next(u32 n) -> u32 {
    auto const x = (n ^ (n << b)) % a;
    auto const y = (x ^ (x >> c)) % a;
    auto const z = (y ^ (y << d)) % a;
    return z;
}
} // namespace

template<>
auto advent2024::day22() -> result {
    auto const lines = aoc::util::readlines("./source/2024/22/input.txt");
    constexpr auto r{2000UL};
    aoc::random::sfc32 rng{};
    aoc::dense::map<u32, u32> sums;
    auto sum{0UL};
    for (auto n : lines | vs::transform(aoc::util::read<u32>)) {
        aoc::dense::set<u32> seen;
        auto h = rng();

        for (auto i = 0; i < r; ++i) {
            auto const m = next(n);
            auto const p = m % 10;
            auto const c = static_cast<u8>(p - n % 10);
            h = (h << 8U) + c;
            if (auto [it, ok] = seen.insert(h); ok) {
                sums[h] += p;
            }
            n = m;
        }
        sum += n;
    }

    auto const p1 = sum;
    auto const p2 = rs::max(sums.values() | vs::transform([](auto const p) { return p.second; }));
    return aoc::result(p1, p2);
}