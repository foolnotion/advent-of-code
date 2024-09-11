#include <aoc.hpp>

namespace rs = std::ranges;

namespace {
    enum class pulse { low = 0, high = 1};
    enum class module_type { flip_flop = 0, conjunction = 1, broadcast = 2 };

    struct module {
        auto reset() const -> void {
            state = false;
            for (auto& p : in) {
                std::get<1>(p) = pulse::low;
            }
        }

        auto receive(module const* src, pulse p) const -> std::optional<pulse> {
            switch(type) {
                case module_type::flip_flop: {
                    if (p == pulse::low) {
                        state = !state;
                        return state ? pulse::high : pulse::low;
                    }
                    return {};
                }
                case module_type::conjunction: {
                    auto s = rs::find_if(in, [&](auto& p) { return std::get<0>(p) == src; });
                    std::get<1>(*s) = p;
                    state = rs::any_of(in, [](auto const& t) { return std::get<1>(t) == pulse::low; });
                    return state ? pulse::high : pulse::low;;
                }
                case module_type::broadcast: {
                    return {};
                }
                default: {
                    throw std::runtime_error("unkown type");
                }
            }
        }
        module_type type{};
        std::string name;
        std::vector<module*> out;

        // mutable state which gets updated upon receiving a pulse
        mutable bool state{false}; // off/on for flip_flop, all inputs high for conjunction
        mutable std::vector<std::tuple<module const*, pulse>> in; // inputs
    };

    struct button {
        using item = std::pair<module const*, pulse>;

        module const* broadcast{};
        module const* rx{};

        std::deque<item> queue;
        aoc::dense::map<module const*, u64> cycles;

        u64 hi{0};
        u64 lo{0};

        auto press(auto n) -> u64 {
            for (auto i = 0; i < n; ++i) {
                (*this)(i);
            }
            return lo * hi;
        }

        auto count() -> u64 {
            auto [parent, _] = rx->in.front();

            for(auto i{0UL}; cycles.size() < parent->in.size(); ++i) {
                (*this)(i);
            }
            auto lcm{1UL};
            for (auto [x, c] : cycles) {
                lcm = std::lcm(lcm, c);
            }
            return lcm;
        }

        auto operator()(u64 i) -> void {
            ++lo;
            queue.emplace_back(broadcast, pulse::low);

            auto const* parent = std::get<0>(rx->in.front());

            while (!queue.empty()) {
                auto [m, p] = queue.front();
                queue.pop_front();

                if (m == parent) {
                    for (auto [x, p] : m->in) {
                        if (p == pulse::high) {
                            cycles.insert({x, i+1});
                        }
                    }
                }

                for (auto const* d : m->out) {
                    hi += p == pulse::high;
                    lo += p == pulse::low;
                    if (auto q = d->receive(m, p); q) {
                        queue.emplace_back(d, *q);
                    }
                }
            }
        }
    };

    auto parse(auto const& input) {
        aoc::util::hash hash;
        auto module_type = [](char c) {
            if (c == '%') { return module_type::flip_flop; }
            if (c == '&') { return module_type::conjunction; }
            if (c == 'b') { return module_type::broadcast; }
            throw std::runtime_error("unknown module type");
        };

        aoc::dense::map<u64, u64> indices;
        std::vector<std::unique_ptr<module>> modules;

        auto get_module = [&](std::string_view v) -> module* {
            auto h = hash(v);
            auto [it, ok] = indices.insert({h, modules.size()});
            if (ok) {
                modules.push_back(std::make_unique<module>());
            }
            auto* m = modules[it->second].get();
            m->name = v;
            return m;
        };

        for (auto i = 0UL; i < input.size(); ++i) {
            auto const& s = input[i];
            auto p = s.find(' ');
            auto q = s.find('>') + 2;

            auto m = get_module(std::string_view{s.begin()+(s[0]!='b'), s.begin()+p});
            m->type = module_type(s[0]);

            for (auto const x : std::string_view{s.begin()+q, s.end()} | std::views::split(',')) {
                std::string_view v{x.begin(), x.end()};
                while(std::isspace(v.front())) { v.remove_prefix(1); }
                auto* n = get_module(v);
                m->out.push_back(n);
                n->in.emplace_back(m, pulse::low);
            }
        }
        return modules;
    }
} // namespace

template<>
auto advent2023::day20() -> result {
    auto input = aoc::util::readlines("./source/2023/20/input.txt");
    auto modules = parse(input);

    auto find_module = [&](auto const& name) {
        auto it = rs::find_if(modules, [&](auto const& m) { return m->name == name; });
        ENSURE(it != modules.end());
        return it->get();
    };

    constexpr auto n{1000};
    auto const* b = find_module("broadcaster");
    auto const* x = find_module("rx");
    button btn{b, x};
    auto const p1 = btn.press(n);

    rs::for_each(modules, [](auto const& m) { m->reset(); });
    auto const p2 = btn.count();
    return aoc::result(p1, p2);
}
