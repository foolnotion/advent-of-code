#include <aoc.hpp>
#include <sul/dynamic_bitset.hpp>

namespace {
    using bitset = sul::dynamic_bitset<u64>;
    constexpr auto nbits{100};
    struct node {
        u64 id;
        bitset antecedents;
        std::vector<node*> children;

        auto operator<(node const& other) const {
            return other.antecedents.test(id);
        }
    };
} // namespace

template<>
auto advent2024::day05() -> result {
    auto lines = aoc::util::readlines("./source/2024/05/input.txt");
    aoc::dense::map<u64, std::unique_ptr<node>> nodes;
    decltype(lines)::iterator it;

    auto insert = [&](auto id) {
        auto [it, ok] = nodes.insert({id, std::make_unique<node>(id) });
        if (ok) { it->second->antecedents.resize(nbits, false); }
        return it->second.get();
    };

    for (it = lines.begin(); !it->empty(); ++it) {
        if (auto res = scn::scan<u64, u64>(*it, "{}|{}"); res) {
            auto [a, b] = res->values();
            auto* pa = insert(a);
            auto* pb = insert(b);
            pa->children.push_back(pb);
            pb->antecedents.set(a);
        }
    }

    auto p1{0UL};
    auto p2{0UL};
    for (auto const& line : std::span{it+1, lines.end()}) {
        auto v = std::views::split(line, ',') | std::views::transform([](auto x) {
            return aoc::util::read<u64>({x.begin(), x.end()});
        });
        std::vector<u64> values{v.begin(), v.end()};
        if (std::ranges::is_sorted(values, std::less{}, [&](auto v) { return *nodes[v]; })) {
            p1 += values[values.size()/2];
        } else {
            std::ranges::sort(values, std::less{}, [&](auto v) { return *nodes[v]; });
            p2 += values[values.size()/2];
        }
    }
    return aoc::result(p1, p2);
}
