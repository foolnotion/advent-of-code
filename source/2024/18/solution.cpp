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
} // namespace

template<>
auto advent2024::day18() -> result {
    auto const lines = aoc::util::readlines("./source/2024/18/input.txt");
    constexpr auto nrow{71};
    constexpr auto ncol{71};
    mdarray grid(nrow, ncol);
    std::ranges::fill(grid.container(), '.');

    auto valid = [&](point const p) {
        return p[0] >= 0 && p[0] < nrow && p[1] >= 0 && p[1] < ncol;
    };

    constexpr auto threshold{1024};

    for (auto const& line : lines | std::views::take(threshold)) {
        auto [x, y] = scn::scan<i32, i32>(line, "{},{}")->values();
        grid(y, x) = '#';
    }

    point const start{0, 0};
    point const end{nrow-1, ncol-1};

    using state = std::tuple<point, i32>;
    constexpr std::array<point, 4> directions{ point{0, 1}, {0, -1}, {1, 0}, {-1, 0} };
    constexpr auto no_path = std::numeric_limits<i32>::max();

    auto shortest_path = [&](point a, point b) -> std::pair<i32, bitset> {
        auto best{no_path};
        bitset seen((static_cast<u64>(nrow * ncol)));
        std::queue<state> queue;
        queue.emplace(a, 0);

        bitset bad(seen.size());
        while(!queue.empty()) {
            auto [p, c] = queue.front();
            queue.pop();

            if (p == b) {
                best = std::min(best, c);
                continue;
            }

            for (auto const d : directions) {
                auto const q = p+d;
                if (!valid(q)) { continue; }
                if (grid(q[0], q[1]) == '#' || seen.test_set(q[0] * ncol + q[1])) {
                    bad.set(q[0] * ncol + q[1]);
                    continue;
                }
                queue.emplace(q, c+1);
            }
        }
        return {best, std::move(bad)};
    };
    auto [p1, bad] = shortest_path(start, end);

    // part 2
    std::string p2;
    for (auto const& line : lines | std::views::drop(threshold)) {
        auto [x, y] = scn::scan<i32, i32>(line, "{},{}")->values();
        grid(y, x) = '#';
        if (bad.test(x * ncol + y)) { continue; }
        auto result = shortest_path(start, end);
        if (result.first == no_path) {
            p2 = fmt::format("{},{}", x, y);
            break;
        }
        std::swap(bad, result.second);
    }
    return aoc::result(p1, p2);
}