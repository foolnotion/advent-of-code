#include <aoc.hpp>

namespace detail {
    using point = Eigen::Array2i;

    auto parse(auto const& input, bool part1) {
        using point = Eigen::Array2i;
        std::vector<std::vector<point>> points;
        points.reserve(input.size());

        point pmin, pmax; // NOLINT
        pmin.setConstant(std::numeric_limits<i32>::max());
        pmax.setConstant(std::numeric_limits<i32>::min());

        point sand_origin{500, 0}; // NOLINT
        pmin = (pmin < sand_origin).select(pmin, sand_origin);
        pmax = (pmax > sand_origin).select(pmax, sand_origin);

        for (auto const& line : input) {
            points.push_back(lz::map(lz::split(line, " -> "), [&](auto s) {
                point p;
                (void)scn::scan(s, "{},{}", p[0], p[1]);
                pmin = (pmin < p).select(pmin, p);
                pmax = (pmax > p).select(pmax, p);
                return p;
            }).toVector());
        }
        sand_origin -= pmin;

        Eigen::Array<char, -1, -1> cave(pmax[0] - pmin[0] + 1, pmax[1] - pmin[1] + 1);
        cave.setConstant('.');

        for (auto const& vec : points) {
            for (auto i = 0; i < std::ssize(vec)-1; ++i) {
                auto p = vec[i] - pmin;
                auto q = vec[i+1L] - pmin;
                if (p[1] == q[1]) {
                    auto seg = cave.col(p[1]).segment(std::min(p[0], q[0]), std::abs(p[0]-q[0])+1);
                    seg = (seg == '.').select('#', seg);
                } else if (p[0] == q[0]) {
                    auto seg = cave.row(p[0]).segment(std::min(p[1], q[1]), std::abs(p[1]-q[1])+1);
                    seg = (seg == '.').select('#', seg);
                }
            }
        }

        // AoC always uses an inversed coordinate system so we transpose the matrix
        cave.transposeInPlace();
        std::swap(sand_origin[0], sand_origin[1]);

        if (!part1) {
            auto nrow = pmax[1] + 2;
            auto ncol = (nrow + std::abs(sand_origin[1] - cave.cols()/2)) * 2;
            decltype(cave) cave2(nrow, ncol);
            cave2.setConstant('.');
            auto offset = (cave2.cols() - cave.cols())/2;
            sand_origin[1] += (cave2.cols() - cave.cols())/2;
            cave2.block(0, offset, cave.rows(), cave.cols()) = cave;
            cave = std::move(cave2);
        }
        return std::tuple{std::move(cave), sand_origin};
    }

    auto drop(auto& cave, point p, bool part1) {
        auto const nrow{cave.rows()};
        auto const ncol{cave.cols()};
        if (part1 && p[1] == 0 || p[1] == ncol-1) {
            return false;
        }
        while(p[0] < nrow-1 && cave(p[0]+1L, p[1]) == '.') { ++p[0]; }
        auto is_valid = [&](point p) {
            auto [x, y] = std::tuple{p[0], p[1]};
            return x >= 0 && y >= 0 && x < nrow && y < ncol && cave(x, y) == '.';
        };
        if (auto x = p + point{+1, -1}; is_valid(x)) { return drop(cave, std::move(x), part1); }
        if (auto x = p + point{+1, +1}; is_valid(x)) { return drop(cave, std::move(x), part1); }
        cave(p[0], p[1]) = 'o';
        return true;
    }

    auto simulate(auto& cave, point sand, bool part1) {
        while (cave(sand[0], sand[1]) != 'o' && detail::drop(cave, sand, part1)) { }
        return (cave == 'o').count();
    };


} // namespace detail

template<>
auto advent2022::day14() -> result {
    auto input = aoc::util::readlines("./source/2022/14/input.txt");
    auto [cave1, sand1] = detail::parse(input, /*part1=*/true);
    auto [cave2, sand2] = detail::parse(input, /*part1=*/false);
    auto part1 = detail::simulate(cave1, sand1, /*part1=*/true);
    auto part2 = detail::simulate(cave2, sand2, /*part1=*/false);
    return aoc::result(part1, part2);
}
