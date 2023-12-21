#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

namespace {
    struct object {
        u64 id;
        mutable bool visited{false};
        std::vector<object*> in;
        std::vector<object*> out;
    };

    auto parse(auto const& input) {
        aoc::util::hash hash;
        aoc::dense::map<u64, u64> indices;
        std::vector<std::unique_ptr<object>> obj;

        auto get_object = [&](auto h) -> object* {
            auto [it, ok] = indices.insert({h, obj.size()});
            if (ok) {
                obj.push_back(std::move(std::make_unique<object>(h)));
            }
            return obj[it->second].get();
        };

        for (auto const& s : input) {
            auto p = static_cast<std::ptrdiff_t>(s.find(')'));
            std::string_view a(s.begin(), s.begin()+p);
            std::string_view b(s.begin()+p+1, s.end());

            auto* x = get_object(hash(a));
            auto* y = get_object(hash(b));

            x->out.push_back(y);
            y->in.push_back(x);
        }

        return obj;
    }

    auto count_orbits(auto const* p, i64 d = 0) -> i64 {
        auto f = [&](auto const* p) { return count_orbits(p, d+1); };
        return fp::transform_reduce(f, std::plus{}, d, p->out);
    };

    auto count_transfers(auto const* you, auto const* san) -> i64 {
        using state = std::pair<i64, object const*>;
        std::priority_queue<state, std::vector<state>, std::greater<>> queue;
        queue.emplace(0, you);

        while(!queue.empty()) {
            auto [c, p] = queue.top();
            queue.pop();

            if (p == san) { return c-2; }
            if (p->visited) { continue; }
            p->visited = true;

            for (auto const* q : p->in) {
                queue.emplace(c+1, q);
            }

            for (auto const* q : p->out) {
                queue.emplace(c+1, q);
            }
        }
        return -1;
    }
} // namespace

template<>
auto advent2019::day06() -> result {
    auto input = aoc::util::readlines("./source/2019/06/input.txt");
    auto obj = parse(input);

    auto const* root = std::ranges::find_if(obj, [](auto const& o) { return o->in.empty(); })->get();
    auto const* you = std::ranges::find_if(obj, [](auto const& o) { return o->id == aoc::util::hash{}("YOU"); })->get();
    auto const* san = std::ranges::find_if(obj, [](auto const& o) { return o->id == aoc::util::hash{}("SAN"); })->get();

    auto const p1 = count_orbits(root);
    auto const p2 = count_transfers(you, san);
    return aoc::result(p1, p2);
}