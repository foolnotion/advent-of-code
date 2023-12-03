#include <aoc.hpp>

template<>
auto advent2023::day03() -> result {
    auto input = aoc::util::readlines("./source/2023/03/input.txt");
    auto rows = std::ssize(input);
    auto cols = std::ssize(input.front());

    Eigen::Array<char, -1, -1, Eigen::RowMajor> map(rows, cols);
    for (auto i = 0; i < rows; ++i) {
        std::ranges::copy(input[i], map.row(i).begin());
    }

    auto p1{0L};
    auto p2{0L};
    for (auto i = 0; i < map.rows(); ++i) {
        for (auto j = 0; j < map.cols(); ++j) {
            auto c = map(i, j);
            if (c == '.' || std::isdigit(c)) { continue; }

            auto n = 0;
            auto p = 1;
            for (auto x = i-1; x <= i+1; ++x) {
                for (auto y = j-1; y <= j+1; ++y) {
                    if (x == i && y == j) { continue; }
                    if (x < 0 || x >= rows) { continue; }
                    if (y < 0 || y >= cols) { continue; }
                    if (std::isdigit(map(x, y))) {
                        auto k = y;
                        auto l = y;
                        while(k >= 0 && std::isdigit(map(x, k))) { --k; }
                        while(l < cols && std::isdigit(map(x, l))) { ++l; }
                        k += !std::isdigit(map(x, k));
                        l += std::isdigit(map(x, l));
                        auto row = map.row(x);
                        std::string_view sv{row.data()+k, static_cast<u64>(l-k)};
                        auto v = scn::scan_value<i32>(sv).value();
                        while(k < l) { row(k++) = '.'; }

                        p1 += v;
                        p  *= v;
                        if (++n == 2) { p2 += p; }
                    }
                }
            }
        }
    }

    return aoc::result(p1, p2);
}