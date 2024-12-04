#include <aoc.hpp>

namespace {
    auto find_p1(auto const& m, auto x, auto y) -> i32 {
        std::string s;
        constexpr std::array<std::pair<i32, i32>, 8> directions = {{
            {+1,  0}, {-1,  0}, { 0, -1}, { 0, +1}, // lines
            {+1, +1}, {+1, -1}, {-1, -1}, {-1, +1}, // diagonals
        }};
        return std::ranges::count_if(directions, [&](auto const p) {
            s.clear();
            for (auto i = x, j = y; s.size() < 4; i += p.first, j += p.second) {
                if (!(i >= 0 && i < m.rows() && j >= 0 && j < m.cols())) {
                    return false;
                }
                s.push_back(m(i, j));
            }
            return s == "XMAS";
        });
    }

    auto find_p2(auto const& m, auto x, auto y) -> bool {
        if (!(x-1 >= 0 && y-1 >= 0 && x+1 < m.rows() && y+1 < m.cols())) {
            return false;
        }
        auto [a, b, c, d] = std::tuple{-1, +1, +1, +1};
        auto check = [&](auto i, auto j) {
            return (m(x+i, y+j) == 'M' && m(x-i, y-j) == 'S') ||
                   (m(x+i, y+j) == 'S' && m(x-i, y-j) == 'M');
        };
        return check(a, b) && check(c, d);
    }
} // namespace

template<>
auto advent2024::day04() -> result {
    auto lines = aoc::util::readlines("./source/2024/04/input.txt");
    auto const nrow = std::ssize(lines);
    auto const ncol = std::ssize(lines.front());
    Eigen::Array<char, -1, -1, Eigen::RowMajor> m(nrow, ncol);
    for (auto i = 0; i < nrow; ++i) {
        std::ranges::copy(lines[i], m.row(i).begin());
    }
    auto p1{0};
    auto p2{0};
    for (auto i = 0; i < m.rows(); ++i) {
        for (auto j = 0; j < m.cols(); ++j) {
            if (m(i, j) == 'X') { p1 += find_p1(m, i, j); }
            else if (m(i, j) == 'A') { p2 += find_p2(m, i, j); }
        }
    }
    return aoc::result(p1, p2);
}
