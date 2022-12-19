#include <aoc.hpp>

namespace detail {
    static auto constexpr N{4}; // NOLINT

    using blueprint = Eigen::Array<i32, N, N>;

    template<std::size_t T = 24>
    struct state {
        blueprint prices;
        u64 time{0};

        Eigen::Array<i32, N, 1> robots = decltype(robots)::Zero();
        Eigen::Array<i32, N, 1> budget = decltype(budget)::Zero();
        Eigen::Array<i32, N, 1> bqueue = decltype(budget)::Zero();

        std::optional<i32> building{std::nullopt};

        auto tick() {
            if (building) {
                ++robots[*building];
                building = std::nullopt;
            }
            for (auto&& [i, q] : lz::enumerate(robots)) {
                budget[i] += q;
            }
            ++time;
            return *this;
        }

        auto neighbors() const {
            std::vector<state> neighbors;
            auto q = *this; // collect resource and do nothing
            neighbors.push_back(q);

            for (auto i : robots) {
                if (!can_build(i)) { continue; }
                auto q = *this;
                neighbors.push_back(q.build(i));
            }

            return neighbors;
        }

        [[nodiscard]] auto can_build(auto i) const {
            // can build if not already building and if enough resources
            return !building && (budget - prices.col(i) >= 0).all();
        }

        auto build(auto i) {
            auto q = *this;
            q.budget -= q.prices.col(i);
            building = std::make_optional(i);
            return q;
        }

        auto hash() const {
            aoc::util::hash h;
            return h(h(robots), h(budget), time);
        }

        friend auto operator<<(std::ostream& os, state<T> const& state) -> std::ostream& {
            os << "Minute: " << state.time << "\n";
            os << "Robots: " << state.robots.transpose() << "\n";
            os << "Budget: " << state.budget.transpose() << "\n";
            os << "Prices:\n" << state.prices << "\n";
            return os;
        }
    };

    auto parse(auto const& input) {
        std::vector<std::string> names{ "ore", "clay", "obsidian", "geode" };

        std::vector<blueprint> blueprints;
        for (auto const& line : input) {
            std::string_view v{line.begin(), line.end()};
            while(v.front() != ':') { v.remove_prefix(1); }
            v.remove_prefix(1);

            blueprint b;
            b.setConstant(0);

            for (auto&& [i, tok] : lz::enumerate(lz::split(v, '.'))) {
                if (tok.empty()) { continue; }
                std::string mineral1, mineral2; // NOLINT
                i32 price1, price2; // NOLINT

                auto const& name = names[i];

                if (tok.find("and") != std::string::npos) {
                    auto scanstr{fmt::format(" Each {} robot costs {{}} {{}} and {{}} {{}}", name)};
                    auto res = scn::scan(tok, scanstr, price1, mineral1, price2, mineral2);
                    auto j = std::distance(names.begin(), std::ranges::find(names, mineral1));
                    auto k = std::distance(names.begin(), std::ranges::find(names, mineral2));
                    b(i, j) = price1;
                    b(i, k) = price2;
                } else {
                    auto scanstr{fmt::format(" Each {} robot costs {{}} {{}}", name)};
                    auto res = scn::scan(tok, scanstr, price1, mineral1);
                    auto j = std::distance(names.begin(), std::ranges::find(names, mineral1));
                    b(i, j) = price1;
                }
            }
            blueprints.push_back(b);
        }
        return blueprints;
    }
} // namespace detail

template<>
auto advent2022::day19() -> result {
    using state = detail::state<5>;

    auto input = aoc::util::readlines("./source/2022/19/sample.txt");
    auto blueprints = detail::parse(input);

    for (auto const& b : blueprints) {
        std::cout << b << "\n\n";
    }

    auto cmp = [](auto const& a, auto const& b) {
        return a.time > b.time;
    };

    state s{blueprints.front()};
    s.robots << 1, 0, 0, 0; // start with one ore robot

    std::priority_queue<state, std::vector<state>, decltype(cmp)> queue;


    while (!queue.empty()) {
    }

    return aoc::result(0,0);
}
