#include <aoc.hpp>
#define MDSPAN_IMPL_STANDARD_NAMESPACE std
#define MDSPAN_IMPL_PROPOSED_NAMESPACE experimental
#include <mdspan/mdspan.hpp>
#include <tlx/container/radix_heap.hpp>

namespace rs = std::ranges;

namespace {
    using point = std::complex<i64>;
    using extents = std::extents<i32, std::dynamic_extent, std::dynamic_extent>;
    using mdspan = std::mdspan<char, extents>;

    auto parse(auto const& input) {
        std::vector<char> vec;
        vec.reserve(input.size() * input.front().size());
        auto copy = [&](auto const& s) { rs::copy(s, std::back_inserter(vec)); };
        rs::for_each(input, copy);
        return vec;
    }

    struct state {
        point p{}; // current position
        i64 d{};   // distance walked
        i64 h{};   // heuristic

        friend auto operator<(state a, state b) {
            return a.d + a.h < b.d + b.h;
        }
    };
} // namespace

template<>
auto advent2023::day23() -> result {
    auto input = aoc::util::readlines("./source/2023/23/input.txt");
    auto const nrows{std::ssize(input)};
    auto const ncols{std::ssize(input.front())};
    auto vec = parse(input);

    mdspan grid(vec.data(), nrows, ncols);

    point const start{0, 1};
    point const end{nrows-1, ncols-2};

    // print(grid, start);

    auto neighbors = [](point p, char c) -> std::array<std::optional<point>, 4> {
        // if (c == '<') { return { point{p.real(), p.imag()-1}, std::nullopt, std::nullopt, std::nullopt }; }
        // if (c == '>') { return { point{p.real(), p.imag()+1}, std::nullopt, std::nullopt, std::nullopt }; }
        // if (c == 'v') { return { point{p.real()+1, p.imag()}, std::nullopt, std::nullopt, std::nullopt }; }
        // if (c == '^') { return { point{p.real()-1, p.imag()}, std::nullopt, std::nullopt, std::nullopt }; }
        // if (c == '.') {
        //     auto [x, y] = std::tuple{p.real(), p.imag()};
        //     return { point{x-1, y}, point{x+1, y}, point{x, y-1}, point{x, y+1} };
        // }
        auto [x, y] = std::tuple{p.real(), p.imag()};
        return { point{x-1, y}, point{x+1, y}, point{x, y-1}, point{x, y+1} };
        throw std::runtime_error(fmt::format("unknown tile type: \"{}\"", c));
    };

    auto valid = [&](std::optional<point> p) {
        if(!p.has_value()) { return false; }
        auto [x, y] = std::tuple{p->real(), p->imag()};
        return x >= 0 && x < nrows && y >= 0 && y < ncols && grid(x, y) != '#';
    };

    auto distance = [&](point p) {
        return std::abs(end.real() - p.real()) + std::abs(end.imag() - p.imag());
    };

    auto dmax{0L};

    std::uniform_real_distribution<double> dist{-1, 1};
    std::random_device rng{};

    aoc::dense::map<point, i64> seen;

    aoc::dense::set<u64> seen2;
    auto f = [&](auto start, auto end, auto seen) -> void {
        std::priority_queue<state> queue;
        queue.push(state { start, 0 });
        while(!queue.empty()) {
            auto [p, d, h] = queue.top();
            queue.pop();

            if (p == end) {
                if (dmax < d) {
                    dmax = d;
                    fmt::print("{}\n", dmax);
                }

                continue;
            }

            if (auto [it, ok] = seen.insert({p, 0}); !ok) {
                continue;
            }

            auto c = grid(p.real(), p.imag());

            std::vector<point> nb;
            for (auto n : neighbors(p, c)) {
                if (valid(n)) { nb.push_back(*n); }
            }
            for (auto const q : nb) {
                queue.push(state { q, d+1, distance(q) + std::uniform_int_distribution<i64>{-5, 5}(rng) });
            }
        }
    };

    for (auto i = 0; i < 5000; ++i) {
        f(start, end, seen);
    }

    auto const p1 = dmax;
    auto const p2 = 0;
    return aoc::result(p1, "");
}