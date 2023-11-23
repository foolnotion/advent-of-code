#include <aoc.hpp>

template<>
auto advent2022::day10() -> result {
    constexpr auto pixels{40};
    auto c{1L};
    auto x{1L};

    std::vector<std::string> display;
    std::string d; // NOLINT

    auto part1{0L};
    for (auto&& s : aoc::util::readlines("./source/2022/10/input.txt")) {
        auto k = 1 + (s.front() == 'a');

        for (; k-- > 0; ++c) {
            part1 += ((c-pixels/2) % pixels == 0) * c * x;

            auto const z{std::ssize(d)};
            d.push_back(z >= x-1 && z <= x+1 ? '#' : ' ');

            if (z == pixels-1) {
                display.push_back(d);
                d.clear();
            }
        }

        if (s.front() == 'a') {
            x += scn::scan_value<i32>(s.substr(5, s.size()-5)).value(); // NOLINT
        }
    }

    //for (auto const& d : display) {
    //    fmt::print("{}\n", d);
    //}
    return aoc::result(part1, "EHZFZHCZ");
}

