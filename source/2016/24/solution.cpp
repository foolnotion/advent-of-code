#include <aoc.hpp>

#define MDSPAN_IMPL_STANDARD_NAMESPACE std
#define MDSPAN_IMPL_PROPOSED_NAMESPACE experimental
#include <mdspan/mdspan.hpp>

template<>
auto advent2016::day24() -> result {
    using std::mdspan;
    using std::extents;

    auto input = aoc::util::readlines("./source/2016/24/input.txt");
    auto const nrows = input.size();
    auto const ncols = input.front().size();
    std::vector<char> loc;
    loc.reserve(nrows * ncols);

    for(auto const& line : input) {
        std::ranges::copy(line, std::back_inserter(loc));
    }

    using ext = extents<i32, std::dynamic_extent, std::dynamic_extent>;
    mdspan<char, ext> grid(loc.data(), nrows, ncols);
    using point = aoc::point<i32, 2>;

    // gather the positions of all the points of interest and the starting point
    std::vector<point> points;

    point start{1,1};
    auto constexpr zero{48};
    for (auto i = 0; i < grid.extent(0); ++i) {
        for (auto j = 0; j < grid.extent(1); ++j) {
            auto c = grid(i, j);
            if (c >= zero) {
                points.push_back({i, j});
            }
            if (c == '0') {
                start = {i,j};
            }
        }
    }

    auto get_neighbors = [](point p) {
        auto [x, y] = p;
        return std::array {
            point{x-1, y}, point{x+1, y}, point{x, y-1}, point{x, y+1}
        };
    };

    auto valid = [&](point p) {
        return p[0] >= 0 && p[0] < nrows && p[1] >= 0 && p[1] < ncols
            && grid(p[0], p[1]) != '#';
    };

    auto hash_pair = [](point a, point b) {
        if (a > b) { std::swap(a, b); }
        return aoc::util::hash{}(a[0], a[1], b[0], b[1]);
    };


    auto bfs = [&](point start, point end) {
        std::queue<std::pair<point, i32>> queue;
        queue.emplace( start, 0 );

        auto dmin{ std::numeric_limits<i32>::max() };
        aoc::dense::set<point> seen;

        while(!queue.empty()) {
            auto [p, d] = queue.front();
            queue.pop();

            if (auto [it, ok] = seen.insert(p); !ok) {
                continue;
            }

            if (p == end) {
                return d;
            }

            for (auto&& q : get_neighbors(p)) {
                if (!valid(q)) { continue; }
                queue.emplace(q, d+1);
            }
        }
        return dmin;
    };
    aoc::dense::map<u64, i32> dist;
    auto distance = [&](point a, point b) {
        auto it = dist.find(hash_pair(a, b));
        return it == dist.end() ? -1 : it->second;
    };

    for (auto i = 0; i < std::ssize(points)-1; ++i) {
        auto a = points[i];
        for (auto j = i+1; j < std::ssize(points); ++j) {
            auto b = points[j];
            auto h = hash_pair(a, b);
            auto d = bfs(a, b);
            dist[h] = d;
        }
    }

    cppsort::merge_sorter{}(points, [&](auto p) { return p != start; });
    auto part1{std::numeric_limits<i32>::max()};
    auto part2{std::numeric_limits<i32>::max()};

    std::span obj{points.begin()+1, points.end()};
    while(std::next_permutation(obj.begin(), obj.end())) {
        auto d = dist[hash_pair(start, obj.front())];
        for (auto i = 0; i < obj.size()-1; ++i) {
            d += dist[hash_pair(obj[i], obj[i+1])];
        }
        part1 = std::min(part1, d);
        part2 = std::min(part2, d + dist[hash_pair(start, obj.back())]);
    }

    return aoc::result(part1, part2);
}