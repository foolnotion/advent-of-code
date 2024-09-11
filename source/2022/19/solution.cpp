#include <aoc.hpp>

namespace {
    static auto constexpr N{4}; // NOLINT

    using blueprint = Eigen::Array<u8, N, N>;

    std::vector<std::string> const names{ "ore", "clay", "obsidian", "geode" };

    template<std::size_t TIME = 24>
    struct state {
        static constexpr auto time_limit{TIME};
        blueprint prices;
        u64 time{0};

        Eigen::Array<u8, N, 1> robots = decltype(robots)::Zero();
        Eigen::Array<u8, N, 1> budget = decltype(budget)::Zero();
        Eigen::Array<u8, N, 1> maxqnt = decltype(maxqnt)::Zero();

        std::optional<u8> building{std::nullopt};

        explicit state(blueprint p) : prices(std::move(p)) {
            for (auto i = 0; i < N; ++i) {
                auto j{0};
                prices.row(i).maxCoeff(&j);
                maxqnt[i] = prices(i, j);
            }
        }

        [[nodiscard]] auto geodes() const { return budget[N-1]; }

        auto tick() {
            // first collect resource from existing robots
            budget += robots;
            // then finish building a new robot
            if (building) {
                ++robots[*building];
                building = std::nullopt;
            }
            ++time;
            return *this;
        }

        [[nodiscard]] auto can_build(auto i) const {
            // cannot build if currenly building
            if (building) { return false; }

            // cannot build more than max amount of corresponding ore that I can spend per minute
            if (i < N-1 && robots[i] >=  maxqnt[i]) {
                return false;
            }

            // can build if I have all the mineral resources
            return (budget >= prices.col(i)).all();
        }

        [[nodiscard]] auto can_build() const {
            return std::ranges::count_if(lz::range(N), [&](auto i) { return can_build(i); });
        }

        auto build(auto i) {
            EXPECT(!building.has_value());
            auto q = *this;
            q.budget -= q.prices.col(i);
            q.building = std::make_optional(i);
            return q;
        }

        [[nodiscard]] auto neighbors() const {
            std::vector<state> nb;

            state p = *this;
            while(p.time < TIME && p.can_build() == 0) {
                p.tick();
            }

            for (auto i{0}; i < N; ++i) {
                if (!p.can_build(i)) { continue; }
                auto q = p;
                nb.push_back(q.build(i).tick());
            }
            if (p.can_build() < 2) {
                nb.push_back(p.tick());
            }

            return nb;
        }


        [[nodiscard]] auto hash() const -> u64 {
            aoc::util::hash h;
            std::span<u8 const> s1{robots.data(), (u64)robots.size()-1};
            std::span<u8 const> s2{budget.data(), (u64)budget.size()-1};
            return h(h(s1), h(s2));
        }

        friend auto operator<<(std::ostream& os, state<TIME> const& state) -> std::ostream& {
            os << "Minute:  " << state.time << "\n";
            os << "Robots:  " << state.robots.transpose() << "\n";
            os << "Build:   " << (state.building ? std::to_string(*state.building) : "none") << "\n";
            os << "Budget:  " << state.budget.transpose() << "\n";
            os << "Maxqnt:  " << state.maxqnt.transpose() << "\n";
            os << "Prices:\n" << state.prices << "\n";
            return os;
        }

        friend auto operator<(state<TIME> const& a, state<TIME> const& b) {
            return a.time < b.time;
        }
    };

    auto parse(auto const& input) {
        std::vector<blueprint> blueprints;
        for (auto const& line : input) {
            std::string_view v{line.begin(), line.end()};
            while(v.front() != ':') { v.remove_prefix(1); }
            v.remove_prefix(1);

            blueprint b;
            b.setConstant(0);

            for (auto&& [i, tok] : lz::enumerate(lz::split(v, '.'))) {
                if (tok.empty()) { continue; }

                if (tok.find("and") != std::string::npos) {
                    // auto scanstr{fmt::format(" Each {} robot costs {{}} {{}} and {{}} {{}}", name)};
                    auto res = scn::scan<scn::discard<std::string>, i32, std::string, i32, std::string>(tok, " Each {} robot costs {} {} and {} {}");
                    auto [_, price1, mineral1, price2, mineral2] = res->values();
                    auto j = std::distance(names.begin(), std::ranges::find(names, mineral1));
                    auto k = std::distance(names.begin(), std::ranges::find(names, mineral2));
                    b(j, i) = price1;
                    b(k, i) = price2;
                } else {
                    // auto scanstr{fmt::format(" Each {} robot costs {{}} {{}}", name)};
                    auto res = scn::scan<scn::discard<std::string>, i32, std::string>(tok, " Each {} robot costs {} {}");
                    auto [_, price1, mineral1] = res->values();
                    auto j = std::distance(names.begin(), std::ranges::find(names, mineral1));
                    b(j, i) = price1;
                }
            }
            blueprints.push_back(b);
        }
        return blueprints;
    }

    template<std::size_t TIME>
    auto evaluate_blueprint(blueprint const& b) {
        using state = state<TIME>;

        state s{b};
        s.robots << 1, 0, 0, 0; // start with one ore robot

        std::priority_queue<state> queue;
        queue.push(s);

        aoc::dense::set<u64> seen;

        state best = s;
        while (!queue.empty()) {
            auto p = queue.top();
            queue.pop();

            if (p.time > TIME) { continue; }

            if (auto [it, ok] = seen.insert(p.hash()); !ok) {
                continue;
            }

            if (p.time == state::time_limit) {
                if (best.geodes() < p.geodes()) {
                    best = p;
                }
                continue;
            }

            for (auto&& q : p.neighbors()) {
                queue.push(q);
            }
        }
        return best.budget[N-1];
    }
} // namespace

template<>
auto advent2022::day19() -> result {
    auto input = aoc::util::readlines("./source/2022/19/input.txt");
    auto blueprints = parse(input);
    auto range = lz::range(blueprints.size());

    auto constexpr minutes_p1{24};
    auto part1 = std::transform_reduce(range.begin(), range.end(), 0UL, std::plus{}, [&](auto i) {
        return evaluate_blueprint<minutes_p1>(blueprints[i]) * (i+1);
    });

    auto constexpr minutes_p2{32};
    auto part2 = std::transform_reduce(range.begin(), range.begin()+3, 1UL, std::multiplies{}, [&](auto i) {
        return evaluate_blueprint<minutes_p2>(blueprints[i]);
    });
    return aoc::result(part1, part2);
}
