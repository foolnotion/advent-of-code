#include <aoc.hpp>
#include <bitset>

template<>
auto advent2020::day06() -> result {
    auto input = aoc::util::readlines("./source/2020/06/input.txt");
    constexpr auto nletter{26};
    std::bitset<nletter> bits; bits.reset();
    auto all = bits; all.set();

    u32 count1{0};
    u32 count2{0};
    for (auto const& line : input) {
        if (line.empty()) {
            count1 += bits.count();
            bits.reset();

            count2 += all.count();
            all.set();

            continue;
        }
        std::bitset<nletter> tmp;
        for (char c : line) {
            bits.set(c - 'a', true);
            tmp.set(c - 'a', true);
        }
        all &= tmp;
    }
    return aoc::result(count1, count2);
}

