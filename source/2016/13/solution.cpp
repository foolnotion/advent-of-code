#include <aoc.hpp>

namespace detail {
    using point = aoc::point<i32, 2>;
    using bitset = std::bitset<std::numeric_limits<u64>::digits>;

    auto is_open_space(point p, auto n) {
        auto [x, y] = p;
        auto v = x*x + 3*x + 2*x*y + y + y*y;
        return bitset(v + n).count() % 2 == 0;
    }

    auto get_neighbors(auto const& map, point p) {
        auto [x, y] = p;
        std::vector<point> neighbors;
        for (auto i : {x-1, x+1}) {
            if (i < 0 || i >= map.rows()) { continue; }
            if (map(i, y) == '#') { continue; }
            neighbors.push_back({i, y});
        }
        for (auto i : {y-1, y+1}) {
            if (i < 0 || i >= map.cols()) { continue; }
            if (map(x, i) == '#') { continue; }
            neighbors.push_back({x, i});
        }
        return neighbors;
    }
} // namespace detail

template<>
auto advent2016::day13() -> result {
    using detail::point;
    constexpr auto input{1364};
    constexpr point goal{39, 31};
    constexpr auto radius{50};
    constexpr auto dim = std::max(goal[0], goal[1]);
    Eigen::Array<char, dim+dim, dim+dim> map;
    map.setConstant(0);
    for (auto i = 0; i < map.rows(); ++i) {
        for (auto j = 0; j < map.cols(); ++j) {
            map(i, j) = detail::is_open_space({i,j}, input) ? '.' : '#';
        }
    }
    map.transposeInPlace();
    aoc::dense::set<point> seen;
    Eigen::Array<u32, map.rows(), map.cols()> arr;
    arr.setConstant(std::numeric_limits<u32>::max());
    arr(1,1) = 0;
    std::queue<point> queue;
    queue.push({1,1});
    while(!queue.empty()) {
        auto p = queue.front(); queue.pop();
        if (auto [it, ok] = seen.insert(p); !ok) { continue; }
        auto v = arr(p[0], p[1]);
        for (auto [x, y] : detail::get_neighbors(map, p)) {
            arr(x, y) = std::min(v+1, arr(x, y));
            queue.push({x, y});
        }
    }
    auto part1 = arr(goal[0], goal[1]);
    auto part2 = (arr <= radius).count();
    return aoc::result(part1, part2);
}
