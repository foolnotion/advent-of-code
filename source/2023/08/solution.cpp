#include <aoc.hpp>

namespace {
    struct node {
        u64 id;
        char end;
        node* left;
        node* right;

        [[nodiscard]] auto next(char c) const {
            return c == 'R' ? right : left;
        }
    };

    auto parse(auto const& input) {
        aoc::dense::map<u64, u64> map;
        std::vector<node> nodes;
        nodes.reserve(input.size());
        aoc::util::hash hash;

        // first pass get nodes
        for (auto i = 0UL; i < input.size(); ++i) {
            auto const& s = input[i];
            std::string_view v{s.begin(), s.begin()+s.find(' ')};
            auto h = hash(v);
            map[h] = nodes.size();
            nodes.push_back(node{h, v.back()});
        }

        // second pass fill pointer links
        for (auto i = 0UL; i < input.size(); ++i) {
            auto const& s = input[i];
            auto j = s.find('(');
            auto k = s.find(',');
            auto l = s.size()-1;
            auto a = s.substr(j+1, k-j-1);
            auto b = s.substr(k+2, l-k-2);
            auto& node = nodes[i];
            node.left  = &nodes[ map[hash(a)] ];
            node.right = &nodes[ map[hash(b)] ];
        }
        return nodes;
    }
} // namespace

template<>
auto advent2023::day08() -> result {
    auto input = aoc::util::readlines("./source/2023/08/input.txt");
    auto instructions = input.front();
    std::span node_input{input.begin()+2, input.end()};
    auto nodes = parse(node_input);
    auto const sz = instructions.size();

    // part 1
    auto constexpr start_id = aoc::util::hash{}("AAA");
    auto constexpr end_id   = aoc::util::hash{}("ZZZ");
    auto it = std::ranges::find_if(nodes, [](auto const& n) { return n.id == start_id; });
    auto* n = &*it;
    auto steps{0UL};
    for (auto i = 0UL; n->id != end_id; i = (i+1) % sz, ++steps) {
        n = n->next(instructions[i]);
    }
    auto const p1 = steps;

    // part 2
    auto ends_with_a = [](auto const& n) { return n.end == 'A'; };
    auto cycle_length = [&](auto const& n) -> u64 {
        auto* p = &n;
        for (auto i = 0UL, j = 0UL; ; i = (i+1) % sz, ++j) {
            if (p->end == 'Z') { return j; }
            p = p->next(instructions[i]);
        }
    };
    auto lengths  = nodes | std::views::filter(ends_with_a) | std::views::transform(cycle_length);
    auto const p2 = std::reduce(lengths.begin(), lengths.end(), 1UL, &std::lcm<u64, u64>);
    return aoc::result(p1, p2);
}