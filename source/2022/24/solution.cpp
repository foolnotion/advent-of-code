#include <aoc.hpp>

namespace {
using point = Eigen::Array2i;

template <typename Hash, typename Equal>
struct valley {
    i64 nrow {};
    i64 ncol {};

    std::vector<std::tuple<point, char>> blizzards;
    Eigen::Array<u8, -1, -1> blizzard_positions;

    auto step() {
        decltype(blizzards) aux;
        aux.reserve(blizzards.size());

        for (auto const& [p, c] : blizzards) {
            point q = p;
            switch (c) {
            case '^': {
                q[0] = p[0] > 1 ? p[0]-1 : nrow-2;
                aux.emplace_back(q, c);
                break;
            }
            case 'v': {
                q[0] = p[0] < nrow-2 ? p[0]+1 : 1;
                aux.emplace_back(q, c);
                break;
            }
            case '<': {
                q[1] = q[1] > 1 ? q[1]-1 : ncol-2;
                aux.emplace_back(q, c);
                break;
            }
            case '>': {
                q[1] = q[1] < ncol-2 ? q[1]+1 : 1;
                aux.emplace_back(q, c);
                break;
            }
            default: {
                break;
            }
            }
        }
        std::swap(blizzards, aux);
        // update cache
        blizzard_positions = decltype(blizzard_positions)::Zero(nrow, ncol);
        for (auto const& [p, c] : blizzards) {
            blizzard_positions(p[0], p[1]) = 1;
        }
    }

    [[nodiscard]] auto valid_dest(auto const& q) const {
        if (q[0] < 1 || q[0] > nrow-2 || q[1] < 1 || q[1] > ncol-2) { return false; }
        return !blizzard_positions(q[0], q[1]);
    }
}; // valley

auto fastest_route(auto valley, auto start, auto end) -> std::tuple<decltype(valley), i32> {
    aoc::dense::map<i32, decltype(valley)> cache;
    cache.insert({0, valley});

    using item = std::tuple<point, i32, i32>;
    auto cmp = [](auto const& a, auto const& b) {
        return std::get<2>(a) > std::get<2>(b);
    };

    std::priority_queue<item, std::vector<item>, decltype(cmp)> queue;
    queue.push({start, 0, (start-end).matrix().template lpNorm<1>() });

    std::array const dir{ point{-1, 0}, point{+1, 0}, point{0, -1}, point{0, +1} };
    aoc::dense::set<u64> seen;

    auto best{std::numeric_limits<i32>::max()};
    while(!queue.empty()) {
        auto [p, m, c] = queue.top();
        queue.pop();

        if (c >= best) { continue; }

        if ((p == end).all()) {
            best = std::min(m, best);
            continue;
        }

        auto h = aoc::util::hash{}(p[0], p[1], m);
        if (auto [it, ok] = seen.insert(h); !ok) {
            continue;
        }

        if (!cache.contains(m+1)) {
            auto u = cache[m];
            u.step();
            cache[m+1] = std::move(u);
        }

        auto const& u = cache[m+1];
        if (!(u.blizzard_positions(p[0], p[1]))) {
            queue.push({p, m+1, c+1});
        }

        for (auto const& r : dir) {
            point q = p + r;
            if (!((q == start).all() || (q == end).all() || u.valid_dest(q))) {
                continue;
            }
            auto const d{ (p - end).matrix().template lpNorm<1>() };
            queue.push({ q, m + 1, m + d });
        }
    }
    return { std::move(cache[best]), best };
}
} // namespace

template <>
auto advent2022::day24() -> result
{
    auto input = aoc::util::readlines("./source/2022/24/input.txt");

    auto const nrow { std::ssize(input) };
    auto const ncol { std::ssize(input.front()) };

    using point = Eigen::Array2i;

    auto hash = [](auto const& p) { return aoc::util::hash {}(p[0], p[1]); };
    auto eql = [](auto const& a, auto const& b) { return (a == b).all(); };

    valley<decltype(hash), decltype(eql)> v0{nrow, ncol};
    for (auto&& [i, line] : lz::enumerate(input)) {
        for (auto&& [j, c] : lz::enumerate(line)) {
            if (aoc::equals<'>', 'v', '<', '^'>(c)) {
                v0.blizzards.emplace_back(point{ i, j }, c);
            }
        }
    }

    point const start{0, 1};
    point const end{nrow-1, ncol-2};

    auto [v1, t1] = fastest_route(v0, start, end);
    auto [v2, t2] = fastest_route(v1, end, start); // NOLINT
    auto [v3, t3] = fastest_route(v2, start, end);
    return aoc::result(t1, t1 + t2 + t3);
}
