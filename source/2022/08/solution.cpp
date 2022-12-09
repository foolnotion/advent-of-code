#include <aoc.hpp>

template<>
auto advent2022::day08() -> result {
    auto input = aoc::util::readlines("./source/2022/08/input.txt");
    auto const nrow{std::ssize(input)};
    auto const ncol{std::ssize(input.front())};
    Eigen::Array<u8, -1, -1> map(nrow, ncol);
    for (auto&& [i, line] : lz::enumerate(input)) {
        map.row(i) = (Eigen::Map<Eigen::Array<char, -1, 1>>(line.data(), ncol) - '0').cast<u8>();
    }

    decltype(map) vis(nrow, ncol);
    vis.setConstant(0);

    auto edge = [&](auto x, auto y) {
        return x == 0 || y == 0 || x == nrow-1 || y == ncol-1;
    };

    auto update_vis = [&](auto&& range_x, auto&& range_y, auto&& get_val, auto&& get_xy) {
        for (auto x : range_x) {
            auto v = get_val(x);
            for (auto y : range_y) {
                auto [a, b] = get_xy(x, y);
                vis(a, b) = vis(a, b) || edge(a, b) || v < map(a, b);
                v = std::max(v, map(a, b));
            }
        }
    };

    auto scenic_score = [&](auto x, auto y) {
        if (edge(x, y)) { return 0U; }
        u32 a{}, b{}, c{}, d{}; // NOLINT
        auto v{map(x,y)};
        for (auto i = x-1; i >= 0; --i) { ++a; if (map(i, y) >= v) { break; } }
        for (auto i = y-1; i >= 0; --i) { ++c; if (map(x, i) >= v) { break; } }
        for (auto i = x+1; i < nrow; ++i) { ++b; if (map(i, y) >= v) { break; } }
        for (auto i = y+1; i < ncol; ++i) { ++d; if (map(x, i) >= v) { break; } }
        return a * b * c * d;
    };

    auto get_xy = [](auto x, auto y) { return std::tuple{x, y}; };
    update_vis(lz::range(nrow), lz::range(1L, ncol, 1L), [&](auto i) { return map(i, 0); }, get_xy);
    update_vis(lz::range(nrow), lz::range(ncol-2, -1L, -1L), [&](auto i) { return map(i, ncol-1); }, get_xy);

    auto get_yx = [](auto x, auto y) { return std::tuple{y, x}; };
    update_vis(lz::range(ncol), lz::range(1L, nrow, 1L), [&](auto i) { return map(0, i); }, get_yx);
    update_vis(lz::range(ncol), lz::range(nrow-2, -1L, -1L), [&](auto i) { return map(nrow-1, i); }, get_yx);
    auto part1 = (vis == 1).count();

    auto const prod = lz::cartesian(lz::range(nrow), lz::range(ncol));
    auto part2{0U};
    for (auto [x, y] : prod) { part2 = std::max(part2, scenic_score(x, y)); }
    return aoc::result(part1, part2);
}
