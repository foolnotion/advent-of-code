#include <aoc.hpp>

namespace {
    using point = aoc::point<i32, 2>;

    auto const parse = [](auto const& line) {
        auto [_, x, y] = scn::scan<i32, i32, i32>(line, "Disc #{} has {} positions; at time=0, it is at position {}.")->values();
        return point{x, y};
    };

    auto find_time(auto const& discs) {
        for (auto t = 0; ; ++t) {
            auto x = t;
            if (std::ranges::all_of(discs, [&](auto d) { return 0 == (++x + d[1]) % d[0]; })) {
                return t;
            }
        }
    }
} // namespace

template<>
auto advent2016::day15() -> result {
    auto discs = lz::map(aoc::util::readlines("./source/2016/15/input.txt"), parse).toVector();
    auto part1 = find_time(discs);
    discs.push_back({11, 0}); // NOLINT
    auto part2 = find_time(discs);
    return aoc::result(part1, part2);
}
