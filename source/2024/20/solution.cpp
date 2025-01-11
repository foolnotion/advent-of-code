#include <aoc.hpp>
#include <sul/dynamic_bitset.hpp>

namespace {
    using point   = aoc::point<i32, 2>;
    using extents = std::extents<i32, std::dynamic_extent, std::dynamic_extent>;
    using mdarray = std::experimental::mdarray<char, extents>;
    using bitset  = sul::dynamic_bitset<u64>;

    auto operator+(point const& a, point const& b) -> point {
        return {a[0]+b[0], a[1]+b[1]};
    }

    auto operator-(point const& a, point const& b) -> point {
        return {a[0]-b[0], a[1]-b[1]};
    }

    auto l1norm(point p) -> i32 { return std::abs(p[0]) + std::abs(p[1]); }
} // namespace

template<>
auto advent2024::day20() -> result {
    auto const lines = aoc::util::readlines("./source/2024/20/input.txt");
    auto const nrow = static_cast<i32>(std::ssize(lines));
    auto const ncol = static_cast<i32>(std::ssize(lines.front()));
    mdarray grid(nrow, ncol);
    std::ranges::fill(grid.container(), '.');

    point start;
    point end;
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            auto const c = lines[i][j];
            if (c == 'S') { start = {i, j}; }
            if (c == 'E') { end   = {i, j}; }
            grid(i, j) = c;
        }
    }

    bitset seen(grid.size());
    auto idx = [&](point const p) { return p[0] * ncol + p[1]; };
    auto valid = [&](point const p) {
        return p[0] >= 0 && p[0] < nrow && p[1] >= 0 && p[1] < ncol;
    };
    static constexpr std::array<point, 4> directions{ point{0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    std::experimental::mdarray<i32, std::extents<i32, ~0UL, ~0UL>> cmap(nrow, ncol);
    std::ranges::fill(cmap.container(), std::numeric_limits<i32>::max());

    i32 best_score{std::numeric_limits<i32>::max()};
    std::deque<point> best_path;
    auto shortest_path = [&](point p, i32 c, std::deque<point>& path, auto&& rec) {
        if (seen.test_set(idx(p))) {
            return;
        }

        if (c > best_score) {
            return;
        }

        cmap(p[0], p[1]) = std::min(cmap(p[0], p[1]), c);
        path.push_back(p);

        if (p == end) {
            if (c < best_score) {
                best_score = c;
                best_path  = path;
                return;
            }
        }

        for (auto const d : directions) {
            auto const q = p+d;
            if (!valid(q) || grid(q[0], q[1]) == '#') { continue; }
            rec(q, c+1, path, rec);
        }

        path.pop_back();
    };

    std::deque<point> path;
    shortest_path(start, 0, path, shortest_path);
    std::swap(path, best_path);

    auto const s1{2};
    auto const s2{20};
    auto const min_difference{100};
    auto p1{0L};
    auto p2{0L};
    for (auto [x, y] : path) {
        auto const a = cmap(x, y);
        for (auto i = std::max(x-s2, 0); i < std::min(x+s2+1, nrow); ++i) {
            for (auto j = std::max(y-s2, 0); j < std::min(y+s2+1, ncol); ++j) {
                if (grid(i, j) == '#') { continue; }
                auto const b = cmap(i, j);
                auto const c = b-a;
                auto const d = l1norm(point{x, y} - point{i, j});

                p1 += (d <= s1 && c-d >= min_difference);
                p2 += (d <= s2 && c-d >= min_difference);
            }
        }
    }
    return aoc::result(p1, p2);
}