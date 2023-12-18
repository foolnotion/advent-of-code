#include <aoc.hpp>
#include <fplus/fplus.hpp>

template<>
auto advent2019::day01() -> result {
    auto input = aoc::util::readlines("./source/2019/01/input.txt");
    std::vector<i64> fuel;
    fuel.reserve(input.size());
    std::ranges::copy(fplus::transform(&aoc::util::read<i64>, input), std::back_inserter(fuel));

    auto convert = [](i64 m) { return static_cast<i64>(std::floor(m / 3.0)) - 2; };

    auto adjust = [convert](i64 m) {
        auto t{0L};
        while (m > 0) {
            m = convert(m);
            t += (m > 0) * m;
        };
        return t;
    };
    auto const p1 = fplus::fold_left(std::plus{}, 0L, fplus::transform(convert, fuel));
    auto const p2 = fplus::fold_left(std::plus{}, 0L, fplus::transform(adjust, fuel));
    return aoc::result(p1, p2);
}