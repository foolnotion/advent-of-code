#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

namespace {
    auto parse(auto const& input) {
        Eigen::Array<char, -1, -1> arr(std::ssize(input), std::ssize(input.front()));
        for (auto i = 0; i < arr.rows(); ++i) {
            std::ranges::copy(input[i], arr.row(i).begin());
        }
        return arr;
    }

    auto tilt(auto& m) {
        for (auto j = 0; j < m.cols(); ++j) {
            for (auto i = 1; i < m.rows(); ++i) {
                if (m(i, j) != 'O') { continue; }
                for (auto k = i; k > 0 && m(k-1, j) == '.'; --k) {
                    std::swap(m(k, j), m(k-1, j));
                }
            }
        }
    };

    auto spin(auto& m) {
        for (auto i = 0; i < 4; ++i) {
            tilt(m);
            m = m.transpose().rowwise().reverse().eval();
        }
    };

    auto load(auto const& m) {
        auto f = [&](auto i) { return (m.rows()-i) * (m.row(i) == 'O').count(); };
        return fp::transform_reduce(f, std::plus{}, 0L, fp::numbers(0L, m.rows()));
    };

    auto spin_cycle(auto m, auto n) {
        aoc::dense::map<u64, std::pair<i64, i64>> seen;
        aoc::util::hash hash;
        for (auto i = 0L; i < n; ++i) {
            if (auto [it, ok] = seen.insert({ hash(m.data(), m.size()), { i, load(m) } }); !ok) {
                auto [j, _] = it->second;
                return seen.values()[j + (n-j) % (i-j)].second.second;
            }
            spin(m);
        }
        return load(m);
    }
} // namespace

template<>
auto advent2023::day14() -> result {
    auto input = aoc::util::readlines("./source/2023/14/input.txt");
    auto m = parse(input);

    tilt(m);
    auto const p1 = load(m);

    constexpr auto n{1'000'000'000};
    auto const p2 = spin_cycle(m, n);
    return aoc::result(p1, p2);
}
