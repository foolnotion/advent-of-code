#include <aoc.hpp>

namespace {
template <int N, int V>
struct circle {
    u64 n { 0 };
    std::list<i64> marbles { 0 };
    static constexpr auto special_marble{23};
    std::array<u64, N> player_scores{0};

    // track the list position where the next marble should be inserted
    decltype(marbles)::iterator pos { marbles.begin() };

    auto advance(auto& iter, i64 d)
    {
        for (decltype(d) i = 0; i < std::abs(d); ++i) {
            auto [a, b, c] = d > 0
                ? std::tuple{marbles.end(), marbles.begin(), +1}
                : std::tuple{marbles.begin(), marbles.end(), -1};
            if (iter == a) { iter = b; }
            std::advance(iter, c);
        }
    }

    auto next(auto iter) { advance(iter, 1); return iter; }

    auto prev(auto iter) { advance(iter, -1); return iter; }

    auto play_turn()
    {
        auto const p = (n++ % N);
        auto const v = n;

        if (marbles.size() > 1 && v % special_marble == 0) {
            advance(pos, -8);
            player_scores[p] += v + *pos;
            pos = next(marbles.erase(pos));
        } else {
            pos = next(marbles.insert(next(pos), v));
        }
    }

    auto play()
    {
        while (*next(pos) < V) { play_turn(); }
        return std::ranges::max(player_scores);
    }
};
} // namespace

template <>
auto advent2018::day09() -> result
{
    auto constexpr players{423};
    auto constexpr lastval{71944};
    auto const p1 = circle<players, lastval>{}.play();
    auto const p2 = circle<players, 100*lastval>{}.play();
    return aoc::result(p1, p2);
}
