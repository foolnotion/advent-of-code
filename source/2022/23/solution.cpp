#include <aoc.hpp>

template <>
auto advent2022::day23() -> result
{
    auto input = aoc::util::readlines("./source/2022/23/input.txt");
    using point = Eigen::Array2i;
    auto hash = [](point const& p) { return aoc::util::hash{}(p[0], p[1]); };
    auto eql = [](point const& a, point const& b) { return a[0] == b[0] && a[1] == b[1]; };

    aoc::dense::set<point, decltype(hash), decltype(eql)> elves;

    for (auto&& [i, line] : lz::enumerate(input)) {
        for (auto&& [j, c] : lz::enumerate(line)) {
            if (c == '#') {
                elves.insert({i, j});
            }
        }
    }

    aoc::dense::map<point, point, decltype(hash), decltype(eql)> proposed_destinations;
    aoc::dense::map<point, u32, decltype(hash), decltype(eql)> proposed_counts;
    using std::ranges::any_of;

    auto try_move = [&](point const& p, std::array<point, 3> const& pos) -> std::optional<point> {
        if (any_of(pos, [&](auto const& q) { return elves.contains(p + q); })) {
            return std::nullopt;
        }
        return { p + pos[1] };
    };

    std::array pos {
        std::array<point, 3>{ point{-1, -1}, point{-1, 0}, point{-1, +1} },
        std::array<point, 3>{ point{+1, -1}, point{+1, 0}, point{+1, +1} },
        std::array<point, 3>{ point{-1, -1}, point{0, -1}, point{+1, -1} },
        std::array<point, 3>{ point{-1, +1}, point{0, +1}, point{+1, +1} },
    };

    std::array<std::function<std::optional<point>(point const&)>, 4> actions {
        [&](auto const& p) -> std::optional<point> { return try_move(p, pos[0]); },
        [&](auto const& p) -> std::optional<point> { return try_move(p, pos[1]); },
        [&](auto const& p) -> std::optional<point> { return try_move(p, pos[2]); },
        [&](auto const& p) -> std::optional<point> { return try_move(p, pos[3]); }
    };

    auto has_elf = [&elves](auto const& p) {
        for (auto i = p[0]-1; i <= p[0]+1; ++i) {
            for (auto j = p[1]-1; j <= p[1]+1; ++j) {
                if (i == p[0] && j == p[1]) { continue; }
                if (elves.contains({i, j})) { return true; }
            }
        }
        return false;
    };

    auto constexpr rounds{2000};
    auto part2{0};
    for (auto r = 0; r < rounds; ++r) {
        proposed_destinations.clear();
        proposed_counts.clear();

        // propose actions
        for (auto const& p : elves) {
            if (!has_elf(p)) { continue; }

            for (auto&& a : actions) {
                if (auto q = a(p); q) {
                    proposed_destinations[p] = *q;
                    auto [it, ok] = proposed_counts.insert({*q, 0});
                    it->second += 1;
                    break;
                }
            }
        }

        // move elves
        auto tmp = elves.values();
        elves.clear();
        bool moved{false};
        for (auto const& p : tmp) {
            auto it = proposed_destinations.find(p);
            if (it != proposed_destinations.end() && proposed_counts[it->second] == 1) {
                moved = true;
                elves.insert(it->second);
            } else {
                elves.insert(p);
            }
        }

        if (!moved) { part2 = r+1; break; }

        // rotate list of actions
        std::rotate(actions.begin(), actions.begin()+1, actions.end());
    }

    auto get_rectangle = [](auto const& elves) {
        point a = elves.values().front();
        point b = elves.values().front();
        for (auto const& p : elves) {
            a = (a < p).select(a, p);
            b = (b > p).select(b, p);
        }
        return std::tuple { a, b };
    };

    auto [a, b] = get_rectangle(elves);
    auto part1 = (b[0] - a[0] + 1) * (b[1] - a[1] + 1) - elves.size();
    return aoc::result(part1, part2);
}
