#include <aoc.hpp>
#include <bits/ranges_util.h>

template<>
auto advent2017::day13() -> result {
    using layer = std::pair<i32, i32>;
    auto input = aoc::util::readlines("./source/2017/13/input.txt");
    std::vector<layer> layers;
    layers.reserve(input.size());
    for (auto const& s : input) {
        auto [first, second] = scn::scan<i32, i32>(s, "{}: {}")->values();
        layers.emplace_back(first, second);
    }
    auto scanner_top = [](layer l) {
        return l.first % (2 * (l.second - 1)) == 0;
    };

    // part 1 - compute severity
    auto const p1 = std::transform_reduce(layers.begin(), layers.end(), 0, std::plus{}, [&](auto l) {
        return scanner_top(l) ? l.first * l.second : 0;
    });

    // part 2 - compute minimum delay
    auto const p2 = *std::ranges::find_if(std::views::iota(1), [&](auto i) {
        return std::ranges::none_of(layers, [&](auto l) { return scanner_top({l.first+i, l.second}); });
    });
    return aoc::result(p1, p2);
}