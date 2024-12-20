#include <aoc.hpp>
#include <sul/dynamic_bitset.hpp>

namespace {
    using point   = aoc::point<i32, 2>;
    using extents = std::extents<i32, std::dynamic_extent, std::dynamic_extent>;
    using mdspan  = std::mdspan<char, extents>;
    using bitset  = sul::dynamic_bitset<u64>;

    auto operator+(point const& a, point const& b) -> point {
        return {a[0]+b[0], a[1]+b[1]};
    }

    [[maybe_unused]] auto distance(point const p, point const q) -> i32 {
        return std::abs(p[0]-q[0]) + std::abs(p[1]-q[1]);
    }

    auto shortest_path(mdspan m, point a, point b) -> std::pair<i32, std::vector<std::tuple<point, i32>>> {
        using state = std::tuple<point, i32, std::vector<std::tuple<point, i32>>>;
        std::queue<state> queue;
        queue.emplace(a, 0, std::vector<std::tuple<point, i32>>{});

        i32 best_score{std::numeric_limits<i32>::max()};
        std::vector<std::tuple<point, i32>> best_path;

        bitset seen(m.size());
        auto idx = [&](point const p) { return p[0] * m.extent(1) + p[1]; };

        auto valid = [&](point const p) {
            return p[0] >= 0 && p[0] < m.extent(0) && p[1] >= 0 && p[1] < m.extent(1);
        };
        static constexpr std::array<point, 4> directions{ point{0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        while(!queue.empty()) {
            auto [p, c, v] = queue.front();
            queue.pop();

            v.emplace_back(p, c);;

            if (p == b) {
                if (c < best_score) {
                    best_score = c;
                    best_path  = v;
                }
                continue;
            }

            for (auto const d : directions) {
                auto const q = p+d;
                if (!valid(q) || m(q[0], q[1]) == '#' || seen.test_set(idx(q))) { continue; }
                queue.emplace(q, c+1, v);
            }
        }
        return {best_score, best_path};
    };
} // namespace

template<>
auto advent2024::day20() -> result {
    auto const lines = aoc::util::readlines("./source/2024/20/input.txt");
    auto const nrow = std::ssize(lines);
    auto const ncol = std::ssize(lines.front());
    std::vector<char> data((static_cast<std::size_t>(nrow * ncol)), '.');
    mdspan grid(data.data(), nrow, ncol);

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
    auto [cost, path] = shortest_path(grid, start, end);

    auto const s1{2};
    auto const s2{20};
    auto const min_difference(100);

    auto p1{0L};
    auto p2{0L};
    for (auto i = 0; i < path.size()-1; ++i) {
        auto const [p, c] = path[i];
        for (auto j = i+1; j < path.size(); ++j) {
            auto const [q, d] = path[j];
            auto const x = distance(p, q);
            p1 += (x <= s1 && d - c - x >= min_difference);
            p2 += (x <= s2 && d - c - x >= min_difference);
        }
    }
    return aoc::result(p1, p2);
}