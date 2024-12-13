#include <aoc.hpp>
#include <Eigen/Dense>

namespace {
constexpr auto offset{10000000000000};

auto solve_p1(i64 x1, i64 y1, i64 x2, i64 y2, i64 px, i64 py) -> i64 {
    Eigen::Matrix2d A; A << x1, x2, y1, y2;
    Eigen::Vector2d b; b << px, py;
    Eigen::Vector2d v = A.colPivHouseholderQr().solve(b);
    Eigen::Matrix<i64, 2, 1> s = v.array().round().cast<i64>();
    if ((s(0) * x1 + s(1) * x2 == px) && (s(0) * y1 + s(1) * y2 == py)) {
        return 3 * s(0) + s(1);
    }
    return 0L;
}

auto solve_p2(i64 x1, i64 y1, i64 x2, i64 y2, i64 px, i64 py) -> i64 {
    return solve_p1(x1, y1, x2, y2, px+offset, py+offset);
}
}  // namespace

template<>
auto advent2024::day13() -> result {
    auto const lines = aoc::util::readlines("./source/2024/13/input.txt");
    std::array total{0UL, 0UL};
    for (auto i = 0L; i < std::ssize(lines); i += 4) {
        auto [x1, y1] = scn::scan<i64, i64>(lines[i], "Button A: X+{}, Y+{}")->values();
        auto [x2, y2] = scn::scan<i64, i64>(lines[i+1], "Button B: X+{}, Y+{}")->values();
        auto [px, py] = scn::scan<i64, i64>(lines[i+2], "Prize: X={}, Y={}")->values();
        total[0] += solve_p1(x1, y1, x2, y2, px, py);
        total[1] += solve_p2(x1, y1, x2, y2, px, py);
    }
    return aoc::result(total[0], total[1]);
}