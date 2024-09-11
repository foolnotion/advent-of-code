#include <aoc.hpp>

template<>
auto advent2020::day02() -> result {
    auto input = aoc::util::readlines("./source/2020/02/input.txt");

    auto n1{0UL};
    auto n2{0UL};
    // u32 lo{};
    // u32 hi{};
    // char let{};
    // std::string pass;
    using std::string;
    for(auto& s : input) {
        auto [lo, hi, let, pass] = scn::scan<u32, u32, char, string>(s, "{}-{} {}: {}")->values();
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
    return aoc::result(n1, n2);
}
