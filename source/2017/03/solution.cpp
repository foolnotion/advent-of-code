#include <aoc.hpp>

using std::abs;
using std::optional;

template<>
auto advent2017::day03() const -> void {

    constexpr i64 input{361527};

    auto distance = [](i64 n) -> u64 {
        auto p = static_cast<i64>(std::sqrt(n)) + 2;
        p -= (p % 2 == 0);
        auto s = p / 2;
        p *= p;
        std::array<i64, 4> a {p-6*s, p-4*s, p-2*s, p }; // NOLINT
        auto *it = std::ranges::partition_point(a, [n](auto x) { return x < n; });
        return s + (it == a.end() ? s : abs(*it - n - s));
    };

    fmt::print("part 1: {}\n", distance(input));

    // part 2
    using point = std::array<i64, 2>;
    robin_hood::unordered_map<point, u64, aoc::util::hash> map;

    auto get = [&](point p) {
        auto it = map.find(p);
        if (it == map.end()) { return 0UL; }
        return it->second;
    };

    auto sum = [&](point p) {
        auto [x, y] = p;
        u64 s{0};
        for (auto i = x-1; i <= x+1; ++i) {
            for (auto j = y-1; j <= y+1; ++j) {
                auto v = get({i, j});
                s += v;
            }
        }
        return s;
    };

    auto apply_increment = [&](point& p, auto idx, auto inc, auto lim) -> std::optional<u64> {
        for (auto i = 0; i < lim; ++i) {
            p[idx] += inc;
            auto v = sum(p);
            if (v > input) { return {v}; }
            map.insert({p, v});
        }
        return {};
    };

    auto find_greater = [&](u64 n) -> std::optional<u64> {
        // start from 1
        map.insert({ {0, 0}, 1 });
        point p{0, 0};
        // tup of which dimension to increment, increment direction, loop limit
        using tup = std::tuple<i64, i64, i64>;
        for (auto i = 1L; ; i += 2) {
            std::array increments = {
                tup{0, +1, 1},
                tup{1, -1, i},
                tup{0, -1, i+1},
                tup{1, +1, i+1},
                tup{0, +1, i+1}
            };

            for (auto [idx, inc, lim] : increments) {
                if (auto res = apply_increment(p, idx, inc, lim); res) {
                    if (res.value() > n) { return res; }
                }
            }
        }
        return {};
    };

    if (auto res = find_greater(input); res) {
        fmt::print("part 2: {}\n", res.value());
    } else {
        fmt::print("no solution found\n");
    }
}
