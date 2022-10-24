#include <aoc.hpp>
#include <functional>

enum op { AND, OR, LSHIFT, RSHIFT, NOT, ASSIGN, NONE }; // NOLINT

struct node {
    std::string id;
    op op; // NONE means that the node is a leaf node

    node* lhs;
    node* rhs;
    node* parent;

    u16 signal;
    bool cycle{false};

    auto reset() {
        cycle = false;
        signal = 0;
    }

    auto eval() -> u16
    {
        if (cycle) { return signal; }
        cycle = true;
        switch (op) {
        case op::AND: {
            signal = lhs->eval() & rhs->eval();
            return signal;
        }
        case op::OR: {
            signal = lhs->eval() | rhs->eval();
            return signal;
        }
        case op::LSHIFT: {
            signal = lhs->eval() << rhs->eval();
            return signal;
        }
        case op::RSHIFT: {
            signal = lhs->eval() >> rhs->eval();
            return signal;
        }
        case op::NOT: {
            signal = ~lhs->eval();
            return signal;
        }
        case op::ASSIGN: {
            signal = lhs->eval();
            return signal;
        }
        default: {
            return signal;
        }
        }
    }
};

template <>
auto advent<2015>::day07() const -> void
{
    constexpr std::array binary_labels = { "AND", "OR", "LSHIFT", "RSHIFT" };
    constexpr std::array unary_labels = { "NOT" };

    std::fstream f("./source/2015/07/input.txt");
    std::vector<std::string> input;
    for (std::string s; std::getline(f, s);) {
        input.push_back(s);
    }
    auto const npos = std::string::npos;

    std::vector<node> nodes;
    nodes.reserve(input.size() * 2);
    robin_hood::unordered_map<std::string, node*> map;

    auto add_node = [&](auto const&... ids) {
        std::array<node*, sizeof...(ids)> pointers = { nullptr };
        int pos { 0 };
        auto try_add = [&](auto const& id) {
            node* p { nullptr };
            if (auto it = map.find(id); it == map.end()) {
                nodes.push_back({ id, NONE, nullptr, nullptr, nullptr, 0U });
                p = &nodes.back();
                map[id] = p;
            } else {
                p = it->second;
            }
            pointers[pos++] = p;
            if (std::isdigit(id[0])) { // leaf node
                (void)scn::scan(id, "{}", p->signal);
            }
        };
        (try_add(ids), ...);
        return pointers;
    };

    for (auto const& s : input) {
        std::string a {};
        std::string b {};
        std::string c {};
        op op { op::NONE };

        // figure out the OP
        if (s.find("AND") != npos) {
            op = op::AND;
            (void)scn::scan(s, "{} AND {} -> {}", a, b, c);
        } else if (s.find("OR") != npos) {
            op = op::OR;
            (void)scn::scan(s, "{} OR {} -> {}", a, b, c);
        } else if (s.find("RSHIFT") != npos) {
            op = op::RSHIFT;
            (void)scn::scan(s, "{} RSHIFT {} -> {}", a, b, c);
        } else if (s.find("LSHIFT") != npos) {
            op = op::LSHIFT;
            (void)scn::scan(s, "{} LSHIFT {} -> {}", a, b, c);
        } else if (s.find("NOT") != npos) {
            op = op::NOT;
            (void)scn::scan(s, "NOT {} -> {}", a, b);
        } else {
            op = op::ASSIGN;
            (void)scn::scan(s, "{} -> {}", a, b);
        }

        // make wires
        if (op < op::NOT) {
            // binary symbol
            auto [pa, pb, pc] = add_node(a, b, c);
            pc->lhs = pa; pa->parent = pc;
            pc->rhs = pb; pb->parent = pc;
            pc->op = op;
        } else {
            auto [pa, pb] = add_node(a, b);
            pb->lhs = pa; pa->parent = pb;
            pb->op = op;
        }
    }

    auto isroot = [](auto const& n) { return n.parent == nullptr; };
    auto isfunc = [](auto const& n) { return n.op != op::NONE; };

    std::ranges::for_each(lz::filter(nodes, isroot), &node::eval);
    auto a = std::ranges::find_if(nodes, [](auto const& n) { return n.id == "a"; });
    auto s = a->signal;
    fmt::print("part 1: {}\n", a->signal);

    auto b = std::ranges::find_if(nodes, [](auto const& n) { return n.id == "b"; });
    std::ranges::for_each(lz::filter(nodes, isfunc), &node::reset);
    b->signal = s;
    b->op = op::NONE;
    std::ranges::for_each(lz::filter(nodes, isroot), &node::eval);
    fmt::print("part 2: {}\n", a->signal);
}
