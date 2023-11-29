#include <aoc.hpp>

namespace {
    struct node {
        i32 id{};
        mutable bool visited{false};
        std::vector<node*> pipes;

        [[nodiscard]] auto group_size() const -> i32 {
            visited = true;
            return std::transform_reduce(pipes.begin(), pipes.end(), 0, std::plus{}, [&](auto const* n) {
                return n->visited ? 0 : n->group_size(); 
            }) + 1;
        }
    };
} // namespace

template<>
auto advent2017::day12() -> result {
    auto input = aoc::util::readlines("./source/2017/12/input.txt");
    std::vector<node> nodes(input.size());

    for (auto i = 0; i < input.size(); ++i) {
        auto& node = nodes[i];
        std::string s;
        std::vector<i32> v;
        (void)scn::scan(input[i], "{} <-> {}", node.id, s);
        (void)scn::scan_list_ex(s, v, scn::list_separator(','));
        std::ranges::transform(v, std::back_inserter(node.pipes), [&](auto i) { return &nodes[i]; });
    }

    auto p1 = nodes.front().group_size();
    auto p2{1};
    while(true) {
        for (auto& n : nodes) {
            if (!n.visited) {
                ++p2;
                (void)n.group_size();
                continue;
            }
        }
        break;
    }
    return aoc::result(p1, p2);
}
