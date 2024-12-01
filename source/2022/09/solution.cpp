#include <aoc.hpp>

namespace {
    using point = Eigen::Vector2i;

    template<std::size_t N>
    auto trace_tail (auto const& moves) -> u64 {
        auto hash = [](point p) { return aoc::util::hash{}(p[0], p[1]); };
        aoc::dense::set<point, decltype(hash)> tailtrace;

        std::array<point, N> k;
        k.fill({0,0});

        for (auto const& [m, s] : moves) {
            for (auto j = 0UL; j < s; ++j) {
                k[0] += m;
                for (auto i = 1UL; i < N; ++i) {
                    if ((k[i-1] - k[i]).template lpNorm<-1>() < 2) { break; }
                    k[i] += (k[i-1] - k[i]).cwiseSign();
                }
                tailtrace.insert(k.back());
            }
        }
        return tailtrace.size();
    }
} // namespace

template<>
auto advent2022::day09() -> result {
    auto input = aoc::util::readlines("./source/2022/09/input.txt");
    std::vector<std::tuple<point, u64>> moves;
    for (auto const& line : input) {
        auto c = line[0];
        auto s = scn::scan_value<i32>(line.substr(2, line.size()-2))->value();
        point p;
        switch(c) {
            case 'L': { p << 0, -1; break; }
            case 'R': { p << 0, +1; break; }
            case 'U': { p << -1, 0; break; }
            case 'D': { p << +1, 0; break; }
            default: { throw std::runtime_error(fmt::format("invalid move key: {}", c));}
        }
        moves.emplace_back(p, s);
    }
    auto part1 = trace_tail<2>(moves);  // NOLINT
    auto part2 = trace_tail<10>(moves); // NOLINT
    return aoc::result(part1, part2);
}
