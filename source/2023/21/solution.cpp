#include <aoc.hpp>
#include <experimental/mdspan>
#include <Eigen/Dense>

namespace rs = std::ranges;

using point   = aoc::point<i32, 2>;
using extents = std::experimental::extents<i32, std::dynamic_extent, std::dynamic_extent>;
using mdspan  = std::experimental::mdspan<char, extents>;

namespace {
    auto parse(auto const& input) {
        std::vector<char> buf;
        buf.reserve(input.size()* input.front().size());
        auto copy = [&](auto const& s) { rs::copy(s, std::back_inserter(buf)); };
        rs::for_each(input, copy);
        return buf;
    }
} // namespace

template<>
auto advent2023::day21() -> result {
    auto input = aoc::util::readlines("./source/2023/21/input.txt");
    auto const nrows{input.size()};
    auto const ncols{input.front().size()};
    auto buf = parse(input);
    mdspan grid(buf.data(), nrows, ncols);

    auto s = rs::find(buf, 'S');
    auto x = std::distance(buf.begin(), s) / ncols;
    auto y = std::distance(buf.begin(), s) % ncols;
    point p{static_cast<i32>(x), static_cast<i32>(y)};

    auto remap = [&](point p) -> point {
        auto mod = [](auto a, auto b) -> i32 {
            while(a < 0) { a += b; }
            return a % b;
        };
        return point{mod(p[0], nrows), mod(p[1], ncols)};
    };

    auto valid = [&](point p) -> bool {
        auto [x, y] = remap(p);
        return x >= 0 && x < nrows && y >= 0 && y < ncols && aoc::equals<'.', 'S'>(grid(x, y));
    };

    auto neighbors = [&](point p) {
        return std::array {
            point { p[0]-1, p[1] },
            point { p[0]+1, p[1] },
            point { p[0], p[1]-1 },
            point { p[0], p[1]+1 }
        };
    };

    auto hash = [](auto p) {
        // std::array a{p.first, p.second[0], p.second[1]};
        // return aoc::util::hash{}(a);
        auto [c, q] = p;
        return aoc::util::hash{}(c, q[0], q[1]);
    };

    std::vector<i64> dist(nrows * ncols, 0L);
    std::experimental::mdspan<i64, extents> distance(dist.data(), nrows, ncols);

    // part 1
    constexpr auto steps{64};
    auto bfs = [&](auto n) {
        auto count{0UL};
        aoc::dense::set<std::pair<i32, point>, decltype(hash)> seen;
        std::deque<std::pair<i64, point>> queue;
        queue.emplace_back(0, p);

        auto hits{0UL};
        while(!queue.empty()) {
            auto [c, q] = queue.front();
            queue.pop_front();

            if (c > n) { continue; }

            auto r = remap(q);
            // auto& d = distance()
            if (auto [it, ok] = seen.insert({c, q}); !ok) {
                ++hits;
                continue;
            }

            count += c == n;

            if (c < n) {
                for (auto r : neighbors(q) | std::views::filter(valid)) {
                    queue.emplace_back(c+1, r);
                }
            }
        }
        fmt::print("seen: {}, hits: {}\n", seen.size(), hits);
        return count;
    };
    auto const p1 = bfs(steps);

    // part 2
    constexpr auto steps_p2{26501365L};
    constexpr auto target{(steps_p2 - 65) / 131};
    Eigen::Vector3d xs({0, 1, 2});
    Eigen::Vector3d ys(3);

    // steps ncols/2, ncols/2+ncols, ncols/2+2*ncols
    rs::transform(std::array{65, 196, 327}, ys.data(), bfs);

    // construct vandermonde matrix
    Eigen::Matrix3d vm;
    vm.col(0).setConstant(1);
    vm.col(1) = xs;
    vm.col(2) = xs.array().square();
    Eigen::Vector3d roots = vm.householderQr().solve(ys);

    auto f = [&](auto i) -> i64 {
        return static_cast<i64>(std::round(roots(i))) * std::pow(target, i);
    };
    constexpr std::array idx{0, 1, 2};
    auto const p2 = std::transform_reduce(idx.begin(), idx.end(), 0UL, std::plus{}, f);

    return aoc::result(p1, p2);
}