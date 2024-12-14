#include <aoc.hpp>

template<>
auto advent2024::day14() -> result {
    auto const lines = aoc::util::readlines("./source/2024/14/input.txt");
    constexpr auto width{101};
    constexpr auto height{103};

    std::vector<std::tuple<i64, i64, i64, i64>> robots;
    for (auto const& line : lines) {
        auto r = scn::scan<i64, i64, i64, i64>(line, "p={},{} v={},{}")->values();
        robots.push_back(std::move(r));
    }
    constexpr auto seconds{100};
    auto safety_factor = [](auto const& robots, i64 seconds) {
        aoc::point<i64> mid{width/2, height/2};
        aoc::point<u64, 4> quadrants = {0};
        for (auto const& [px, py, vx, vy] : robots) {
            auto x = (width  + (px + vx * seconds) % width)  % width;
            auto y = (height + (py + vy * seconds) % height) % height;
            quadrants[0] += x < mid[0] && y < mid[1];
            quadrants[1] += x < mid[0] && y > mid[1];
            quadrants[2] += x > mid[0] && y < mid[1];
            quadrants[3] += x > mid[0] && y > mid[1];
        }
        return std::reduce(quadrants.begin(), quadrants.end(), 1UL, std::multiplies{});
    };

    std::array<i32, width> xs{};
    std::array<i32, height> ys{};
    auto find_easter_egg = [&xs,&ys](auto const& robots, i32 max_seconds) -> std::optional<i32> {
        constexpr auto min_collinear{20};
        // note: this will be wrong if the tree appears more than once in this time interval
        //       (it's safer to start from s=1 but that will be much slower)
        for (auto s = max_seconds; s > 0; --s) {
            std::ranges::fill(xs, 0);
            std::ranges::fill(ys, 0);
            for (auto const& [px, py, vx, vy] : robots) {
                ++xs[ (width  + (px + vx * s) % width)  % width  ];
                ++ys[ (height + (py + vy * s) % height) % height ];
            }
            if (std::ranges::max(xs) >= min_collinear && std::ranges::max(ys) >= min_collinear) {
                return s;
            }
        }
        return std::nullopt;
    };
    auto const p1 = safety_factor(robots, seconds);
    auto const p2 = find_easter_egg(robots, 10'000L).value_or(0);
    return aoc::result(p1, p2);
}