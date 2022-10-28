#include <aoc.hpp>
#include <util/dynamic_bitset.hpp>

using point = aoc::point<u64>;

template <>
auto advent2015::day09() -> void // NOLINT
{
    auto input = aoc::util::readlines("./source/2015/09/input.txt");
    robin_hood::unordered_flat_map<u64, std::string> names;

    using point = aoc::point<u64>;
    robin_hood::unordered_map<point, i64> dist;
    aoc::util::hash hash;

    for (auto const& s : input) {
        std::string a, b; // NOLINT
        u32 d {};
        (void)scn::scan(s, "{} to {} = {}", a, b, d);
        auto ha = hash(a);
        auto hb = hash(b);
        names.insert({ ha, a });
        names.insert({ hb, b });
        dist[{ ha, hb }] = dist[{ hb, ha }] = d;
    }

    std::vector<u64> cities = lz::map(names, [](auto const& x) { return x.first; }).toVector();

    i64 goal {};
    robin_hood::unordered_flat_set<u64> visited;
    auto search = [&]<typename F, typename G>(auto curr, auto depth, auto distance, F accept, G reject, auto&& search) {
        visited.insert(curr);
        if (depth == cities.size() && accept(distance, goal)) {
            goal = distance;
            visited.erase(curr);
            return;
        }
        for (auto next : lz::filter(cities, [&](auto x) { return !visited.contains(x); })) {
            auto d = dist[{ curr, next }];
            if (reject(distance + d, goal)) {
                continue;
            }
            search(next, depth + 1, distance + d, accept, reject, search);
        }
        visited.erase(curr);
    };

    goal = std::numeric_limits<i64>::max();
    for (auto c : cities) {
        visited.clear();
        search(c, 1, 0, std::less{}, std::greater{}, search);
    }
    fmt::print("part 1: {}\n", goal);

    goal = std::numeric_limits<i64>::min();
    for (auto c : cities) {
        visited.clear();
        search(c, 1, 0, std::greater{}, [](i64, i64) { return false; }, search);
    }
    fmt::print("part 2: {}\n", goal);
}
