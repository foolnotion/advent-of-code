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
        bool moved{true};
        while(moved) {
            moved = false;
            for (auto j = 0; j < m.cols(); ++j) {
                for (auto i = 1; i < m.rows(); ++i) {
                    if (m(i, j) != 'O') { continue; }
                    for (auto k = i; k > 0 && m(k-1, j) == '.'; --k) {
                        moved = true;
                        std::swap(m(k, j), m(k-1, j));
                    }
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
} // namespace

template<>
auto advent2023::day14() -> result {
    auto input = aoc::util::readlines("./source/2023/14/input.txt");
    auto m = parse(input);

    // part 1
    tilt(m);
    auto const p1 = load(m);

    // part 2
    constexpr auto n{1'000'000'000}; // total cycles
    aoc::dense::map<u64, i64> seen;
    aoc::util::hash hash;
    bool found_cycle{false};
    for (auto i = 0L; i < n; ++i) {
        spin(m);
        if (found_cycle) { continue; }
        if (auto [it, ok] = seen.insert({ hash(m.reshaped()), i }); !ok) {
            found_cycle = true;
            auto const k{it->second};
            i = n - (n-k) % (i-k);
        }
    }
    auto const p2 = load(m);
    return aoc::result(p1, p2);
}