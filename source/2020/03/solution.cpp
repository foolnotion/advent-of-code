#include <aoc.hpp>

using Eigen::Map;
using Eigen::Array;

template<>
auto advent2020::day03() -> result {
    auto input = aoc::util::readlines("./source/2020/03/input.txt");
    auto const nrow{std::ssize(input)};
    auto const ncol = std::ssize(input.front());
    Array<char, -1, -1> a(nrow, ncol);
    for (auto&& [i, s] : lz::enumerate(input)) {
        a.row(i) = Map<Array<char, 1, -1>>(s.data(), ncol);
    }
    auto n = [&](int right, int down) -> i64 {
        i64 x{0};
        i64 y{0};
        i64 n{0};
        while (x < a.rows()) {
            n += a(x, y) == '#';
            x += down;
            y = (y + right) % a.cols();
        }
        return n;
    };
    auto p1 = n(3, 1);
    auto p2 = n(1, 1) * n(3, 1) * n(5, 1) * n(7, 1) * n(1, 2);
    return aoc::result(p1, p2);
}
