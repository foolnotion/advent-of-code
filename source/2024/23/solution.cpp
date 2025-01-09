#include <aoc.hpp>

namespace rs = std::ranges;
namespace vs = std::views;

namespace {
    struct node {
        u16 id;
        aoc::dense::set<node const*> connections;

        [[nodiscard]] auto count() const -> i32 {
            std::deque<u16> deq{id};
            std::function<i32(node const*, i32)> find = [&](node const* current, i32 depth) -> i32 {
                if (depth < 2 && (rs::find(deq, current->id) != deq.end())) { return 0; }
                if (depth == 2) { return current == this && rs::any_of(deq, [](auto const& s) { return s / 100 == 19; }); }

                deq.push_back(current->id);
                auto count = 0;
                for (auto const* c : current->connections) {
                    if (c->id < current->id) { continue; }
                    count += find(c, depth+1);
                }
                deq.pop_back();
                return count;
            };
            auto f = [&](node const* n) { return find(n, 0); };
            return std::transform_reduce(connections.begin(), connections.end(), 0, std::plus{}, f);
        }

        [[nodiscard]] auto name() const {
            std::string s;
            s.push_back(static_cast<char>(id / 100 + 'a'));
            s.push_back(static_cast<char>(id % 100 + 'a'));
            return s;
        }
    };

    struct graph {
        aoc::dense::map<u16, std::unique_ptr<node>> nmap;

        auto add(u16 id) -> node* {
            auto [it, _] = nmap.insert({id, std::make_unique<node>(id)});
            return it->second.get();
        };

        auto add(u16 a, u16 b) -> void {
            auto* na = add(a);
            auto* nb = add(b);
            na->connections.insert(nb);
            nb->connections.insert(na);
        }

        [[nodiscard]] auto nodes() const {
            return nmap | vs::transform([](auto const& p) { return p.second.get(); });
        }

        [[nodiscard]] auto max_clique() const -> std::vector<node const*> {
            auto nodes = this->nodes();
            std::vector<node const*> max_clique;
            for (auto const* n : nodes) {
                std::vector<node const*> clique{n};
                for (auto const* m : nodes) {
                    if (n == m) { continue; }
                    auto not_adjacent = [&](auto const* c) { return !m->connections.contains(c); };
                    if (rs::any_of(clique, not_adjacent)) { continue; }
                    clique.push_back(m);
                }
                if (clique.size() > max_clique.size()) {
                    std::swap(clique, max_clique);
                }
            }
            rs::sort(max_clique, std::less{}, &node::id);
            return max_clique;
        }
    };
} // namespace

template<>
auto advent2024::day23() -> result {
    auto const lines = aoc::util::readlines("./source/2024/23/input.txt");
    graph g;

    // parse nodes
    auto encode = [](std::string const& label) {
        return static_cast<u16>(label[0]-'a') * 100 + static_cast<u16>(label[1]-'a');;
    };
    for (auto const& line : lines) {
        auto a = line.substr(0, 2);
        auto b = line.substr(3);
        g.add(encode(a), encode(b));
    }
    auto const nodes = g.nodes();
    auto const p1 = std::transform_reduce(nodes.begin(), nodes.end(), 0, std::plus{}, std::mem_fn(&node::count));
    auto max_clique = g.max_clique();
    auto const p2 = fmt::format("{}", fmt::join(max_clique | vs::transform(&node::name), ","));
    return aoc::result(p1, p2);
}