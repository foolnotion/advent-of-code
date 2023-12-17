#include <aoc.hpp>
#include <experimental/mdspan>

using std::experimental::mdspan;
using std::experimental::extents;

namespace {
    template<typename T>
    using mdspan = mdspan<T, extents<i32, std::dynamic_extent, std::dynamic_extent>>;
    using grid   = mdspan<u8>;
    using point  = std::complex<u16>;

    struct facing {
        static constexpr point left{0, -1};
        static constexpr point right{0, +1};
        static constexpr point up{-1, 0};
        static constexpr point down{+1, 0};

        static auto index(point p) {
            if (p == left)  { return 0U; }
            if (p == right) { return 1U; }
            if (p == up)    { return 2U; }
            if (p == down)  { return 3U; }
            throw std::runtime_error("invalid facing");
        }
    };

    struct turn {
        static constexpr auto left  = point{0, +1};
        static constexpr auto right = point{0, -1};
        static constexpr auto none  = point{+1, 0};
    };

    auto parse(auto const& input) {
        std::vector<grid::value_type> vec;
        vec.reserve(input.size() * input.front().size());
        for (auto const& s : input) {
            std::ranges::transform(s, std::back_inserter(vec), [](auto c) { return c - '0'; });
        }
        return vec;
    }

    struct state {
        point d;    // direction
        point p;    // position
        u32   m{0}; // momentum
        u32   c{0}; // cost so far
        u32   h{0}; // heuristic

        auto operator<(state const& s) const { return h > s.h; };
    };

    auto astar(grid grid, i32 mmin, i32 mmax) {
        auto const nrows{ grid.extent(0) };
        auto const ncols{ grid.extent(1) };
        point const goal{nrows-1, ncols-1};

        auto valid = [&](auto p) {
            auto [x, y] = std::tuple{p.real(), p.imag()};
            return x >= 0 && x < nrows && y >= 0 && y < ncols;
        };

        auto heat = [&](auto p) -> u16 {
            return grid(p.real(), p.imag());
        };

        auto distance = [&](auto p) -> u16 {
            return std::abs(p.real()-goal.real()) + std::abs(p.imag()-goal.imag());
        };

        std::vector<u64> cache(nrows * ncols, 0);

        auto try_cache = [&](state t, bool update) {
            auto& b = cache[nrows * t.p.real() + t.p.imag()];
            auto const m = 1UL << (facing::index(t.d) * (mmax+1) + t.m);
            auto res = b & m;
            if (update) { b |= m; }
            return res;
        };

        std::array turns { turn::none, turn::left, turn::right };

        std::priority_queue<state> queue;
        queue.push({ facing::right, point{0, 0}, 0, 0 });

        while(!queue.empty()) {
            auto s = queue.top();
            queue.pop();

            if (s.p == goal) { return s.c; }
            if (try_cache(s, true)) { continue; }

            auto check = [&](u32 m, point r) { return r == turn::none ? m < mmax : m >= mmin; };

            for (auto t : turns) {
                auto m = t == turn::none ? s.m+1 : 1;
                if (auto d = s.d * t, p = s.p + d; valid(p) && check(s.m, t)) {
                    state q{ d, p, m, s.c + heat(p), s.c + heat(p) + distance(p) };
                    if (!try_cache(q, false)) { queue.push(q); }
                }
            }
        }

        return std::numeric_limits<u32>::max();
    }
} // namespace

template<>
auto advent2023::day17() -> result {
    auto input = aoc::util::readlines("./source/2023/17/input.txt");
    auto const nrows{ std::ssize(input) };
    auto const ncols{ std::ssize(input.front()) };

    auto buf = parse(input);
    grid grid(buf.data(), nrows, ncols);

    auto const p1 = astar(grid, 0, 3);
    auto const p2 = astar(grid, 4, 10);

    return aoc::result(p1, p2);
}