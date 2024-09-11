#include <aoc.hpp>
#include <complex>
#include <iterator>

template<>
auto advent2016::day01() -> result {
    auto input = aoc::util::readlines("./source/2016/01/input.txt").front();

    using std::complex;
    complex dir{0,1}; // direction
    complex pos{0,0}; // position

    auto hash = [](auto c) { return aoc::util::hash{}(c.real(), c.imag()); };
    aoc::dense::set<decltype(pos), decltype(hash)> seen;
    auto distance = [](auto c) { return std::abs(c.real()) + std::abs(c.imag()); };

    auto part2{0};
    for (auto const& m : lz::split(input, ", ")) {
        dir *= m[0] == 'L' ? complex{0, 1} : complex{0, -1};
        std::string_view sv{&m[1], m.size()-1};
        auto s = scn::scan_value<int>(sv)->value();
        for (auto i = 0; i < s; ++i) {
            pos += dir;
            if (auto [_, ok] = seen.insert(pos); !ok) {
                if (!part2) { part2 = distance(pos); }
            }
        }
    }
    auto part1 = distance(pos);
    return aoc::result(part1, part2);
}
