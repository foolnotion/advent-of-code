#include <aoc.hpp>

template<>
auto advent2020::day15() -> void {
    auto input = aoc::util::readlines("./source/2020/15/input.txt");
    auto tokens = lz::split(input.front(), ',');
    auto nums = lz::map(tokens, [](auto v) { return scn::scan_value<u32>(v).value(); }).toVector();
    robin_hood::unordered_map<u32, u32> map;
    map.reserve(nums.size());
    for (auto&& [i, n] : lz::enumerate(nums)) { map.insert({n, i+1}); }

    constexpr auto p1{2020UL};
    constexpr auto p2{30'000'000UL};

    u32 a1{};
    u32 a2{};
    u32 n{0};
    for (u32 t = nums.size()+1; t < p2; ++t) {
        if (auto [it, ok] = map.insert({ n, t }); !ok) {
            n = t - it->second;
            it->second = t;
        } else {
            n = 0;
        }
        if (t == p1-1) { a1 = n; }
    }
    a2 = n;
    fmt::print("part 1: {}\n", a1);
    fmt::print("part 2: {}\n", a2);
}

