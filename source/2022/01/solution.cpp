#include <aoc.hpp>

template<>
auto advent2022::day01() -> result {
    auto sum{0};
    std::vector<i32> values;
    for (auto const& s : aoc::util::readlines("./source/2022/01/input.txt")) {
        if (s.empty()) { values.push_back(sum); sum = 0; }
        sum += scn::scan_value<i32>(s).value();
    }
    std::ranges::sort(values, std::greater{});
    return aoc::result(values[0], values[0]+values[1]+values[2]);
}
