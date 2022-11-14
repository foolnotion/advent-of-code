#include <aoc.hpp>
#include <iostream>

using Eigen::Array;
using Eigen::Map;
using point = aoc::point<i32>;

namespace detail {
struct count_occupied_p1 {
    auto operator()(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        for (int i = x-1; i <= x+1; ++i) {
            for (int j = y-1; j <= y+1; ++j) {
                if (i == x && j == y) { continue; }
                n += (i >= 0 && j >= 0 && i < a.rows() && j < a.cols()) && a(i, j) == '#';
            }
        }
        return n;
    }
};

struct count_occupied_p2 {
    auto count_nw(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(--i >= 0 && --j >= 0) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_ww(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(--j >= 0) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_sw(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(++i < a.rows() && --j >= 0) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_ss(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(++i < a.rows()) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_se(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(++i < a.rows()  && ++j < a.cols()) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_ee(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(++j < a.cols()) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_ne(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(--i >= 0 && ++j < a.cols()) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto count_nn(auto const& a, point p) {
        auto [x, y] = p;
        int i = x;
        int j = y;
        int n = 0;
        while(--i >= 0) {
            if(a(i, j) == '.') { continue; }
            n += a(i, j) == '#';
            break;
        }
        return n;
    }

    auto operator()(auto const& a, point p) { // NOLINT
        return count_nw(a, p) + count_ww(a, p) +
               count_sw(a, p) + count_ss(a, p) +
               count_se(a, p) + count_ee(a, p) +
               count_ne(a, p) + count_nn(a, p);
    }
};
} // namespace detail


template<>
auto advent2020::day11() -> result {
    auto input = aoc::util::readlines("./source/2020/11/input.txt");
    auto const nrow{std::ssize(input)};
    auto const ncol{std::ssize(input.front())};
    Array<char, -1, -1> a(nrow, ncol);

    for(auto&& [i, s] : lz::enumerate(input)) {
        a.row(i) = Map<Array<char, -1, 1>>(s.data(), ncol);
    }
    constexpr int s1{4};
    constexpr int s2{5};

    auto simulate = [](auto mat, auto&& count_occupied, int p) {
        auto changed{false};
        decltype(mat) aux;
        do {
            changed = false;
            aux = mat;
            for(int i = 0; i < mat.rows(); ++i) {
                for (int j = 0; j < mat.cols(); ++j) {
                    if (mat(i, j) == '.') { continue; }
                    auto oc = count_occupied(mat, {i, j});
                    if (mat(i, j) == 'L' && oc == 0) {
                        aux(i, j) = '#';
                        changed = true;
                    }
                    if (mat(i, j) == '#' && oc >= p) {
                        aux(i, j) = 'L';
                        changed = true;
                    }
                }
            }
            mat = aux;
        } while (changed);
        return (mat == '#').count();
    };

    auto p1 = simulate(a, detail::count_occupied_p1{}, s1);
    auto p2 = simulate(a, detail::count_occupied_p2{}, s2);
    return aoc::result(p1, p2);
}
