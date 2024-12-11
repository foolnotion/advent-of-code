#include <aoc.hpp>

namespace {
    auto count_digits(auto const x) {
        auto n = 0;
        auto d = 1;
        for (auto v = x; v != 0; v /= 10) { ++n; }
        for (auto i = 0; i < n/2; ++i) { d *= 10; }
        return std::tuple{n, d};
    };

    auto expand(auto const x) -> std::vector<u64> {
        constexpr auto c{2024};
        if (x == 0) {
            return {1};
        }
        auto [n, d] = count_digits(x);
        if (n % 2 == 0) {
            return {x/d, x%d};
        }
        return {x * c};
    };
} // namespace

template<>
auto advent2024::day11() -> result {
    auto const input = aoc::util::readlines("./source/2024/11/input.txt").front();
    auto sv = [](auto x) { return std::string_view{x.begin(), x.end()}; };
    auto view = std::views::split(input, ' ') | std::views::transform(sv) | std::views::transform(aoc::util::read<u64>);
    std::vector<u64> stones{view.begin(), view.end()};
    constexpr auto blinks = std::array{25, 75};
    aoc::dense::map<std::tuple<u64, u64>, u64> len;
    std::function<u64(u64, i32)> length = [&](u64 x, i32 d) -> u64 {
        if (d == 0) { return 1; }
        if (auto it = len.find({x, d}); it != len.end()) {
            return it->second;
        }
        auto values = expand(x);
        auto sum = std::transform_reduce(values.begin(), values.end(), 0UL, std::plus{}, [&](auto const x){
            return length(x, d-1);
        });
        return len.insert({{x,d}, sum}).first->second;
    };
    auto const p1 = std::transform_reduce(stones.begin(), stones.end(), 0UL, std::plus{}, [&](auto x){ return length(x, blinks[0]); });
    auto const p2 = std::transform_reduce(stones.begin(), stones.end(), 0UL, std::plus{}, [&](auto x){ return length(x, blinks[1]); });
    return aoc::result(p1, p2);
}