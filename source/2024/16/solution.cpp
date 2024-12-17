#include <aoc.hpp>

namespace {
    using point = aoc::point<i32>;
    using grid  = Eigen::Array<char, -1, -1, Eigen::RowMajor>;
    auto operator+(point const& a, point const& b) { return point{a[0]+b[0], a[1]+b[1]}; }
    auto operator-(point const& a) { return point{-a[0], -a[1]}; }
    struct dir {
        static constexpr point east{0, +1};
        static constexpr point west{0, -1};
        static constexpr point north{-1, 0};
        static constexpr point south{+1, 0};
    };
} // namespace

template<>
auto advent2024::day16() -> result {
    auto const lines = aoc::util::readlines("./source/2024/16/input.txt");
    auto [nrow, ncol] = std::tuple{ std::ssize(lines), std::ssize(lines.front()) };
    grid g(nrow, ncol);
    point start;
    point end;
    for (auto i = 0; i < nrow; ++i) {
        for (auto j = 0; j < ncol; ++j) {
            if (lines[i][j] == 'S') { start = {i, j}; }
            if (lines[i][j] == 'E') { end = {i, j}; }
            g(i, j) = lines[i][j];
        }
    }

    Eigen::Array<u8, -1, -1> seen = decltype(seen)::Zero(nrow, ncol);
    Eigen::Array<u32, -1, -1> cost(nrow, ncol);
    cost.setConstant(std::numeric_limits<u32>::max());

    constexpr std::array directions{dir::east, dir::west, dir::north, dir::south};
    constexpr auto turn_cost{1000};

    using state = std::tuple<point, point, u32, std::vector<std::pair<point, u32>>>;
    auto cmp = [](state const& a, state const& b) { return std::get<2>(a) > std::get<2>(b); };
    std::priority_queue<state, std::vector<state>, decltype(cmp)> queue;
    queue.emplace(start, dir::east, 0, std::vector<std::pair<point, u32>>{});

    aoc::dense::map<point, u32> best;
    auto best_score{0U};

    while(!queue.empty()) {
        auto [p, d, c, m] = queue.top(); queue.pop();
        auto const [i, j] = p;
        m.emplace_back(p, c);
        cost(i, j) = std::min(c, cost(i, j));
        if (p == end) {
            best_score = c;
            best.insert(m.begin(), m.end());
            break;
        }
        if (std::exchange(seen(i, j), 1)) { continue; }
        for (auto const e : directions) {
            auto const q = p+e;
            if (e == -d || g(q[0], q[1]) == '#') { continue; } // don't go back
            auto const t = e == d ? 1 : 1+turn_cost;
            queue.emplace(q, e, c+t, m);
        }
    }

    std::function<bool(point, point, u32)> dfs = [&](point p, point d, u32 c) -> bool {
        auto const [i, j] = p;
        if (c > std::min(best_score, cost(i, j)+turn_cost)) {
            return false;
        }
        if (p == end) { return true; }
        if (std::exchange(seen(i, j), 1)) { return false; }
        auto found = false;
        for (auto const e : directions) {
            auto const q = p+e;
            if(e == -d || g(q[0], q[1]) == '#') { continue; }
            auto const t = e == d ? 1 : 1+turn_cost;
            found |= dfs(q, e, c+t);
        }
        if (found) { best.insert({p, c}); }
        seen(i, j) = 0;
        return found;
    };
    seen.setConstant(0);
    dfs(start, dir::east, 0);
    auto const p1 = best_score;
    auto const p2 = best.size();
    return aoc::result(p1, p2);
}