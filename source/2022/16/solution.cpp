#include <aoc.hpp>
#include "util/dynamic_bitset.hpp"

namespace {
struct valve {
    u64 index{};
    u64 id{};
    u64 rate{};
    std::vector<valve*> tunnels;
};

template<std::size_t N, std::size_t M = 30UL>
requires (N == 1 || N == 2)
struct state {
    std::array<valve const*, N> operators{};
    u64 open_valves{0UL};
    u64 time{0UL};
    u64 pressure{0UL};

    [[nodiscard]] inline auto is_open(auto i) const {
        return (open_valves & (1UL << i)) != 0;
    }

    inline auto open(auto i) {
        open_valves |= (1UL << i);
    }

    [[nodiscard]] inline auto hash() const {
        aoc::util::hash h;
        auto sum = [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return (operators[Idx]->id + ...);
        }(std::make_index_sequence<N>{});
        //return sum;
        return h(sum);
    }

    [[nodiscard]] inline auto ppm(auto const& valves) const {
        auto s{0UL};
        for (auto x = open_valves; x != 0; x &= (x-1)) {
            s += valves[std::countr_zero(x)].rate;
        }
        return s;
    }

    inline auto update(auto const& valves) {
        pressure += ppm(valves);
        ++time;
    }

    [[nodiscard]] inline auto get_neighbors(auto op) const {
        std::vector<state> neighbors;

        auto const*v = operators[op];

        if (!is_open(v->index) && v->rate > 0) {
            state s = *this;
            s.open(v->index);
            neighbors.push_back(s);
        }

        for (auto const* u : v->tunnels) {
            state s = *this;
            s.operators[op] = u;
            neighbors.push_back(s);
        }
        return neighbors;
    }

    [[nodiscard]] inline auto get_neighbors() const {
        if constexpr (N == 1) { return get_neighbors(0); }
        else {
            std::vector<state> neighbors;
            auto n0 = get_neighbors(0);
            auto n1 = get_neighbors(1);

            for (auto&& [s, q] : lz::cartesian(n0, n1)) {
                ENSURE(s.time == q.time);
                state w{};
                w.operators[0] = s.operators[0];
                w.operators[1] = q.operators[1];
                w.open_valves = s.open_valves | q.open_valves;
                w.time = s.time;
                w.pressure = s.pressure;
                neighbors.push_back(w);
            }
            return neighbors;
        }
    }

    friend auto operator<(state const& a, state const& b) {
        return a.time > b.time;
    }
};

template<std::size_t N, std::size_t M>
auto bfs(auto const& valves, auto const* startpos) {
    state<N, M> start;
    for (auto i = 0UL; i < N; ++i) { start.operators[i] = startpos; }

    aoc::dense::map<u64, std::tuple<u64, u64>> seen;

    std::priority_queue<state<N, M>> queue;
    queue.push(start);

    state<N, M> best{};
    while(!queue.empty()) {
        auto state = queue.top();
        queue.pop();
        state.update(valves);

        if (state.time >= M) {
            continue;
        }

        ENSURE(best.time <= state.time);

        while(best.time < state.time) { best.update(valves); }
        if (best.pressure < state.pressure) {
            best = state;
        }

        auto [t, p] = std::tuple{state.time, state.pressure};

        if (auto [it, ok] = seen.insert({ state.hash(), {t, p} }); !ok) {
            auto [t1, p1] = it->second;
            ENSURE(t1 <= t);
            if (p <= p1) { continue; }
            it->second = {t, p};
        }

        for (auto&& s : state.get_neighbors()) {
            queue.push(s);
        }
    }
    while (best.time < M) { best.update(valves); }
    return best.pressure;
}
} // namespace

template<>
auto advent2022::day16() -> result {
    auto input = aoc::util::readlines("./source/2022/16/input.txt");
    aoc::util::hash hash;

    using valve = valve;
    std::vector<valve> valves(input.size()); // important for iterator stability

    aoc::dense::map<u64, std::string> names;
    aoc::dense::map<u64, valve*> ptr;

    // first pass, get valves
    for (auto i = 0UL; i < input.size(); ++i) {
        auto const& line = input[i];
        std::string_view s{line.data(), line.find(';')};
        // std::string id;
        // auto rate{0UL};
        auto [id, rate] = scn::scan<std::string, u64>(s, "Valve {} has flow rate={}")->values();
        valves[i] = { i, hash(id), rate };
        names.insert({ valves[i].id, id });
        ptr.insert({ valves[i].id, &valves[i] });
    }

    // second pass, get connections
    for(auto&& [i, line] : lz::enumerate(input)) {
        aoc::util::remove_all(line, ","); // due to scn bug
        auto p = line.find(';');
        std::string_view s{&line[p+2], line.size()-p-2};
        while(std::isspace(s.front()) || std::islower(s.front())) { s.remove_prefix(1); }
        auto tunnels = lz::map(lz::split(s, ' '), [](auto sv){ return std::string{sv}; }).toVector();

        auto& v = valves[i];
        for (auto const& t : tunnels) {
            ENSURE(ptr.contains(hash(t)));
            v.tunnels.push_back(ptr[hash(t)]);
        }
    }

    auto constexpr time_limit_p1{30};
    auto constexpr time_limit_p2{26};
    auto part1 = bfs<1, time_limit_p1>(valves, ptr[hash("AA")]); //  NOLINT
    auto part2 = bfs<2, time_limit_p2>(valves, ptr[hash("AA")]); //  NOLINT

    return aoc::result(part1, part2);
}
