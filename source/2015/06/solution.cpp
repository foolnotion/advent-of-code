#include <aoc.hpp>

template <>
auto advent2015::day06() -> result
{
    constexpr i64 width { 1000 };
    constexpr i64 height { 1000 };

    Eigen::Array<u32, -1, -1> grid = decltype(grid)::Zero(width, height);

    std::fstream f("./source/2015/06/input.txt");
    std::vector<std::string> input;

    for (std::string s; std::getline(f, s);) {
        input.push_back(s);
    }

    auto setup_lights = [&](bool part1) -> void {
        for (auto const& s : input) {
            if (s[1] == 'u' /* turn on/off */) {
                if (s[6] == 'n') { // NOLINT
                    auto [x1, y1, x2, y2] = scn::scan<i64, i64, i64, i64>(s, "turn on {},{} through {}, {}")->values();
                    auto block = grid.block(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
                    if (part1) { block = 1; }
                    else       { block += 1; }
                } else if (s[6] == 'f') { // NOLINT
                    auto [x1, y1, x2, y2] = scn::scan<i64, i64, i64, i64>(s, "turn off {},{} through {}, {}")->values();
                    auto block = grid.block(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
                    if (part1) { block = 0; }
                    else       { block = (block > 0).select(block-1, 0); }
                }
            } else if (s[1] == 'o' /* toggle */) {
                auto [x1, y1, x2, y2] = scn::scan<i64, i64, i64, i64>(s, "toggle {},{} through {}, {}")->values();
                auto block = grid.block(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
                if (part1) { block = (block == 0).select(2, block) - 1; }
                else       { block += 2; }
            }
        }
    };

    setup_lights(true);
    auto p1 = grid.sum();
    grid.setConstant(0);
    setup_lights(false);
    auto p2 = grid.sum();
    return aoc::result(p1, p2);
}
