#include <aoc.hpp>

template<>
auto advent2018::day03() -> result {
    auto lines = aoc::util::readlines("source/2018/03/input.txt");
    auto nrow{0};
    auto ncol{0};

    using claim = std::tuple<i32, i32, i32, i32, i32>;
    std::vector<claim> claims;
    claims.reserve(lines.size());

    for (auto const& line : lines) {
        auto [n, x, y, w, h] = scn::scan<i32, i32, i32, i32, i32>(line, "#{} @ {},{}: {}x{}")->values();
        nrow = std::max(nrow, y+h);
        ncol = std::max(ncol, x+w);
        claims.emplace_back(n, x, y, w, h);
    }

    Eigen::Array<i32, -1, -1> m = decltype(m)::Zero(nrow, ncol);
    for (auto [n, x, y, w, h] : claims) {
        m.block(x, y, w, h) += 1;
    }

    auto const p1 = (m > 1).count();
    auto const p2 = std::get<0>(*std::ranges::find_if(claims, [&](auto t) {
        auto [n, x, y, w, h] = t;
        return (m.block(x, y, w, h) == 1).all();
    }));

    return aoc::result(p1, p2);
}