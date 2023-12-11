#include <aoc.hpp>

namespace {
    auto parse(auto const& input) {
        auto const nr{std::ssize(input)};
        auto const nc{std::ssize(input.front())};

        Eigen::Array<char, -1, -1, Eigen::RowMajor> mat(nr, nc);
        for (auto i = 0; i < nr; ++i) {
            std::ranges::copy(input[i], mat.row(i).begin());
        }
        return mat;
    }
} // namespace

template<>
auto advent2023::day11() -> result {
    auto input = aoc::util::readlines("./source/2023/11/input.txt");
    auto m = parse(input);

    std::vector<Eigen::Array<i64, 2, 1>> galaxies;
    for (auto i = 0; i < m.rows(); ++i) {
        for (auto j = 0; j < m.cols(); ++j) {
            if (m(i, j) == '#') {
                galaxies.emplace_back(i, j);
            }
        }
    }

    Eigen::Array<i64, -1, -1> distance = decltype(distance)::Constant(m.rows(), m.cols(), 1);

    constexpr auto d1{2L};
    for (auto i = 0; i < m.rows(); ++i) {
        if ((m.row(i) == '.').all()) {
            distance.row(i).setConstant(d1);
        }
    }
    for (auto i = 0; i < m.cols(); ++i) {
        if ((m.col(i) == '.').all()) {
            distance.col(i).setConstant(d1);
        }
    }

    auto shortest_distance = [&](auto p, auto q) {
        return distance.col(q(1))(Eigen::seq(std::min(p(0), q(0)), std::max(p(0), q(0))-1)).sum() +
               distance.row(q(0))(Eigen::seq(std::min(p(1), q(1)), std::max(p(1), q(1))-1)).sum();
    };

    auto sum_distance = [&](auto const& galaxies) {
        auto sum{0L};
        for (auto i = 0; i < std::ssize(galaxies) - 1; ++i) {
            for (auto j = i + 1; j < std::ssize(galaxies); ++j) {
                sum += shortest_distance(galaxies[i], galaxies[j]);
            }
        }
        return sum;
    };
    auto const p1 = sum_distance(galaxies);

    constexpr auto d2{1'000'000L};
    distance = (distance == d1).select(d2, distance);
    auto const p2 = sum_distance(galaxies);

    return aoc::result(p1, p2);
}