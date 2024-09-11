#include <aoc.hpp>

namespace {
    using point = aoc::point<i32, 2>;

    auto const parse = [](auto const& line) -> std::pair<point, point> {
        auto [x1, y1, x2, y2] = scn::scan<i32, i32, i32, i32>(line, "{}-{},{}-{}")->values();
        point a{x1, y1};
        point b{x2, y2};
        return {a,b};
    };

    template<bool B = true>
    auto contains(std::pair<point, point> p) -> bool {
        auto [a, b] = p;
        return (a[0] <= b[0] && a[1] >= b[1]) || (B && contains<!B>({b, a}));
    };

    template<bool B = true>
    auto overlaps(std::pair<point, point> p) -> bool {
        auto [a, b] = p;
        return (a[0] <= b[0] && a[1] >= b[0]) || (B && overlaps<!B>({b, a}));
    };
} // namespace

template<>
auto advent2022::day04() -> result {
    auto pairs = lz::map(aoc::util::readlines("./source/2022/04/input.txt"), parse).toVector();
    auto part1 = std::transform_reduce(pairs.begin(), pairs.end(), u64{0}, std::plus{}, contains<true>);
    auto part2 = std::transform_reduce(pairs.begin(), pairs.end(), u64{0}, std::plus{}, overlaps<true>);
    return aoc::result(part1, part2);
}
