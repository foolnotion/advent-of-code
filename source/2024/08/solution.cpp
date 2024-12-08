#include <aoc.hpp>

template<>
auto advent2024::day08() -> result {
    auto lines = aoc::util::readlines("./source/2024/08/input.txt");
    auto const nrow = std::ssize(lines);
    auto const ncol = std::ssize(lines.front());

    using point = aoc::point<i64>;
    aoc::dense::map<char, std::vector<point>> m;
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            auto const c = lines[i][j];
            if (std::isalnum(c)) {
                auto [it, ok] = m.insert({ c, {} });
                it->second.push_back({i, j});
            }
        }
    }

    auto valid = [nrow, ncol](point p) {
        return p[0] >= 0 && p[0] < nrow && p[1] >= 0 && p[1] < ncol;
    };

    auto next = [](point p, point q) {
        return point{(2*q[0])-p[0], (2*q[1])-p[1]};
    };

    auto antinodes = [&](bool part1) {
        Eigen::Array<char, -1, -1> a(nrow, ncol);
        a.setConstant('.');
        for (auto const& [c, v] : m.values()) {
            auto const s = std::ssize(v);
            for (auto i = 0; i < s-1; ++i) {
                for (auto j = i+1; j < s; ++j) {
                    if (part1) {
                        for (auto p : {next(v[i], v[j]), next(v[j], v[i])}) {
                            if (valid(p)) { a(p[0], p[1]) = '#'; }
                        }
                    } else {
                        for (auto p = v[i], q = v[j]; valid(q); p = std::exchange(q, next(p, q))) {
                            a(q[0], q[1]) = '#';
                        }
                        for (auto p = v[j], q = v[i]; valid(q); p = std::exchange(q, next(p, q))) {
                            a(q[0], q[1]) = '#';
                        }
                    }
                }
            }
        }
        return (a == '#').count();
    };
    auto const p1 = antinodes(true);
    auto const p2 = antinodes(false);
    return aoc::result(p1, p2);
}