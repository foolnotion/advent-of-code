#include <aoc.hpp>

namespace {
    using std::ranges::any_of;
    using std::ranges::none_of;

    template<i64 N>
    struct state {
        using point = aoc::point<i64, 2>;
        static constexpr std::size_t size = N;

        i32 level{};
        std::array<point, N> items;

        [[nodiscard]] auto neighbors() const {
            std::vector<point> neighbors;
            for (decltype(N) i = 0; i < N; ++i) {
                auto [g, m] = items[i];
                if (g == level) { neighbors.push_back({i, 0}); }
                if (m == level) { neighbors.push_back({i, 1}); }
            }
            return neighbors;
        }

        auto move(auto dest, point item) -> state {
            state m{dest, items};
            auto [idx, type] = item;
            m.items[idx][type] = dest;
            return m;
        }

        [[nodiscard]] auto valid() const {
            return none_of(items, [&](auto a) {
                return any_of(items, [&](auto b) {
                    return a[0] != b[0] && a[0] == b[1];
                });
            });
        }

        [[nodiscard]] auto complete() const {
            return std::ranges::all_of(items, [](auto t) {
                return t[0] == t[1] && t[0] == 3;
            });
        }

        [[nodiscard]] auto hash() const {
            aoc::util::hash h;
            auto hashes = [&]<auto... Idx>(std::index_sequence<Idx...>) {
                return std::array { h(items[Idx])... };
            }(std::make_index_sequence<N>{});
            cppsort::sorting_network_sorter<N>{}(hashes);
            return level + h(hashes);
        }
    };

    auto bfs(auto items) {
        state<items.size()> start{0, items};
        using item = std::tuple<decltype(start), i32>;
        std::queue<item> queue;
        queue.push({ start, 0});

        aoc::dense::set<u64> seen;
        auto best{std::numeric_limits<i32>::max()};
        while(!queue.empty()) {
            auto [s, c] = queue.front();
            queue.pop();
            if (auto [it, ok] = seen.insert(s.hash()); !ok) {
                continue;
            }
            if (s.complete()) {
                best = std::min(best, c);
                continue;
            }

            auto nb = s.neighbors();
            if (nb.empty()) { continue; }

            for (auto f : {s.level+1, s.level-1}) {
                if (f < 0 || f > 3) { continue; }
                for (auto i = 0; i < std::ssize(nb); ++i) {
                    auto [a, b] = nb[i];
                    auto q = s.move(f, nb[i]);
                    if (q.valid()) {
                        queue.push({ q, c+1 });
                    }

                    for (auto j = i+1; j < std::ssize(nb); ++j) {
                        auto [x, y] = nb[j];
                        // skip move if G and M are not of the same kind
                        if (b != y && a != x) { continue; }
                        if (auto r = q.move(f, nb[j]); r.valid()) {
                            queue.push({ r, c+1 });
                        }
                    }
                }
            }
        }
        return best;
    }
} // namespace

template<>
auto advent2016::day11() -> result {
    std::array items_p1 {
        aoc::point<i64, 2>{ 0, 0 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
    };

    std::array items_p2 {
        aoc::point<i64, 2>{ 0, 0 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 1, 2 },
        aoc::point<i64, 2>{ 0, 0 },
        aoc::point<i64, 2>{ 0, 0 },
    };

    auto part1 = bfs(items_p1);
    auto part2 = bfs(items_p2);
    return aoc::result(part1, part2);
}
