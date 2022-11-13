#include <aoc.hpp>
#include <iostream>
#include <experimental/mdspan>

namespace ex = std::experimental;

template<>
auto advent2015::day13() -> void {
    // note that ending periods have been removed from each sentence (due to a scn scan issue)
    auto input = aoc::util::readlines("./source/2015/13/input.txt");

    ankerl::unordered_dense::map<std::string, int> idx;
    int i = 0;
    for (auto const& s : input) {
        auto name = s.substr(0, s.find(' '));
        if (!idx.contains(name)) {
            idx[name] = i++;
        }
    }
    idx["Me"] = i; // for part 2
    i64 sz = std::ssize(idx);
    std::vector<i64> hvec(sz * sz);
    ex::mdspan const happy(hvec.data(), sz, sz);
    std::string a, b; // NOLINT
    i64 v{};
    for (auto const& s : input) {
        if (aoc::contains(s, "lose")) {
            (void)scn::scan(s, "{} would lose {} happiness units by sitting next to {}", a, v, b);
            happy(idx[a], idx[b]) = -v;
        } else {
            (void)scn::scan(s, "{} would gain {} happiness units by sitting next to {}", a, v, b);
            happy(idx[a], idx[b]) = v;
        }
    }
    i64 maxhappy = std::numeric_limits<i64>::min();
    ankerl::unordered_dense::set<u64> seen;

    std::vector<u8> mvec(sz * sz, 0);
    ex::mdspan map(mvec.data(), sz, sz); // map of neighbours

    std::vector<u8> nc(sz, 0); // neighbour count

    auto clear = [&]() {
        seen.clear();
        std::ranges::fill(nc, 0);
        std::ranges::fill(mvec, 0);
    };

    auto search = [&](std::optional<int> current, int cost, int depth, auto&& search) {
        if (depth == sz && std::all_of(nc.begin(), nc.begin() + sz, [](auto x) { return x == 2; })) {
            if (maxhappy < cost) {
                maxhappy = cost;
            }
            return;
        }

        if (!current) {
            for (auto i = 0; i < sz; ++i) {
                search(i, cost, depth, search);
            }
            return;
        }

        if (auto [it, ok] = seen.insert(aoc::util::hash{}(mvec)); !ok) {
            return;
        }

        auto i = *current;
        if (nc[i] == 2) { return; }
        ++nc[i];
        for(auto j = 0; j < sz; ++j) {
            if (i == j || map(i, j) || nc[j] == 2) {
                continue;
            }
            ++nc[j]; map(i, j) = 1;
            search(j, cost + happy(i, j) + happy(j, i), depth + 1, search);
            --nc[j]; map(i, j) = 0;
        }
        --nc[i];
    };

    sz--;
    search({}, 0, 0, search);
    fmt::print("part 1: {}\n", maxhappy);

    sz++;
    clear();
    maxhappy = std::numeric_limits<i64>::min();
    search({}, 0, 0, search);
    fmt::print("part 2: {}\n", maxhappy);
}
