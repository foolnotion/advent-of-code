#include <aoc.hpp>

namespace rg = std::ranges;

namespace {
struct reindeer_stats {
    u32 speed;
    u32 duration;
    u32 rest;
};

struct reindeer {
    reindeer_stats stats;
    u32 cooldown;
    u32 distance{0};
    bool resting{false};

    explicit reindeer(reindeer_stats s)
        : stats(s)
        , cooldown(s.duration) { }

    auto step() {
        if (!resting) { distance += stats.speed; }
        if (--cooldown == 0) {
            resting = !resting;
            cooldown = resting ? stats.rest : stats.duration;
        };
    }

    [[nodiscard]] auto run(u32 time) const {
        u32 distance = 0;
        for (auto t = u32{0}; t < time; t += stats.duration + stats.rest) {
            distance += stats.speed * std::min(stats.duration, time-t);
        }
        return distance;
    };
};
} // namespace

template<>
auto advent2015::day14() -> result {
    std::vector<std::string> input {
        "Dancer can fly 27 km/s for 5 seconds, but then must rest for 132 seconds",
        "Cupid can fly 22 km/s for 2 seconds, but then must rest for 41 seconds",
        "Rudolph can fly 11 km/s for 5 seconds, but then must rest for 48 seconds",
        "Donner can fly 28 km/s for 5 seconds, but then must rest for 134 seconds",
        "Dasher can fly 4 km/s for 16 seconds, but then must rest for 55 seconds",
        "Blitzen can fly 14 km/s for 3 seconds, but then must rest for 38 seconds",
        "Prancer can fly 3 km/s for 21 seconds, but then must rest for 40 seconds",
        "Comet can fly 18 km/s for 6 seconds, but then must rest for 103 seconds",
        "Vixen can fly 18 km/s for 5 seconds, but then must rest for 84 seconds",
    };

    std::vector<reindeer> vec;

    for (auto const& s : input) {
        std::string name;
        reindeer_stats stats{};
        auto result = scn::scan<std::string, u32, u32, u32>(s, "{} can fly {} km/s for {} seconds, but then must rest for {} seconds")->values();
        name = std::get<0>(result);
        stats.speed = std::get<1>(result);
        stats.duration = std::get<2>(result);
        stats.rest = std::get<3>(result);
        vec.emplace_back(stats);
    }

    constexpr int limit{2503};
    u32 distance{0};
    rg::for_each(vec, [&](auto const& deer) { distance = std::max(distance, deer.run(limit)); });

    std::vector<u64> points(vec.size(), 0);
    for (auto i = 0; i < limit; ++i) {
        rg::for_each(vec, &reindeer::step);
        auto best = rg::max_element(vec, std::less{}, &reindeer::distance)->distance;

        for (auto j = 0; j < std::ssize(vec); ++j) {
            points[j] += vec[j].distance == best;
        }
    }
    return aoc::result(distance, *rg::max_element(points));
}
