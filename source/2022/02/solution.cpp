#include <aoc.hpp>

template<>
auto advent2022::day02() -> result {
    Eigen::Array<int, 3, 3> outcome_p1 {
        { 4, 8, 3 }, // NOLINT
        { 1, 5, 9 }, // NOLINT
        { 7, 2, 6 }  // NOLINT
    };
    Eigen::Array<int, 3, 3> outcome_p2 {
        { 3, 4, 8 }, // NOLINT
        { 1, 5, 9 }, // NOLINT
        { 2, 6, 7 }  // NOLINT
    };
    auto input = aoc::util::readlines("./source/2022/02/input.txt");
    auto res = std::transform_reduce(input.begin(), input.end(), Eigen::Array2i{0, 0}, std::plus{}, [&](auto const& s) {
        auto [a, b] = scn::scan<char, char>(s, "{} {}")->values();
        return Eigen::Array2i{outcome_p1(a-'A', b-'X'), outcome_p2(a-'A', b-'X')};
    });
    return aoc::result(res(0), res(1));
}
