#include <aoc.hpp>

template<>
auto advent2023::day06() -> result {
    std::vector<std::pair<double, double>> input = {
        {56, 546}, {97, 1927}, {78, 1131}, {75, 1139}
    };

    auto limits = [](std::pair<double, double> p) {
        auto [t, d] = p;
        auto w0 = (t - std::sqrt(t*t - 4*d)) / 2;
        auto w1 = (t + std::sqrt(t*t - 4*d)) / 2;
        auto a = std::ceil(w0);
        auto b = std::floor(w1);
        a += (a == w0);
        b -= (b == w1);
        return b-a+1;
    };

    auto const p1 = std::transform_reduce(input.begin(), input.end(), 1L, std::multiplies{}, limits);
    auto const p2 = limits({56977875, 546192711311139});
    return aoc::result(p1, p2);
}