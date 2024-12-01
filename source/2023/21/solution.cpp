#include <aoc.hpp>
#define MDSPAN_IMPL_STANDARD_NAMESPACE std
#define MDSPAN_IMPL_PROPOSED_NAMESPACE experimental
#include <mdspan/mdspan.hpp>
#include <tlx/container/radix_heap.hpp>

namespace rs = std::ranges;

using point   = aoc::point<i32, 2>;
using extents = std::extents<i32, std::dynamic_extent, std::dynamic_extent>;
using mdspan  = std::mdspan<char, extents>;

namespace {
    auto parse(auto const& input) {
        std::vector<char> buf;
        buf.reserve(input.size()* input.front().size());
        auto copy = [&](auto const& s) { rs::copy(s, std::back_inserter(buf)); };
        rs::for_each(input, copy);
        return buf;
    }

    auto print(auto grid) {
        for (auto i{0}; i < grid.extent(0); ++i) {
            for (auto j{0}; j < grid.extent(1); ++j) {
                fmt::print("{}", grid(i, j));
            }
            fmt::print("\n");
        }
    }
} // namespace

template<>
auto advent2023::day21() -> result {
    auto input = aoc::util::readlines("./source/2023/21/example.txt");
    auto const nrows{std::ssize(input)};
    auto const ncols{std::ssize(input.front())};
    auto buf = parse(input);
    mdspan grid(buf.data(), nrows, ncols);

    auto s = rs::find(buf, 'S');
    auto x = std::distance(buf.begin(), s) / ncols;
    auto y = std::distance(buf.begin(), s) % ncols;
    point p{static_cast<i32>(x), static_cast<i32>(y)};

    auto remap = [&](point p) -> point {
        auto mod = [](auto a, auto b) -> i32 {
            while(a < 0) { a += b; }
            return a % b;
        };
        return point{mod(p[0], nrows), mod(p[1], ncols)};
    };

    auto valid = [&](point p) -> bool {
        auto [x, y] = remap(p);
        return x >= 0 && x < nrows && y >= 0 && y < ncols && aoc::equals<'.', 'S'>(grid(x, y));
    };

    auto neighbors = [&](point p) {
        return std::array {
            point { p[0]-1, p[1] },
            point { p[0]+1, p[1] },
            point { p[0], p[1]-1 },
            point { p[0], p[1]+1 }
        };
    };

    auto hash = [](auto p) {
        auto [c, q] = p;
        return aoc::util::hash{}(c, q[0], q[1]);
    };

    aoc::dense::set<std::pair<i32, point>, decltype(hash)> seen;

    auto count{0UL};
    constexpr auto steps{10};



    std::queue<std::pair<i32, point>> queue;
    queue.emplace(0, p);
    while(!queue.empty()) {
        auto [c, q] = queue.front();
        queue.pop();

        if (auto [it, ok] = seen.insert({c, q}); !ok) {
            continue;
        }

        count += c == steps;
        if (c > steps) { continue; }

        for (auto r : neighbors(q) | std::views::filter(valid)) {
            queue.emplace(c+1, r);
        }
    }
    auto const p1 = count;

    // part 2
    std::vector<i64> dist(nrows * ncols, 0L);
    std::mdspan<i64, extents> distances(dist.data(), nrows, ncols);

    aoc::dense::set<point> ss;
    queue.emplace(0, p);
    while(!queue.empty()) {
        auto [c, q] = queue.front();
        queue.pop();

        auto r = remap(q);
        fmt::print("{}\n", r);

        if (auto [it, ok] = ss.insert(r); !ok) {
            continue;
        }
        distances(r[0], r[1]) = c;

        for (auto n : neighbors(q) | std::views::filter(valid)) {
            queue.emplace(c+1, n);
        }
    }

    for (auto i = 0; i < nrows; ++i) {
        for (auto j = 0; j < ncols; ++j) {
            auto color = grid(i, j) == 'S' ? fmt::color::green : fmt::color::gray;
            if (distances(i, j) > 0 && steps % distances(i, j) == 0) { color = fmt::color::orange; }
            if (grid(i, j) == '#') { color = fmt::color::red; }
            fmt::print(fmt::fg(color), "{:#2d} ", distances(i, j));
        }
        fmt::print("\n");
    }

    return aoc::result(p1, "");
}
