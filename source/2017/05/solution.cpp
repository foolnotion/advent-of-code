#include <aoc.hpp>
#include <scn/detail/file.h>

template<>
auto advent2017::day05() -> result {
    auto input = aoc::util::readlines("./source/2017/05/input.txt");
    auto vec = lz::map(input, [](auto const& s){ return scn::scan_value<i64>(s).value(); }).toVector();
    auto tmp = vec;

    u64 steps{0};
    for (i64 i = 0; i < vec.size();) {
        i += vec[i]++;
        ++steps;
    }
    auto p1 = steps;

    steps = 0;
    std::swap(vec, tmp);
    for (i64 i = 0; i < vec.size();) {
        auto& v = vec[i];
        i += v;
        v += v < 3 ? 1 : -1;
        ++steps;
    }
    auto p2 = steps;
    return aoc::result(p1, p2);
}
