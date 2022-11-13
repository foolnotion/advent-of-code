#include <aoc.hpp>

template<>
auto advent2020::day25() -> void {
    constexpr u64 a { 1526110 };
    constexpr u64 b { 20175123 };
    constexpr u64 m { 20201227 };
    u64 x{1};
    u64 p{0};
    while (x != a && ++p) {
        x = aoc::math::mul_mod(u64{7}, x, m); // NOLINT
    }
    fmt::print("{}\n", aoc::math::pow_mod(b, p, m));
}
