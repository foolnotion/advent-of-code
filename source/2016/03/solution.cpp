#include <aoc.hpp>
#include <Eigen/Core>
#include <iostream>

template<>
auto advent2016::day03() -> result {
    auto input = aoc::util::readlines("./source/2016/03/input.txt");
    using point = Eigen::Array3i;

    auto is_triangle = [](point p) {
        std::ranges::sort(p);
        do {
            if (!(p[0] + p[1] > p[2])) { return false; }
        } while (std::next_permutation(p.begin(), p.end()));
        return true;
    };

    int part1{0};
    int part2{0};
    point a, b, c, p; // NOLINT
    for (auto i = 0; i < std::ssize(input); i += 3) {
        for (auto j = 0; j < 3; ++j) {
            (void)scn::scan(input[i+j], "{} {} {}", p[0], p[1], p[2]);
            part1 += is_triangle(p);
            (void)scn::scan(input[i+j], "{} {} {}", a[j], b[j], c[j]);
        }
        part2 += is_triangle(a) + is_triangle(b) + is_triangle(c);
    }
    return aoc::result(part1, part2);
}
