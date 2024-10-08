#include <aoc.hpp>

template<>
auto advent2017::day06() -> result {
    constexpr std::array input{ 4UL, 1UL, 15UL, 12UL, 0UL, 9UL, 9UL, 5UL, 5UL, 8UL, 7UL, 3UL, 14UL, 5UL, 12UL, 3UL };
    constexpr int nb{input.size()};

    auto mem = input;
    ankerl::unordered_dense::map<decltype(mem), u64, aoc::util::hash> seen;

    u64 cycles_part1{0};
    u64 cycles_part2{0};
    while(cycles_part2 == 0) {
        auto* it = std::ranges::max_element(mem);
        auto i = std::distance(mem.begin(), it);
        ++cycles_part1;
        for (auto j = (i+1) % nb; mem[i] > 0; j = (j+1) % nb) {
            --mem[i];
            ++mem[j];
        }
        if (auto [it, ok] = seen.insert({mem, cycles_part1}); !ok) {
            cycles_part2 = cycles_part1 - it->second;
        }
    }
    return aoc::result(cycles_part1, cycles_part2);
};
