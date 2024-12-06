#include <aoc.hpp>

namespace {
    using point = std::complex<i32>;
    using grid  = Eigen::Array<char, -1, -1, Eigen::RowMajor>;
    using map   = Eigen::Array<u8, -1, -1, Eigen::RowMajor>;
    enum direction : u8 { none = 0, left = 1, right = 2, up = 4, down = 8 };

    auto valid(auto const& m, auto p) {
        auto [re, im] = std::tuple{p.real(), p.imag()};
        return re >= 0 && re < m.rows() && im >= 0 && im < m.cols();
    };

    auto point_to_dir(point p) {
        if (p == point{0, +1}) { return direction::left; }
        if (p == point{0, -1}) { return direction::right; }
        if (p == point{+1, 0}) { return direction::down; }
        if (p == point{-1, 0}) { return direction::up; }
        throw std::runtime_error("point is not a direction");
    };
} // namespace

template<>
auto advent2024::day06() -> result {
    auto lines = aoc::util::readlines("./source/2024/06/input.txt");
    auto const nrow = std::ssize(lines);
    auto const ncol = std::ssize(lines.front());
    grid m(nrow, ncol);
    point start;
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            m(i, j) = lines[i][j];
            if (m(i, j) == '^') {
                start = {i, j};
                m(i, j) = '.';
            }
        }
    }

    map seen = map::Zero(nrow, ncol);
    auto predict_path = [&](auto const& grid, auto pos, auto dir) -> bool {
        seen(pos.real(), pos.imag()) |= direction::up;
        while(valid(grid, pos+dir)) {
            auto const p = pos+dir;
            auto [i, j] = std::tuple{p.real(), p.imag()};
            if (grid(i, j) == '#') {
                dir *= point{0, -1};
                continue;
            }
            auto d = point_to_dir(dir);
            if ((seen(i, j) & d) != 0) {
                return true;
            }
            seen(i, j) |= d;
            pos = p;
        }
        return false;
    };
    predict_path(m, start, point{-1, 0});
    auto const p1 = (seen > 0).count();
    map path = seen;
    auto count{0};
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            if (path(i, j) == 0) { continue; }
            seen.setConstant(0);
            m(i, j) = '#';
            count += predict_path(m, start, point{-1, 0});
            m(i, j) = '.';
        }
    }
    auto const p2 = count;
    return aoc::result(p1, p2);
}
