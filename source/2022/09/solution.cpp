#include <aoc.hpp>

namespace detail {
    using point = Eigen::Vector2i;

    template<std::size_t N /*number of knots*/>
    auto count_tail_positions(auto const& moves) {
        auto hash = [](point p) { return aoc::util::hash{}(std::span{p.data(), (std::size_t)p.size()}); };
        auto equal = [](point a, point b) { return (a.array() == b.array()).all(); };
        aoc::dense::set<point, decltype(hash), decltype(equal)> tailtrace;

        auto knots = [&]<auto... Idx>(std::index_sequence<Idx...>) {
            std::array<point, N> knots;
            ((knots[Idx] = point{0, 0}), ...);
            return knots;
        }(std::make_index_sequence<N>{});

        std::array<point, 4> cross { point{-1, +0}, {+0, -1}, {+1, +0}, {+0, +1} };
        std::array<point, 4> diag { point{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };

        using std::ranges::min_element;

        for (auto [m, s] : moves) {
            for (auto j = 0; j < s; ++j) {
                knots[0] += m;
                for (auto i = 1; i < N; ++i) {
                    auto const& h = knots[i-1];
                    auto& t = knots[i];
                    if ((h-t).template lpNorm<-1>() < 2) { break; }
                    auto l1 = [&](auto d) { return (h-t-d).template lpNorm<+1>(); };
                    t += *min_element(h[0] == t[0] || h[1] == t[1] ? cross : diag, std::less{}, l1);
                }
                tailtrace.insert(knots.back());
            }
        }
        return tailtrace.size();
    };
} // namespace detail

template<>
auto advent2022::day09() -> result {
    auto input = aoc::util::readlines("./source/2022/09/input.txt");
    using point = detail::point;
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

    constexpr auto knots_p1{2};
    constexpr auto knots_p2{10};

    auto part1 = detail::count_tail_positions<knots_p1>(moves);
    auto part2 = detail::count_tail_positions<knots_p2>(moves);
    return aoc::result(part1, part2);
}
