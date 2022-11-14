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

    auto setup_lights = [&](bool part1) {
        for (auto const& s : input) {
            i64 x1{0}, x2{0}, y1{0}, y2{0}; // NOLINT

            if (s[1] == 'u' /* turn on/off */) {
                if (s[6] == 'n') { // NOLINT
                    (void)scn::scan(s, "turn on {},{} through {},{}", x1, y1, x2, y2);
                    auto block = grid.block(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
                    if (part1) { block = 1; }
                    else       { block += 1; }
                } else if (s[6] == 'f') { // NOLINT
                    (void)scn::scan(s, "turn off {},{} through {},{}", x1, y1, x2, y2);
                    auto block = grid.block(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
                    if (part1) { block = 0; }
                    else       { block = (block > 0).select(block-1, 0); }
                }
            } else if (s[1] == 'o' /* toggle */) {
                (void)scn::scan(s, "toggle {},{} through {},{}", x1, y1, x2, y2);
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
