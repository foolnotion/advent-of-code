#include <aoc.hpp>
#include <util/dynamic_bitset.hpp>
#include <Eigen/Dense>

namespace {
    using point = Eigen::Vector2i;
    using vec3d = Eigen::Vector3d;

    struct grid {
        Eigen::Array<char, -1, -1> m;

        [[nodiscard]] inline auto remap(auto i, auto j) const {
            return std::tuple{aoc::math::mod_euclid(i, m.rows()), aoc::math::mod_euclid(j, m.cols())};
        }

        auto operator()(i32 i, i32 j) const -> char {
            auto [x, y] = remap(i, j);
            return m(x, y);
        }

        template<std::size_t N = 1>
        [[nodiscard]] auto reachable(std::array<i32, N> steps) const -> std::array<i32, N> {
            point s{m.rows()/2, m.cols()/2};
            std::deque<std::pair<point, i32>> queue{{s, 0}};

            auto neighbors = [&](point p) {
                return std::array<point, 4> {
                    point{ p[0]-1, p[1] }, { p[0]+1, p[1] }, { p[0], p[1]-1 }, { p[0], p[1]+1 }
                };
            };

            auto parity = [](auto x) { return x % 2; };
            std::array<i32, N> total{};

            auto const n{steps.back()};
            auto const d{2UL*n+1UL};
            sul::dynamic_bitset<u64> seen(d*d);

            while(!queue.empty()) {
                auto [p, t] = queue.front();
                queue.pop_front();

                auto k{(p(0)-s(0)+n) * d + (p(1)-s(1)+n)};
                if (seen.test_set(k)) { continue; }

                // total += t <= n && parity(t) == parity(n);
                for (auto i = 0; i < N; ++i) {
                    total[i] += t <= steps[i] && parity(t) == parity(steps[i]);
                }
                if (t+1 > n) { continue; }

                for (auto q : neighbors(p)) {
                    auto [x, y] = remap(q(0), q(1));
                    if (m(x, y) != '#') { queue.emplace_back(q, t+1); }
                }
            }
            return total;
        }
    };

    auto parse(auto const& input) {
        Eigen::Array<char, -1, -1> m(input.size(), input.front().size());
        for (auto i = 0; i < input.size(); ++i) {
            std::ranges::copy(input[i], m.row(i).begin());
        }
        return grid{m};
    }

    auto find_roots(Eigen::Vector3d xs, Eigen::Vector3d ys) -> Eigen::Vector3d {
        Eigen::Matrix3d vm;
        vm.col(0).setConstant(1);
        vm.col(1) = xs;
        vm.col(2) = xs.array().square();
        return vm.householderQr().solve(ys);
    }
} // namespace

template<>
auto advent2023::day21() -> result {
    auto input = aoc::util::readlines("./source/2023/21/input.txt");
    auto grid = parse(input);

    constexpr auto steps_p1{64L};
    auto const p1 = grid.reachable({steps_p1})[0];

    // part 2
    constexpr auto steps_p2{26501365L};
    constexpr auto t{(steps_p2 - 65L) / 131L};
    vec3d x{0, 1, 2};
    auto r{grid.reachable<3>({65L, 196L, 327L})};
    vec3d y{r[0], r[1], r[2]};
    vec3d roots = find_roots(x, y).array().round();
    auto const p2 = (vec3d::Constant(t).array().pow(x.array()) * roots.array()).sum();
    return aoc::result(p1, p2);
}