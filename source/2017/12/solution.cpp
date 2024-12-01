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

    for (auto i = 0; i < std::ssize(input); ++i) {
        auto& node = nodes[i];
        auto [id, str] = scn::scan<i32, std::string>(input[i], "{} <-> {}")->values();
        node.id = id;
        auto v = lz::map(lz::split(str, ','), [](auto const& s){ return scn::scan_value<i32>(s)->value(); }).toVector();
        std::ranges::transform(v, std::back_inserter(node.pipes), [&](auto i) { return &nodes[i]; });
    }

    auto p1 = nodes.front().group_size();
    auto p2 = std::transform_reduce(nodes.begin(), nodes.end(), 1, std::plus{}, [](auto const& n) {
        if (!n.visited) { (void)n.group_size(); return 1; }
        return 0;
    });
    return aoc::result(p1, p2);
}