#include <aoc.hpp>

namespace {
    using point    = aoc::point<i32, 2>;
    using line     = std::pair<point, point>;
    using interval = aoc::math::interval<i32>;

    auto parse(auto const& path) -> std::vector<line> {
        std::vector<line> lines;

        auto x{0};
        auto y{0};
        for (auto const p : path | std::views::split(',')) {
            if (p.empty()) { continue; }
            std::string_view v{p.begin()+1, p.end()};
            auto m = aoc::util::read<i32>(v);
            auto c = p.front();

            if (c == 'L' || c == 'R') {
                auto y1 = c == 'L' ? y - m : y + m;
                lines.emplace_back(point{x, y}, point{x, y1});
                y = y1;
            } else if (c == 'U' || c == 'D') {
                auto x1 = c == 'U' ? x - m : x + m;
                lines.emplace_back(point{x, y}, point{x1, y});
                x = x1;
            }
        }

        return lines;
    }

    auto distance(auto x, auto y) { return std::abs(x) + std::abs(y); };

    auto length(line l) {
        return distance(l.first[0]-l.second[0], l.first[1]-l.second[1]);
    };

    auto total_distance(auto const& lines, point p) {
        auto d{0};
        for (auto [a, b] : lines) {
            auto i = a[0] == b[0] ? 1 : 0;
            auto z = i == 0 ? a[1] : a[0];

            interval v{std::min(a[i], b[i]), std::max(a[i], b[1])};
            auto [x, y] = p;
            if ((x == z && v.contains(y)) || (y == z && v.contains(x))) {
                d += length({a, p});
                break;
            }
            d += length({a, b});
        }
        return d;
    }
} // namespace

template<>
auto advent2019::day03() -> result {
    auto input = aoc::util::readlines("./source/2019/03/input.txt");
    auto const& path1 = input.front();
    auto const& path2 = input.back();

    auto l1 = parse(path1);
    auto l2 = parse(path2);

    auto min_distance{std::numeric_limits<i32>::max()};
    auto min_steps{std::numeric_limits<i32>::max()};

    auto compute_minimums = [&](point p) {
        auto d1 = total_distance(l1, p);
        auto d2 = total_distance(l2, p);

        min_distance = std::min(distance(p[0], p[1]), min_distance);
        min_steps    = std::min(min_steps, d1+d2);
    };

    auto intersect_v = [&](line l) {
        auto [p1, p2] = l;
        auto x = p1[0];

        for (auto [q1, q2] : l2) {
            if (q1[0] == q2[0]) { continue; }
            auto y = q1[1];
            if (x == y && x == 0) { continue; }
            // this is a vertical line
            interval i{std::min(p1[1], p2[1]), std::max(p1[1], p2[1])};
            interval j{std::min(q1[0], q2[0]), std::max(q1[0], q2[0])};

            if (i.contains(y) && j.contains(x)) {
                compute_minimums({x, y});
            }
        }
    };

    auto intersect_h = [&](line l) {
        auto [p1, p2] = l;
        auto y = p1[1];

        for (auto [q1, q2] : l2) {
            if (q1[1] == q2[1]) { continue; }
            auto x = q1[0];
            if (x == y && x == 0) { continue; }
            // this is a horizontal line
            interval i{std::min(p1[0], p2[0]), std::max(p1[0], p2[0])};
            interval j{std::min(q1[1], q2[1]), std::max(q1[1], q2[1])};

            if (i.contains(x) && j.contains(y)) {
                compute_minimums({x, y});
            }
        }
    };

    for (auto [p1, p2] : l1) {
        if (p1[0] == p2[0]) { intersect_v({p1, p2}); }
        else                { intersect_h({p1, p2}); }
    }

    auto const p1 = min_distance;
    auto const p2 = min_steps;
    return aoc::result(p1, p2);
}