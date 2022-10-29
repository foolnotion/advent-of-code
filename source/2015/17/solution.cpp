#include <aoc.hpp>

template<>
auto advent2015::day17() -> void {
    std::array cap = { 7, 10, 11, 18, 18, 21, 22, 24, 26, 32, 36, 40, 40, 42, 43, 44, 46, 47, 49, 50 }; // NOLINT
    constexpr int sz{std::ssize(cap)};
    std::array<int, sz> used = {0};

    constexpr int volume{150};
    ankerl::unordered_dense::set<u64> seen;
    ankerl::unordered_dense::map<int, int> ways;
    int count{0};

    auto fill = [&](auto idx, auto sum, auto total, auto&& fill) {
        if (auto [it, ok] = seen.insert(aoc::util::hash{}(used)); !ok) { return; }

        if (sum == total) {
            ++count;
            ways[std::reduce(used.begin(), used.end())]++;
            return;
        }

        for (auto i = idx; i < sz; ++i) {
            if (used[i] || sum + cap[i] > total) { continue; }
            used[i] = 1;
            fill(idx + 1, sum + cap[i], total, fill);
            used[i] = 0;
        }
    };

    fill(0, 0, volume, fill);
    fmt::print("part 1: {}\n", count);
    fmt::print("part 2: {}\n", std::ranges::min_element(ways, [](auto a, auto b) { return a.first < b.first; })->second);
}
