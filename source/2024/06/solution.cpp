#include <aoc.hpp>

namespace {
    using point = std::complex<i32>;
    using grid  = Eigen::Array<char, -1, -1, Eigen::RowMajor>;
    using map   = Eigen::Array<u8, -1, -1, Eigen::RowMajor>;
    enum direction : u8 { none = 0, left = 1, right = 2, up = 4, down = 8 };

    auto valid(auto const& m, auto p) {
        auto [i, j] = std::tuple{p.real(), p.imag()};
        return i >= 0 && i < m.rows() && j >= 0 && j < m.cols();
    };

    auto point_to_dir(point p) {
        if (p == point{0, +1}) { return direction::right; }
        if (p == point{0, -1}) { return direction::left; }
        if (p == point{+1, 0}) { return direction::down; }
        if (p == point{-1, 0}) { return direction::up; }
        throw std::runtime_error("point is not a direction");
    };

    auto simulate_walk(grid& m, point pos, point dir, bool can_add, map& seen, map& obstacles) {
        auto const d = point_to_dir(dir);
        if ((seen(pos.real(), pos.imag()) & d) != 0) {
            return 1;
        }
        seen(pos.real(), pos.imag()) |= d;
        auto const p = pos+dir;
        auto const [i, j] = std::tuple{p.real(), p.imag()};
        auto cycles = 0;

        if (valid(m, p)) {
            auto newdir = dir * point{0, -1};
            if (m(i, j) == '#') {
                cycles += simulate_walk(m, pos, newdir, can_add, seen, obstacles);
            } else {
                if (can_add && !std::exchange(obstacles(i, j), 1)) {
                    m(i, j) = '#';
                    map tmp = seen;
                    cycles += simulate_walk(m, pos, newdir, /*can_add=*/false, tmp, obstacles); // recurse with added obstacle
                    m(i, j) = '.';
                }
                cycles += simulate_walk(m, p, dir, can_add, seen, obstacles);
            }
        }
        return cycles;
    }
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
    map seen      = map::Zero(nrow, ncol);
    map obstacles = map::Zero(nrow, ncol);
    auto const p2 = simulate_walk(m, start, point{-1, 0}, /*can_add=*/true, seen, obstacles);
    auto const p1 = (seen > 0).count();
    return aoc::result(p1, p2);
}
