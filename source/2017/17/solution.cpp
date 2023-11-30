#include <aoc.hpp>

template<>
auto advent2017::day17() -> result {
    constexpr auto step{348};

    // part 1
    constexpr auto year{2017};
    std::list<i64> buf{0};
    auto p{0L}; // current position
    auto it = buf.begin();
    for (auto i = 0; i < year; ++i) {
        auto q = (p + step) % (i+1);
        std::advance(it, q-p);
        p = q+1;
        buf.insert(std::next(it), i+1);
        ++it;
    }
    auto p1 = *(std::next(it));

    // part 2
    constexpr auto iter{50'000'000};
    p = 0L;
    auto p2{0L};
    for (auto i = 1; i <= iter; ++i) {
        if (p == 1) { p2 = i-1; }
        while (p+step < i) {
            p += step+1;
            i += 1;
        }
        p = (p+step) % i + 1;
    }
    return aoc::result(p1, p2);
}