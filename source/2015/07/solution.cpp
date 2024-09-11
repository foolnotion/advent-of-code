#include <aoc.hpp>
#include <functional>

namespace {
struct node {
    std::string id;
    std::function<u16(u16, u16)> op{nullptr}; // NONE means that the node is a leaf node
    node* lhs{nullptr};
    node* rhs{nullptr};
    node* parent{nullptr};
    std::optional<u16> signal{};

    auto reset() { signal = std::nullopt; }
    auto eval() -> u16 {
        if(!signal) { signal = op(lhs->eval(), rhs ? rhs->eval() : 0); }
        return signal.value();
    }
};
} // namespace

template <>
auto advent2015::day07() -> result
{
    using ::node;

    std::fstream f("./source/2015/07/input.txt");
    constexpr auto npos = std::string::npos;
    std::vector<node> nodes;
    nodes.reserve(1024); // NOLINT
    aoc::dense::map<std::string, node*> map;

    auto add_node = [&](auto const&... ids) {
        auto try_add = [&](auto const& id) {
            node* p { nullptr };
            if (auto it = map.find(id); it == map.end()) {
                nodes.push_back({ id });
                p = &nodes.back();
                map[id] = p;
            } else {
                p = it->second;
            }
            if (std::isdigit(id[0])) { // leaf node
                p->signal = scn::scan_value<u16>(id)->value();
            }
            return p;
        };
        return std::array {try_add(ids)...};
    };

    for (std::string s; std::getline(f, s); ) {
        // std::string a, b, c; // NOLINT
        decltype(node::op) func{nullptr};

        using S = std::string; // NOLINT
        std::tuple<S, S, S> ids;

        // figure out the OP
        bool binary{true};
        if (s.find("AND") != npos) {
            func = [](u16 x, u16 y) { return x & y; };
            ids = scn::scan<S, S, S>(s, "{} AND {} -> {}")->values();
        } else if (s.find("OR") != npos) {
            func = [](u16 x, u16 y) { return x | y; };
            ids = scn::scan<S, S, S>(s, "{} OR {} -> {}")->values();
        } else if (s.find("RSHIFT") != npos) {
            func = [](u16 x, u16 y) { return x >> y; };
            ids = scn::scan<S, S, S>(s, "{} RSHIFT {} -> {}")->values();
        } else if (s.find("LSHIFT") != npos) {
            func = [](u16 x, u16 y) { return x << y; };
            ids = scn::scan<S, S, S>(s, "{} LSHIFT {} -> {}")->values();
        } else if (s.find("NOT") != npos) {
            binary = false;
            func = [](u16 x, u16) { return ~x; };
            auto [a, b] = scn::scan<S, S>(s, "NOT {} -> {}")->values();
            ids = {a, b, ""};
        } else { // ASSIGN
            binary = false;
            func = [](u16 x, u16) { return x; };
            auto [a, b] = scn::scan<S, S>(s, "{} -> {}")->values();
            ids = {a, b, ""};
        }
        // make wires
        auto [a, b, c] = ids;
        if (binary) { // binary node
            auto [pa, pb, pc] = add_node(a, b, c);
            pc->lhs = pa; pa->parent = pc;
            pc->rhs = pb; pb->parent = pc;
            pc->op = func;
        } else { // unary node
            auto [pa, pb] = add_node(a, b);
            pb->lhs = pa; pa->parent = pb;
            pb->op = func;
        }
    }

    auto isroot = [](auto const& n) { return n.parent == nullptr; };
    auto isfunc = [](auto const& n) { return static_cast<bool>(n.op); };

    std::ranges::for_each(lz::filter(nodes, isroot), &node::eval);
    auto a = std::ranges::find_if(nodes, [](auto const& n) { return n.id == "a"; });
    auto p1 = *a->signal;

    auto b = std::ranges::find_if(nodes, [](auto const& n) { return n.id == "b"; });
    std::ranges::for_each(lz::filter(nodes, isfunc), &node::reset);
    b->signal = p1;
    std::ranges::for_each(lz::filter(nodes, isroot), &node::eval);
    auto p2 = *a->signal;
    return aoc::result(p1, p2);
}
