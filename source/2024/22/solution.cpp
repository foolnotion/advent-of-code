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
    constexpr auto rounds{2000UL};
    aoc::random::sfc32 rng{};
    auto sum{0UL};

    constexpr auto r{19}; // price change range from -9 to +9
    constexpr auto s{r*r*r*r}; // total size of the cache array

    using extents = std::extents<i32, r, r, r, r>;
    using mdarray = std::experimental::mdarray<u32, extents, std::layout_right, std::array<u32, s>>;

    mdarray sums(r, r, r, r);
    mdarray seen(r, r, r, r);

    for (auto n : lines | vs::transform(aoc::util::read<u32>)) {
        rs::fill(seen.container(), 1);
        std::array<u32, 4> changes{0,0,0,0};

        for (auto i = 0; i < rounds; ++i) {
            auto const m = next(n);
            auto const p = m % 10;
            auto const c = p + 9U - n % 10;
            rs::rotate(changes, changes.begin()+1);
            changes.back() = c;
            auto [x, y, z, w] = changes;
            sums(x, y, z, w) += p * std::exchange(seen(x, y, z, w), 0);
            n = m;
        }
        sum += n;
    }
    auto const p1 = sum;
    auto const p2 = rs::max(sums.container());
    return aoc::result(p1, p2);
}