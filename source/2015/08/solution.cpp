#include <aoc.hpp>

template <>
auto advent2015::day08() -> void
{
    std::ifstream f("./source/2015/08/input.txt");

    u64 p1{0};
    u64 p2{0};

    for (std::string s; std::getline(f, s);) {
        u32 c1 { 0 };
        for (auto i = 0L; i < std::ssize(s); ++i, ++c1) {
            auto c = s[i];
            if (c == '\\') {
                if (aoc::equals<'x'>(s[i+1])) { i += 3; } 
                else if (aoc::equals<'\\', '\"'>(s[i+1])) { i += 1; }
            }
        }
        auto c2 = std::transform_reduce(s.begin(), s.end(), 0UL, std::plus {}, [](auto c) {
            return 1 + aoc::equals<'\\', '\"'>(c);
        });
        p1 += std::ssize(s) - c1 + 2;
        p2 += c2 + 2 - std::ssize(s);
    }
    fmt::print("part 1: {}\n", p1);
    fmt::print("part 2: {}\n", p2);
}
