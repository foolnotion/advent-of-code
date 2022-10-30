#include <aoc.hpp>
#include <Eigen/Core>

template<>
auto advent2015::day18() -> void {

    auto input = aoc::util::readlines("./source/2015/18/input.txt");
    constexpr int dim{100};

    Eigen::Array<char, -1, -1> initial_state(dim, dim);
    for (auto const&& [i, s] : lz::enumerate(input)) {
        initial_state.row(i) = Eigen::Map<Eigen::Array<char, -1, 1>>(s.data(), std::ssize(s));
    }

    constexpr int steps{100};
    auto count = [&](bool part2) {
        decltype(initial_state) map = initial_state;;
        decltype(initial_state) aux(dim, dim);

        if (part2) {
            map(0, 0) = map(0, map.cols()-1) = map(map.rows()-1, 0) = map(map.rows()-1, map.cols()-1) = '#';
        }

        for (auto s = 0; s < steps; ++s) {
            for (auto x = 0; x < map.rows(); ++x) {
                for (auto y = 0; y < map.cols(); ++y) {
                    auto x1 = std::max(x-1, 0);
                    auto y1 = std::max(y-1, 0);
                    auto x2 = std::min(x+1L, map.rows()-1);
                    auto y2 = std::min(y+1L, map.cols()-1);
                    auto s = (map.block(x1, y1, x2-x1+1, y2-y1+1) == '#').count() - (map(x, y) == '#');

                    if (map(x, y) == '#' && !(s == 2 || s == 3)) {
                        aux(x, y) = '.'; // turn off
                    } else if (map(x, y) == '.' && s == 3) {
                        aux(x, y) = '#'; // turn on
                    } else {
                        aux(x, y) = map(x, y);
                    }
                }
            }
            std::swap(map, aux);
            if (part2) {
                map(0, 0) = map(0, map.cols()-1) = map(map.rows()-1, 0) = map(map.rows()-1, map.cols()-1) = '#';
            }
        }
        return (map == '#').count();
    };

    fmt::print("part 1: {}\n", count(false));
    fmt::print("part 2: {}\n", count(true));
}
