#include <aoc.hpp>

namespace {
    using vector = Eigen::Array<i64, 2, 1>;
    struct point {
        vector position;
        vector velocity;
    };
} // namespace

template<>
auto advent2018::day10() -> result {
    auto const lines = aoc::util::readlines("source/2018/10/input.txt");
    std::vector<point> points;
    points.reserve(lines.size());

    for (auto const& line : lines) {
        if (auto res = scn::scan<i64, i64, i64, i64>(line, "position=<{},{}> velocity=<{},{}>"); res) {
            auto [x, y, vx, vy] = res->values();
            points.emplace_back(vector{x, y}, vector{vx, vy});
        }
    }

    constexpr auto msg_height = 10;
    auto seconds{0UL};
    for (; ++seconds; ) {
        vector pmin{std::numeric_limits<i64>::max(), std::numeric_limits<i64>::max()};
        vector pmax{std::numeric_limits<i64>::min(), std::numeric_limits<i64>::min()};
        for (auto& p : points) {
            p.position += p.velocity;
            pmin(0) = std::min(pmin(0), p.position(0));
            pmin(1) = std::min(pmin(1), p.position(1));
            pmax(0) = std::max(pmax(0), p.position(0));
            pmax(1) = std::max(pmax(1), p.position(1));
        }
        auto const cols = pmax(0) - pmin(0) + 1;
        auto const rows = pmax(1) - pmin(1) + 1;

        if (rows == msg_height) {
            Eigen::Array<char, -1, -1> m(rows, cols);
            m.setConstant(' ');

            for (auto const& p : points) {
                vector q = p.position - pmin;
                m(q(1), q(0)) = '#';
            }

            for (auto i = 0; i < m.rows(); ++i) {
                for (auto j = 0; j < m.cols(); ++j) {
                    fmt::print("{:c}", m(i ,j));
                }
                fmt::print("\n");
            }
            break;
        }
    }
    return aoc::result("", seconds);
}