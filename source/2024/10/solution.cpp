#include <aoc.hpp>

template<>
auto advent2024::day10() -> result {
    using point = Eigen::Array2i;

    auto const lines = aoc::util::readlines("./source/2024/10/input.txt");
    auto [nrow, ncol] = std::tuple{std::ssize(lines), std::ssize(lines.front())};

    Eigen::Array<u8, -1, -1, Eigen::RowMajor> grid(nrow, ncol);

    std::vector<point> start;
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            auto const c = lines[i][j];
            grid(i, j) = c-'0';
            if (c == '0') { start.emplace_back(i, j); }
        }
    }
    std::array const directions { point{-1, 0}, point{+1, 0}, point{0, -1}, point{0, +1} };

    auto valid = [&](point p) {
        return p(0) >= 0 && p(1) < nrow && p(1) >= 0 && p(1) < ncol;
    };

    auto count_trailheads = [&](point start, bool distinct = false) {
        std::queue<point> queue;
        queue.emplace(start);
        auto c{0L};
        aoc::dense::set<std::tuple<i32, i32>> seen;
        while(!queue.empty()) {
            auto const p = queue.front(); queue.pop();
            if (!distinct) {
                if (auto [it, ok] = seen.insert({p(0), p(1)}); !ok) {
                    continue;
                }
            }
            if (grid(p(0), p(1)) == 9) { ++c; continue; }
            for (auto const& d : directions) {
                auto const q = p+d;
                if (valid(q) && grid(q(0), q(1)) - grid(p(0), p(1)) == 1) {
                    queue.emplace(q);
                }
            }
        }
        return c;
    };
    auto const p1 = std::transform_reduce(start.begin(), start.end(), 0L, std::plus{}, [&](auto const p){ return count_trailheads(p, false); });
    auto const p2 = std::transform_reduce(start.begin(), start.end(), 0L, std::plus{}, [&](auto const p){ return count_trailheads(p, true); });
    return aoc::result(p1, p2);
}