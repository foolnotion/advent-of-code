#include <aoc.hpp>

namespace rs = std::ranges;
namespace vs = std::views;

namespace {
    enum class node_type : u8 { gate_and, gate_or, gate_xor, wire };

    constexpr std::array<std::add_pointer_t<u8(u8, u8)>, 3> gates {
        [](u8 a, u8 b) -> u8 { return a & b; },
        [](u8 a, u8 b) -> u8 { return a | b; },
        [](u8 a, u8 b) -> u8 { return a ^ b; }
    };

    struct node {
        node_type type;
        std::string label;
        mutable std::optional<u8> value;
        std::vector<node*> in;
        std::vector<node*> out;

        [[nodiscard]] auto eval() const -> std::optional<u8> {
            if (value) { return value; }
            value = 0; // avoid cycles
            value = is_gate()
                ? gates[static_cast<u8>(type)](*in[0]->eval(), *in[1]->eval())
                : in[0]->eval();
            return value;
        }

        auto reset() { value = std::nullopt; }
        auto front() const -> char { return label.front(); }
        auto is_initial_input() const -> bool { return label == "x00" || label == "y00"; }
        auto is_input() const -> bool { return type == node_type::wire && in.empty(); }
        auto is_output() const -> bool { return type == node_type::wire && front() == 'z'; }
        auto is_input_output() const -> bool { return is_input() || is_output(); }
        auto is_gate() const -> bool { return type != node_type::wire && std::isupper(front()); }
        auto is_inner() const -> bool { return !(is_gate() || is_input_output()); }

        auto valid() const -> bool {
            if (is_gate() && front() == 'A') {
                return out[0]->is_inner() &&
                (rs::all_of(in, &node::is_initial_input) || out[0]->out[0]->front() == 'O');
            }
            if (is_gate() && front() == 'X') {
                auto inner_or_initial_input = [](auto const* n) { return n->is_initial_input() || n->is_inner(); };
                return rs::none_of(out[0]->out, [](auto const* n) { return n->front() == 'O'; }) &&
                       ((out[0]->is_output() && rs::all_of(in, inner_or_initial_input)) ||
                       (out[0]->is_inner() && rs::all_of(in, &node::is_input)));
            }
            if (is_gate() && front() == 'O') {
                return (out[0]->is_inner() && rs::all_of(in, &node::is_inner)) ||
                       (out[0]->is_output() && out[0]->label == "z45");
            }
            return true;
        }
    };

    struct node_manager {
        aoc::dense::map<std::string, std::unique_ptr<node>> nmap;
        auto add_node(std::string const& label, std::optional<u8> value = {}) {
            node_type t{};
            switch(label[0]) {
                case 'X': t = node_type::gate_xor; break;
                case 'A': t = node_type::gate_and; break;
                case 'O': t = node_type::gate_or; break;
                default:  t = node_type::wire;
            }
            auto [it, _] = nmap.insert({label, std::make_unique<node>(t, label, value)});
            return it->second.get();
        };
        auto operator[](std::string const& label) -> node* { return nmap[label].get(); }

        [[nodiscard]] auto nodes() const {
            auto get_node = [&](auto const& p) { return p.second.get(); };
            return nmap | vs::transform(get_node);
        }
    };

    auto parse(std::vector<std::string> const& lines) {
        node_manager m;
        auto it = rs::find(lines, std::string{});
        for (auto const& line : std::span{lines.begin(), it}) {
            auto label = line.substr(0, 3);
            auto value = line.back() == '0' ? 0 : 1;
            (void) m.add_node(label, value);
        }
        auto i = 0;
        for (auto const& line : std::span{it+1, lines.end()}) {
            auto split = vs::split(line, ' ') | vs::transform([](auto x) { return std::string{x.begin(), x.end()}; });
            std::vector<std::string> tokens{split.begin(), split.end()};
            auto* lhs  = m.add_node(tokens[0]);
            auto* gate = m.add_node(fmt::format("{}{}", tokens[1], i++));
            auto* rhs  = m.add_node(tokens[2]);
            auto* out  = m.add_node(tokens[4]);
            gate->in.push_back(lhs); lhs->out.push_back(gate);
            gate->in.push_back(rhs); rhs->out.push_back(gate);
            out->in.push_back(gate); gate->out.push_back(out);
        }
        return m;
    }

    auto add(std::vector<node const*>& x, std::vector<node const*>& y) {
        u64 c = 0;
        u64 v = 0;
        for (auto i = 0UL; i < x.size(); ++i) {
            auto const a = *(x[i]->value);
            auto const b = *(y[i]->value);
            auto const r = static_cast<u8>(a ^ b);
            u64  const s = r ^ c;
            c = static_cast<u8>(a & b) | (c & r);
            v += s << i;
        }
        // normally we would add the carry at the end
        // but here we have to debug the wire outputs
        // v += (c << sz);
        return v;
    };
} // namespace

template<>
auto advent2024::day24() -> result {
    auto get_value = [&](std::vector<node const*> nodes, u64 size = 0) {
        auto v{0UL};
        auto const sz = size == 0 ? nodes.size() : std::min(nodes.size(), size);
        for (auto i = 0UL; i < sz; ++i) {
            auto const* n = nodes[i];
            if (n->label[0] == 'z') { (void) n->eval(); }
            v += static_cast<u64>(*n->value) << i;
        }
        return v;
    };
    auto const lines = aoc::util::readlines("./source/2024/24/input.txt");
    auto m = parse(lines);

    // gather all the bit-wires into some "variables"
    std::vector<node const*> x;
    std::vector<node const*> y;
    std::vector<node const*> z;
    std::array a{ &x, &y, &z };
    for (auto const* n : m.nodes() | vs::filter(&node::is_input_output)) {
        a[n->front()-'x']->push_back(n);
    }
    for (auto* v : a) { rs::sort(*v, std::less{}, &node::label); }
    auto const p1 = get_value(z);

    // part 2 - identify misplaced nodes and do an exhaustive search
    auto expected = add(x, y);
    auto success = [&]() {
        rs::for_each(m.nodes() | vs::filter(std::not_fn(&node::is_input)), &node::reset);
        return expected == get_value(z, x.size());
    };
    auto misplaced = m.nodes() | vs::filter(std::not_fn(&node::valid));
    std::vector<node*> vec{misplaced.begin(), misplaced.end()};

    std::vector<std::string> swapped;
    [[maybe_unused]] auto swap = [&](node* u, node* v) {
        for (auto *c : u->out) { c->in[0] = v; }
        for (auto *c : v->out) { c->in[0] = u; }
        std::swap(u->out, v->out);
        swapped.push_back(u->out[0]->label);
        swapped.push_back(v->out[0]->label);
    };
    auto swap_pairs = [&](auto& vec) -> bool {
        for (auto i = 0; i < 4; ++i) {
            if (vec[i]->front() == vec[i+4]->front()) { return false; }
            swap(vec[i], vec[i+4]);
        }
        return true;
    };
    while(rs::next_permutation(vec).found) {
        swapped.clear();
        if (swap_pairs(vec) && success()) { break; }
        swap_pairs(vec);
    }
    rs::sort(swapped);
    auto const p2 = fmt::format("{}", fmt::join(swapped, ","));
    return aoc::result(p1, p2);
}