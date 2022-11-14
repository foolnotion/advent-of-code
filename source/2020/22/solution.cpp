#include <aoc.hpp>
#include <iostream>

template<>
auto advent2020::day22() -> result {
    auto input = aoc::util::readlines("./source/2020/22/input.txt");

    using deck = std::deque<u32>;
    deck d1;
    deck d2;

    auto line = input.begin()+1; // skip "Player 1"
    for (line = input.begin() + 1; !line->empty(); ++line) {
        d1.push_back(scn::scan_value<u32>(*line).value());
    }
    line += 2; // skip empty line and "Player 2" line
    for (; line < input.end(); ++line) {
        d2.push_back(scn::scan_value<u32>(*line).value());
    }

    auto score = [](auto const& deq) {
        auto const e = lz::enumerate(lz::reverse(deq));
        auto zip = [](auto t) { auto [a,b] = t; return (a+1)*b; };
        return std::transform_reduce(e.begin(), e.end(), 0UL, std::plus{}, zip);
    };

    auto draw = [](auto& deq) { auto x = deq.front(); deq.pop_front(); return x; };

    auto combat = [&](auto p1, auto p2) {
        std::remove_cvref_t<decltype(p1)>* winner{nullptr};
        while (!(p1.empty() or p2.empty())) {
            auto a = draw(p1);
            auto b = draw(p2);
            winner = a > b ? &p1 : &p2;
            if (winner == &p2) { std::swap(a,b); }
            winner->push_back(a);
            winner->push_back(b);
        }
        return score(*winner);
    };

    using aoc::util::hash;
    auto recursive_combat = [&](auto p1, auto p2) -> u32 {
        auto rec = [&](auto p1, auto p2, auto&& f) -> std::pair<u32, bool> { // NOLINT
            aoc::dense::set<u64> seen;
            deck* winner{nullptr};
            while(!(p1.empty() || p2.empty())) {
                if (winner != nullptr) {
                    if (auto [it, ok] = seen.insert(hash{}(*winner)); !ok) {
                        return {score(p1), true}; // p1 wins by default
                    }
                }
                auto a = draw(p1);
                auto b = draw(p2);
                if (p1.size() < a || p2.size() < b) {
                    winner = a > b ? &p1 : &p2;
                } else {
                    deck r1(p1.begin(), p1.begin()+a);
                    deck r2(p2.begin(), p2.begin()+b);
                    auto [s, p] = f(std::move(r1), std::move(r2), f);
                    winner = p ? &p1 : &p2;
                }
                if (winner == &p2) { std::swap(a,b); }
                winner->push_back(a);
                winner->push_back(b);
            }
            return {score(*winner), winner == &p1};
        };
        return rec(p1, p2, rec).first;
    };
    auto p1 = combat(d1, d2);
    auto p2 = recursive_combat(d1, d2);
    return aoc::result(p1, p2);
}
