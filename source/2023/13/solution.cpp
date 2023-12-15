#include <aoc.hpp>
#include <fplus/container_common.hpp>
#include <fplus/fplus.hpp>

namespace {
    using matrix = Eigen::Array<char, -1, -1>;

    auto parse(auto const& input) {
        std::vector<matrix> v;
        for (auto i = 0; i < std::ssize(input); ++i) {
            auto j = i;
            while(j < std::ssize(input) && !input[j].empty()) { ++j; }
            matrix m(j-i, std::ssize(input[i]));
            for (auto k = i; k < j; ++k) {
                std::ranges::copy(input[k], m.row(k-i).begin());
            }
            v.push_back(m);
            i = j;
        }
        return v;
    }

    auto find_horizontal(matrix const& m, bool part2 = false) {
        auto p{0L};
        auto t{0L};

        for (auto k = 0L; k < m.rows()-1; ++k) {
            auto s = std::min(k+1, m.rows()-(k+1));
            auto a = m.block(k+1-s, 0, s, m.cols());
            auto b = m.block(k+1, 0, s, m.cols());

            auto c = (a.colwise().reverse() == b).count();
            if (part2 && c == a.size()-1) {
                return k+1;
            }
            if (!part2 && c == a.size()) {
                if(s > p) {
                    p = s;
                    t = k+1;
                }
            }
        }
        return t;
    };

    auto find_vertical(matrix m, bool part2 = false) {
        auto p{0L};
        auto t{0L};

        for (auto k = 0L; k < m.cols()-1; ++k) {
            auto s = std::min(k+1, m.cols()-(k+1));
            auto a = m.block(0, k+1-s, m.rows(), s);
            auto b = m.block(0, k+1, m.rows(), s);

            auto c = (a.rowwise().reverse() == b).count();
            if (part2 && c == a.size()-1) {
                return k+1;
            }
            if (!part2 && c == a.size()) {
                if(s > p) {
                    p = s;
                    t = k+1;
                }
            }
        }
        return t;
    };

    template<bool P2>
    struct summary {
        auto operator()(auto const& m) {
            auto sum { 0L };
            auto constexpr h { 100 };
            auto k = find_horizontal(m, P2);
            return k ? h * k : find_vertical(m, P2);
        }
    };
} // namespace

template<>
auto advent2023::day13() -> result {
    auto input = aoc::util::readlines("./source/2023/13/input.txt");
    auto mirrors = parse(input);
    auto const p1 = fplus::fold_left(std::plus{}, 0L, fplus::transform(summary<false>{}, mirrors));
    auto const p2 = fplus::fold_left(std::plus{}, 0L, fplus::transform(summary<true>{}, mirrors));
    return aoc::result(p1, p2);
}