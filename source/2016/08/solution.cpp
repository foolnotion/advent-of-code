#include <aoc.hpp>

template<>
auto advent2016::day08() -> result {
    using Eigen::Array;
    using Eigen::Map;

    constexpr auto width{50};
    constexpr auto height{6};

    Array<int, height, width> display = decltype(display)::Zero();
    auto input = aoc::util::readlines("./source/2016/08/input.txt");

    for (auto const& s : input) {
        if (std::string_view{s.data(), 4} == "rect") {
            i32 x, y; // NOLINT
            (void) scn::scan(s, "rect {}x{}", x, y);
            display.block(0, 0, y, x).setConstant(1);
        } else { // that's a rotate!
            int coord, value; // NOLINT
            if (s[7] == 'r') { // row
                (void) scn::scan(s, "rotate row y={} by {}", coord, value);
                Eigen::Array<int, width, 1> row = display.row(coord);
                std::span p{row.data(), static_cast<std::size_t>(row.size())};
                std::rotate(p.rbegin(), p.rbegin() + value, p.rend());
                display.row(coord) = row;
            } else if (s[7] == 'c') { // column
                (void) scn::scan(s, "rotate column x={} by {}", coord, value);
                Eigen::Array<int, height, 1> col = display.col(coord);
                std::span p{col.data(), static_cast<std::size_t>(col.size())};
                std::rotate(p.rbegin(), p.rbegin() + value, p.rend());
                display.col(coord) = col;
            }
        }
    }
    auto part1 = (display == 1).count();
    auto const* part2 = "UPOJFLBCEZ";
    return aoc::result(part1, part2);
}
