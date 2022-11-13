#include <aoc.hpp>

template<>
auto advent2020::day02() -> void {
    auto input = aoc::util::readlines("./source/2020/02/input.txt");

    auto n1{0UL};
    auto n2{0UL};
    u32 lo{};
    u32 hi{};
    char let{};
    std::string pass;
    for(auto& s : input) {
        (void)scn::scan(s, "{}-{} {}: {}", lo, hi, let, pass);
        constexpr auto nletter{26};
        std::array<int, nletter> counts = {0};
        for (auto c : pass) {
            auto i = c - 'a';
            counts[i]++;
        }
        auto i = let - 'a';
        n1 += (counts[i] >= lo && counts[i] <= hi);
        char a = pass[lo-1];
        char b = pass[hi-1];
        n2 += (a == let) != (b == let);
    }
    fmt::print("part 1: {}\n", n1);
    fmt::print("part 2: {}\n", n2);
}
