#include <aoc.hpp>

template<>
auto advent2020::day12() -> void {
    auto input = aoc::util::readlines("./source/2020/12/input.txt");
    std::vector<std::pair<char, int>> vec;

    for (auto const& line : input) {
        char dir{};
        int val{};
        (void)scn::scan(line, "{}{}", dir, val);
        vec.emplace_back(dir, val);
    }

    // part 1
    std::array ship { 0, 0, 0, 0 };
    u8 i{0}; // 0 - east, 1 - south, 2 - west, 3 - north

    for (auto [dir, val] : vec) {
        switch (dir) {
        case 'R': {
            i = (i + val / 90) % 4;
            break;
        }
        case 'L': {
            i = (i + (360 - val) / 90) % 4;
            break;
        }
        case 'F': {
            ship[i] += val;
            break;
        }
        case 'N': {
            ship[3] += val;
            break;
        }
        case 'S': {
            ship[1] += val;
            break;
        }
        case 'E': {
            ship[0] += val;
            break;
        }
        case 'W': {
            ship[2] += val;
            break;
        }
        default: {
            break;
        }
        }
    }
    fmt::print("part 1: {}\n", std::abs(ship[1]-ship[3]) + std::abs(ship[0]-ship[2]));

    // part 2
    std::array waypoint { 10, 0, 0, 1 };
    ship = { 0, 0, 0, 0 }; // reset ship position
    i = 0; // reset orientation to facing east

    for (auto [dir, val] : vec) {
        switch (dir) {
        case 'R': {
            // rotate the waypoint around the ship clockwise
            std::rotate(waypoint.rbegin(), waypoint.rbegin() + val / 90, waypoint.rend());
            break;
        }
        case 'L': {
            // rotate the waypoint around the ship counter-clockwise
            std::rotate(waypoint.begin(), waypoint.begin() + val / 90, waypoint.end());
            break;
        }
        case 'F': {
            for (int j = 0; j < 4; ++j) {
                ship[j] += val * waypoint[j];
            }
            break;
        }
        case 'N': {
            waypoint[3] += val;
            break;
        }
        case 'S': {
            waypoint[1] += val;
            break;
        }
        case 'E': {
            waypoint[0] += val;
            break;
        }
        case 'W': {
            waypoint[2] += val;
            break;
        }
        default: {
            break;
        }
        }
    }
    fmt::print("part 2: {}\n", std::abs(ship[1]-ship[3]) + std::abs(ship[0]-ship[2]));
}
