#include <aoc.hpp>
#include <thread>

using namespace std::chrono_literals;

namespace {
    using point = Eigen::Vector2i;

    template<std::size_t N>
    auto trace_tail (auto const& moves) -> u64 {
        auto hash = [](point p) { return aoc::util::hash{}(p[0], p[1]); };
        aoc::dense::set<point, decltype(hash)> tailtrace;

        std::array<point, N> k;
        k.fill({0,0});

        for (auto [m, s] : moves) {
            for (auto j = 0; j < s; ++j) {
                k[0] += m;
                for (auto i = 1; i < N; ++i) {
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
    std::array<point, 'R'-'A'+1> move;
    move['L'-'A'] = {0, -1};
    move['R'-'A'] = {0, +1};
    move['U'-'A'] = {-1, 0};
    move['D'-'A'] = {+1, 0};

    auto moves = lz::map(input, [&](auto const& line) {
        auto c = line[0];
        auto s = scn::scan_value<i32>(line.substr(2, line.size()-2)).value();
        return std::tuple{move[c-'A'], s};
    }).toVector();

    auto const part1 = trace_tail<2>(moves);  // NOLINT
    auto const part2 = trace_tail<10>(moves); // NOLINT
    return aoc::result(part1, part2);
}
