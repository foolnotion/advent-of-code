#include <aoc.hpp>

template<>
auto advent2018::day01() -> result {
    auto lines = aoc::util::readlines("source/2018/01/input.txt");
    auto values = lines | std::views::transform(aoc::util::read<i32>);
    auto p1 = std::reduce(values.begin(), values.end());

    aoc::dense::set<i64> seen;

    auto const sz{ std::ssize(values) };
    auto p2 = 0;
    for (auto i = 0L; ; ++i) {
        i %= sz;
        if (auto [it, ok] = seen.insert(p2); !ok) { break; }
        p2 += values[i];
    }
    return aoc::result(p1, p2);
}