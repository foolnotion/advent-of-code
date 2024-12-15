#include <aoc.hpp>

namespace {
    using point = Eigen::Vector2i;
    using grid  = Eigen::Array<char, -1, -1>;

    auto parse(std::string const& path) {
        auto const lines = aoc::util::readlines(path);
        auto empty_line  = std::ranges::find_if(lines, &std::string::empty);

        auto const nrow = std::distance(lines.begin(), empty_line);
        auto const ncol = std::ssize(lines.front());
        grid g1(nrow, ncol);
        grid g2(nrow, ncol*2);
        std::vector<point> moves;
        point r1;
        point r2;

        // parse
        for (auto i = 0L; i < nrow; ++i) {
            for (auto j = 0L; j < ncol; ++j) {
                auto k = 2*j;
                auto const c = lines[i][j];
                if (c == '@') {
                    g1(i, j) = g2(i, k) = g2(i, k+1) = '.';
                    r1 = point(i, j);
                    r2 = point(i, k);
                } else if (c == 'O') {
                    g1(i, j) = c;
                    g2(i, k) = '[';
                    g2(i, k+1) = ']';
                } else {
                    g1(i, j) = g2(i, k) = g2(i, k+1) = c;
                }
            }
        }
        for (auto i = nrow+1; i < std::ssize(lines); ++i) {
            for (auto const c : lines[i]) {
                if (c == '<') { moves.emplace_back(0, -1); }
                if (c == '>') { moves.emplace_back(0, +1); }
                if (c == '^') { moves.emplace_back(-1, 0); }
                if (c == 'v') { moves.emplace_back(+1, 0); }
            }
        }
        return std::tuple{std::move(g1), std::move(g2), std::move(r1), std::move(r2), std::move(moves)};
    }

    auto sum(auto const& grid, char c) {
        auto sum{0UL};
        for (auto i = 0; i < grid.rows(); ++i) {
            for (auto j = 0; j < grid.cols(); ++j) {
                if (grid(i, j) == c) {
                    sum += 100 * i + j;
                }
            }
        }
        return sum;
    }

    auto horizontal_push(auto& grid, point& robot, point const& m, point const& q) {
        auto [right, left] = std::tuple{ point(0, +1), point(0, -1) };
        auto row = grid.row(q(0));
        if (m == right) {
            for (auto i = q(1); i > robot(1); --i) {
                row(i) = row(i-1);
            }
        } else if (m == left) {
            for (auto i = q(1); i < robot(1); ++i) {
                row(i) = row(i+1);
            }
        }
        robot += m;
    }

    auto vertical_push(auto& grid, point& robot, point const& m, point const& q) {
        auto [up, down] = std::tuple{ point(-1, 0), point(+1, 0) };
        auto col = grid.col(q(1));
        if (m == down) {
            for (auto i = q(0); i > robot(0); --i) {
                col(i) = col(i-1);
            }
        } else if (m == up) {
            for (auto i = q(0); i < robot(0); ++i) {
                col(i) = col(i+1);
            }
        }
        robot += m;
    }

    auto vertical_push_p2(auto& grid, point& robot, point const& m) {
        auto hash = [](point p) { return aoc::util::hash{}(p(0), p(1)); };
        aoc::dense::map<point, char, decltype(hash)> to_move;
        point const p = robot+m;
        auto const c = grid(p(0), p(1));
        std::function<bool(point, point)> can_move = [&](point const& q, point const& d) -> bool {
            auto const r = q+d;
            auto const x = grid(r(0), r(1));
            if (x == '#') { return false; }
            if (x == '.') {
                to_move.insert({q, grid(q(0), q(1))});
                return true;
            }
            auto const k = x == '[' ? +1 : -1;
            if(can_move(r, d) && can_move(point(r(0), r(1)+k), d)) {
                to_move.insert({r, x});
                to_move.insert({point(r(0), r(1)+k), x});
                to_move.insert({q, grid(q(0), q(1))});
                return true;
            }
            return false;
        };
        auto const k = c == '[' ? +1 : -1;
        if (can_move(p, m) && can_move(point(p(0), p(1)+k), m)) {
            for (auto const& [q, x] : to_move.values()) {
                grid(q(0), q(1)) = '.';
            }
            for (auto const& [q, x] : to_move.values()) {
                point const r = q+m;
                grid(r(0), r(1)) = x;
            }
            robot += m;
        }
    }

    auto valid(auto const& grid, point p) {
        return p(0) >= 0 && p(0) < grid.rows() && p(1) >= 0 && p(1) < grid.cols() && grid(p(0), p(1)) != '#';
    };

    auto move_robot(auto grid, point robot, std::vector<point> const& moves, bool part2 = false) {
        auto [left, right, up, down] = std::array{ point(0, -1), point(0, +1), point(-1, 0), point(+1, 0) };
        for (auto const& m : moves) {
            auto const p = robot + m;
            auto const c = grid(p(0), p(1));
            if (c == '#') {
                continue; // no movement possible
            }
            if (c == '.') {
                robot = p; // move robot to new position
                continue;
            }
            point q = p; while (valid(grid, q) && grid(q(0), q(1)) != '.') { q += m; }
            if(!valid(grid, q)) { continue; }

            if (m == right || m == left) {
                horizontal_push(grid, robot, m, q);
            } else {
                part2 ? vertical_push_p2(grid, robot, m) : vertical_push(grid, robot, m, q);
            }
        }
        return sum(grid, part2 ? '[' : 'O');
    }
} // namespace

template<>
auto advent2024::day15() -> result {
    auto [g1, g2, r1, r2, moves] = parse("./source/2024/15/input.txt");
    auto const p1 = move_robot(g1, r1, moves, /*part2=*/false);
    auto const p2 = move_robot(g2, r2, moves, /*part2=*/true);
    return aoc::result(p1, p2);
}