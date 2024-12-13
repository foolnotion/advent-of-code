#include <aoc.hpp>

namespace {
    using point = aoc::point<i32, 2>;
    constexpr std::array<point, 4> neighbors{ point{-1, 0}, {0, +1}, {+1, 0}, {0, -1} };
    constexpr std::array<point, 4> diagonals{ point{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };

    auto count_sides(aoc::dense::set<point> const& inner, aoc::dense::set<point> const& outer) {
        aoc::dense::set<point> visited;
        std::function<i32(point, point, i32)> sides = [&](point p, point d, i32 c) {
            if (auto [it, ok] = visited.insert(p); !ok) {
                return c;
            }
            for (auto n : neighbors) {
                if (n == point{-d[0], -d[1]}) { continue; }
                auto [x, y] = n;
                point const q{p[0]+2*x, p[1]+2*y};
                if (!outer.contains(q)) { continue; }
                point const a = x == 0 ? point{p[0]-1, p[1]+y} : point{p[0]+x, p[1]-1};
                point const b = x == 0 ? point{p[0]+1, p[1]+y} : point{p[0]+x, p[1]+1};
                auto const ca = inner.contains(a);
                auto const cb = inner.contains(b);
                if ((ca && cb) || !(ca || cb)) { continue; }
                c = std::max(c, sides(q, n, c + (n != d)));
            }
            return c;
        };
        auto s = 0;
        while(visited.size() < outer.size()) {
            auto view = outer.values() | std::views::filter([&](auto const& p){ return !visited.contains(p); });
            auto start = std::ranges::min(view);
            s += sides(start, {0, +1}, 0);
        }
        return s;
    };
} // namespace

template<>
auto advent2024::day12() -> result {
    auto const lines = aoc::util::readlines("./source/2024/12/input.txt");
    auto [nrow, ncol] = std::tuple{std::ssize(lines), std::ssize(lines.front())};
    Eigen::Array<char, -1, -1, Eigen::RowMajor> m(nrow, ncol);
    Eigen::Array<u8, -1, -1> s = decltype(s)::Zero(nrow, ncol);

    for (auto i = 0; i < nrow; ++i) {
        std::ranges::copy(lines[i], m.row(i).begin());
    }

    [[maybe_unused]] auto valid = [&](point p) {
        return p[0] >= 0 && p[0] < nrow && p[1] >= 0 && p[1] < ncol;
    };

    auto [p1, p2] = point{0, 0};
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            if (s(i, j) == 1) { continue; }

            auto const c = m(i, j);
            auto area{0};
            auto perimeter{0};

            std::queue<point> queue;
            queue.push({i, j});

            aoc::dense::set<point> boundary;
            aoc::dense::set<point> inner;

            while(!queue.empty()) {
                auto const p = queue.front();
                queue.pop();
                auto [x, y] = p;
                if (std::exchange(s(x, y), 1) == 1) { continue; }
                inner.insert({2*x, 2*y});
                ++area;
                // map neighbors
                for (auto d : neighbors) {
                    point p{x+d[0], y+d[1]};
                    if (valid(p) && m(p[0], p[1]) == c) {
                        queue.push(p);
                        continue;
                    }
                    ++perimeter;
                }
                // map boundary
                for (auto d : diagonals) {
                    point p{x+d[0], y+d[1]};
                    if (!valid(p) || m(p[0], p[1]) != c || (m(x+d[0], y) != c && m(x, y+d[1]) != c)) {
                        boundary.insert({2*x+d[0], 2*y+d[1]});
                    }
                }
            }
            if (area == 0) { continue; }
            auto sides = count_sides(inner, boundary);
            p1 += area * perimeter;
            p2 += area * sides;
        }
    }
    return aoc::result(p1, p2);
}
