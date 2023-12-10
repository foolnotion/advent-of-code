#include <aoc.hpp>

namespace {
    struct port {
        i64 value{};
        bool free{true};

        [[nodiscard]] auto matches(port p) const {
            return free && p.free && value == p.value;
        }
    };

    struct component {
        std::array<port, 2> ports;
        std::vector<component*> compatible_components;

        [[nodiscard]] auto can_connect() const {
            return std::ranges::any_of(ports, [](auto p) { return p.free; });
        }

        [[nodiscard]] auto compatible_with(component const& x) const {
            for (auto p : ports) {
                for (auto q : x.ports) {
                    if (p.matches(q)) { return true; }
                }
            }
            return false;
        }

        [[nodiscard]] auto sum() const {
            return ports[0].value + ports[1].value;
        }
    };

    auto parse(auto const& input) {
        std::vector<component> components;
        components.reserve(input.size());
        for (auto const& s : input) {
            component c;
            (void)scn::scan(s, "{}/{}", c.ports[0].value, c.ports[1].value);
            for (auto& p : c.ports) {
                p.free = p.value != 0;
            }
            components.push_back(c);
        }

        // create a map of matching ports
        for (auto i = 0; i < std::ssize(components); ++i) {
            auto& u = components[i];
            for (auto j = 0; j < std::ssize(components); ++j) {
                if (i == j) {
                    continue;
                }
                auto& v = components[j];
                if (u.compatible_with(v)) {
                    u.compatible_components.push_back(&v);
                }
            }
        }
        return components;
    }
} // namespace

template<>
auto advent2017::day24() -> result {
    auto input = aoc::util::readlines("./source/2017/24/input.txt");
    auto components = parse(input);

    auto max_strength{0L};
    auto max_length{0L};
    auto max_strength_longest{0L};

    auto dfs = [&](auto& c, i64 s, i64 l, auto&& dfs) -> void {
        s += c.sum();
        l += 1;

        max_strength = std::max(s, max_strength);
        max_length   = std::max(l, max_length);
        if (l == max_length) {
            max_strength_longest = std::max(max_strength_longest, s);
        }

        if (!c.can_connect()) { return; }

        for (auto* d : c.compatible_components) {
            if (!d->can_connect()) { continue; }
            for (auto& p : c.ports) {
                for (auto& q : d->ports) {
                    if (p.matches(q)) {
                        p.free = q.free = false;
                        dfs(*d, s, l, dfs);
                        p.free = q.free = true;
                    }
                }
            }
        }
    };

    for (auto& c : components) {
        if (std::ranges::any_of(c.ports, [](auto p){ return p.value == 0; })) {
            dfs(c, 0, 0, dfs);
        }
    }
    auto const p1 = max_strength;
    auto const p2 = max_strength_longest;
    return aoc::result(p1, p2);
}