#include <aoc.hpp>

namespace detail {
struct node {
    u32 value;
    u32 next;
    u32 index;
};
} // namespace detail

template <>
auto advent2020::day23() -> void {
    std::vector<u8> input { 6, 2, 4, 3, 9, 7, 1, 5, 8 }; // NOLINT
    auto play = [&input](auto ncups, auto rounds, bool part2 = false) {
        std::vector<detail::node> nodes(ncups);
        for (u32 i = 0; i < ncups; ++i) {
            auto& n = nodes[i];
            n.value = i < input.size() ? input[i] : i + 1;
            n.next = 0;
            nodes[n.value - 1].index = i;
            if (i > 0) {
                nodes[i - 1].next = i;
            }
        }
        nodes.back().next = 0;
        for (u32 round = 0, p = 0; round < rounds; ++round, p = nodes[p].next) {
            auto q = nodes[p].next;
            auto const a { nodes[q].value };
            q = nodes[q].next;
            auto const b { nodes[q].value };
            q = nodes[q].next;
            auto const c { nodes[q].value };
            auto d = nodes[p].value;
            do {
                if (--d == 0) {
                    d = ncups;
                }
            } while (a == d || b == d || c == d);
            auto r = nodes[d - 1].index;
            auto x = nodes[q].next;
            nodes[q].next = nodes[r].next;
            nodes[r].next = nodes[p].next;
            nodes[p].next = x;
        }
        if (part2) {
            auto x = nodes[nodes[0].index];
            auto y = nodes[x.next];
            auto z = nodes[y.next];
            return fmt::format("{}", 1UL * x.value * y.value * z.value);
        }
        std::string s;
        s.reserve(ncups - 1);
        auto j = nodes[0].index;
        for (auto i = 0; i < ncups - 1; ++i) {
            j = nodes[j].next;
            s.push_back(static_cast<char>(nodes[j].value + '0'));
        }
        return s;
    };
    constexpr u32 p1cups { 9 };
    constexpr u32 p1rounds { 100 };
    constexpr u32 p2cups { 1'000'000 };
    constexpr u32 p2rounds { 10'000'000 };
    fmt::print("part 1: {}\n", play(p1cups, p1rounds, false));
    fmt::print("part 2: {}\n", play(p2cups, p2rounds, true));
}
