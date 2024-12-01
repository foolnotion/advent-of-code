#include <aoc.hpp>
#include <queue>

#define MDSPAN_IMPL_STANDARD_NAMESPACE std
#define MDSPAN_IMPL_PROPOSED_NAMESPACE experimental
#include <mdspan/mdspan.hpp>

namespace {
    using point = aoc::point<i32, 2>;

    struct node {
        point pos;
        i32 used{};
        i32 avail{};

        auto cp(i32 size) {
            EXPECT(avail >= size);
            avail -= size;
            used  += size;
        }

        auto rm(i32 size) {
            EXPECT(used >= size);
            avail += size;
            used  -= size;
        }

        friend auto operator<(node const& a, node const& b) {
            return a.pos < b.pos;
        }
    };

    auto parse_input(auto const& input) {
        std::vector<node> nodes;
        nodes.reserve(input.size());

        i32 xmax{0};
        i32 ymax{0};
        for (auto const& s : input) {
            if (s.front() != '/') { continue; }
            auto q = aoc::util::trim_extra_space(s);
            node n{};
            auto [x, y, size, used, avail, percent] = scn::scan<i32, i32, i32, i32, i32, i32>(q, "/dev/grid/node-x{}-y{} {}T {}T {}T {}%")->values();
            n.pos = point{x, y};
            n.used = used;
            n.avail = avail;
            xmax = std::max(xmax, n.pos[0]);
            ymax = std::max(ymax, n.pos[1]);
            nodes.push_back(n);
        }
        return std::make_tuple(std::move(nodes), xmax, ymax);
    }
} // namespace

template<>
auto advent2016::day22() -> result {
    using std::mdspan;
    using std::extents;

    auto input = aoc::util::readlines("./source/2016/22/input.txt");
    auto [nodes, xmax, ymax] = parse_input(input);

    cppsort::merge_sorter{}(nodes, &node::avail);
    auto find_viable = [&](node const& n) {
        auto res = std::ranges::partition_point(nodes, [&](auto const& x) { return x.avail < n.used; });
        return std::distance(res, nodes.end())-1;
    };
    auto part1 = std::transform_reduce(nodes.begin(), nodes.end(), 0UL, std::plus{}, find_viable);

    // -- part 2 --
    cppsort::merge_sorter{}(nodes);
    node empty_node = *std::ranges::find_if(nodes, [](auto const& n){ return n.used == 0; });

    using grid = mdspan<node, extents<i32, std::dynamic_extent, std::dynamic_extent>>;
    struct state {
        aoc::dense::map<point, node> changed;
        grid cluster;

        node empty;
        point target{};
        i32 steps{};

        state(grid c, node e, point t, i32 s)
            : cluster(c), empty(e), target(t), steps(s)
        { }

        [[nodiscard]] auto query(point p) const -> node const& {
            if (auto it = changed.find(p); it != changed.end()) {
                return it->second;
            }
            return cluster(p[0], p[1]);
        };

        auto operator()(point p) -> node& {
            auto [it, ok] = changed.insert({p, cluster(p[0], p[1])});
            return it->second;
        }

        [[nodiscard]] auto hash() const {
            return aoc::util::hash{}(empty.pos[0], empty.pos[1], target[0], target[1]);
        }
    };

    auto get_neighbors = [](point p) {
        auto [x, y] = p;
        std::array neighbors = {
            point{x-1, y},
            point{x+1, y},
            point{x, y-1},
            point{x, y+1}
        };
        return neighbors;
    };

    auto valid = [&](auto x, auto y) {
        return x >= 0 && x <= xmax && y >= 0 && y <= ymax;
    };

    grid g{nodes.data(), xmax+1, ymax+1};

    auto bfs = [&]() {
        auto result = std::numeric_limits<i32>::max();
        std::queue<state> queue;
        queue.emplace(g, empty_node, point{xmax, 0}, 0);
        aoc::dense::set<u64> visited;

        while (!queue.empty()) {
            auto s = queue.front();
            queue.pop();

            if (s.target == point{0,0}) { return s.steps; }

            if (auto [it, ok] = visited.insert(s.hash()); !ok) {
                continue;
            }

            for (auto [i, j] : get_neighbors(s.empty.pos)) {
                auto n = s.query({i, j});  // new empty node
                auto& m = s.empty;
                if (!valid(i, j) || n.used > m.avail) { continue; }
                n.rm(n.used);
                queue.emplace(g, n, s.target == point{i, j} ? m.pos : s.target, s.steps+1);
                queue.front()({i, j}) = n;
            }
        }
        return result;
    };

    auto part2 = bfs();
    return aoc::result(part1, part2);
}