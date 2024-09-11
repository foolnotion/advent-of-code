#include <aoc.hpp>

namespace {
    struct game {
        static constexpr auto rmax{12};
        static constexpr auto gmax{13};
        static constexpr auto bmax{14};

        i64 id;
        i64 r;
        i64 g;
        i64 b;

        [[nodiscard]] auto valid() const {
            return (r > rmax || g > gmax || b > bmax) ? 0L : id;
        }

        [[nodiscard]] auto power() const {
            return r * g * b;
        }
    };
} // namespace

template<>
auto advent2023::day02() -> result {
    using namespace std::literals;
    auto input = aoc::util::readlines("./source/2023/02/input.txt");
    std::vector<game> games;
    games.reserve(input.size());

    aoc::util::hash hash{};
    constexpr auto red{hash("red")};
    constexpr auto green{hash("green")};
    constexpr auto blue{hash("blue")};

    for (auto const& s : input) {
        auto p = s.find(':');
        std::string_view u{s.begin(), s.begin()+p};
        auto id = scn::scan<i64>(u, "Game {}")->value();
        game m{id};

        std::string_view v{s.begin()+p+2, s.end()};
        for (auto const w : std::views::split(v, "; "sv)) {
            std::string_view sv{w.begin(), w.end()};
            i64 value{};
            std::string color;
            for (auto z : std::views::split(sv, ", "sv)) {
                auto [value, color] = scn::scan<i64, std::string>(z, "{} {}")->values();
                switch(aoc::util::hash{}(color)) {
                    case red:   { m.r = std::max(m.r, value); break; }
                    case green: { m.g = std::max(m.g, value); break; }
                    case blue:  { m.b = std::max(m.b, value); break; }
                }
            }
        }
        games.push_back(m);
    }
    auto const valid = games | std::views::transform(&game::valid);
    auto const power = games | std::views::transform(&game::power);
    auto const p1 = std::reduce(valid.begin(), valid.end());
    auto const p2 = std::reduce(power.begin(), power.end());
    return aoc::result(p1, p2);
}