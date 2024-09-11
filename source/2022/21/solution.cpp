#include <aoc.hpp>

namespace {
    struct node {
        u64 hash{};

        char op{};
        i64 value{};

        u64 lhs{};
        u64 rhs{};

        auto evaluate(auto& ptr) const -> i64 {
            if (lhs == 0 && rhs == 0) { return value; }
            auto a = ptr[lhs]->evaluate(ptr);
            auto b = ptr[rhs]->evaluate(ptr);
            switch(op) {
                case '+': { return a + b; }
                case '-': { return a - b; }
                case '*': { return a * b; }
                case '/': { return a / b; }
                default:  { throw std::runtime_error("unknown op"); }
            }
        }
    };
} // namespace

template<>
auto advent2022::day21() -> result {
    auto input = aoc::util::readlines("./source/2022/21/input.txt");

    aoc::util::hash hash;
    std::vector<node> nodes(input.size());

    aoc::dense::map<u64, node const*> ptr;
    aoc::dense::map<u64, node const*> parent;

    for (auto&& [i, line] : lz::enumerate(input)) {
        auto p = line.find(':');
        std::string_view s(line.data(), p);
        std::string_view r{&line[p+2], line.size()-p-2};
        std::function<i64(i64, i64)> f;

        u64 hash_lhs{};
        u64 hash_rhs{};
        char op{};
        i64 v{};

        if (std::isdigit(r[0])) {
            v = aoc::util::read<i64>(r);
            f = [v](i64, i64) { return v; };
        } else {
            auto res = scn::scan<std::string, char, std::string>(r, "{} {} {}")->values();
            op = std::get<1>(res);
            hash_lhs = hash(std::get<0>(res));
            hash_rhs = hash(std::get<2>(res));
        }

        node n{hash(s), op, v, hash_lhs, hash_rhs};
        nodes.push_back(n);
        parent.insert({hash_lhs, &nodes.back()});
        parent.insert({hash_rhs, &nodes.back()});
        ptr.insert({n.hash, &nodes.back()});
    }

    auto const* root = ptr[hash("root")];
    auto part1 = root->evaluate(ptr);

    auto const* me = ptr[hash("humn")];
    std::vector<u64> chain;
    auto h = me->hash;

    while(parent.find(h) != parent.end()) {
        chain.push_back(h);
        auto const* r = parent[h];
        if (r->hash == 0) { break; } // reached the root
        h = r->hash;
    }

    h = chain.back() == root->lhs ? root->rhs : root->lhs;
    auto v = ptr[h]->evaluate(ptr);

    for (auto i = std::ssize(chain) - 1; i > 0; --i) {
        auto const* p = ptr[chain[i]];
        auto left = chain[i-1] == p->lhs;
        h = left ? p->rhs : p->lhs;
        auto u = ptr[h]->evaluate(ptr);
        switch(p->op) {
            case '+': { v = v - u; break; }
            case '-': { v = left ? u + v : u - v; break; }
            case '*': { v = v / u; break; }
            case '/': { v = left ? u * v : u / v; break; }
        }
    }
    auto part2 = v;
    return aoc::result(part1, part2);
}
