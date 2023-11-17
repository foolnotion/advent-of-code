#include <aoc.hpp>

namespace detail {
    struct bot {
        u64 id{};
        mutable u64 count{};
        mutable std::array<u64, 2> values;

        auto operator=(u64 v) const -> bot const& {
            values[count++] = v;
            return *this;
        }

        auto operator*() const {
            count = 0;
            auto [a, b] = values;
            return std::tuple{std::min(a, b), std::max(a, b)};
        }
    };
} // namespace detail

template<>
auto advent2016::day10() -> result {
    auto hash = [](auto const& bot) { return bot.id; };
    auto equals = [](auto const& a, auto const& b) { return a.id == b.id; };

    aoc::dense::set<detail::bot, decltype(hash), decltype(equals)> bots;
    aoc::dense::map<u64, u64> outputs;

    auto input = aoc::util::readlines("./source/2016/10/input.txt");
    std::ranges::stable_sort(input, [](auto const& a, auto const& b) { return a[0] > b[0]; });
    auto pt = std::ranges::partition_point(input, [](auto const& a) { return a[0] != 'b';});

    for (auto const& s : std::span{input.begin(), pt}) {
        u64 v, b; // NOLINT
        (void)scn::scan(s, "value {} goes to bot {}", v, b);
        detail::bot bot{b};
        *bots.insert(bot).first = v;
    }

    auto check_p1 = [](auto bot) {
        constexpr std::tuple goal{17, 61};
        return *bot == goal;
    }; // NOLINT

    auto part1{0UL};
    auto give = [&](auto value, auto target, bool output) {
        if (output) { outputs[target] = value; return; }
        auto& taker = *bots.insert({target}).first;
        taker = value;
        if(!part1 && check_p1(taker)) { part1 = taker.id; }
    };

    bool changed{true};
    std::span instructions{pt, input.end()};
    while(changed) {
        changed = false;
        for (auto const& s : instructions) {
            u64 a, b, c; // NOLINT
            std::string x, y; // NOLINT
            (void)scn::scan(s, "bot {} gives low to {} {} and high to {} {}", a, x, b, y, c);
            if (auto& giver = *bots.insert({a}).first; giver.count == 2) {
                changed = true;
                auto [lo, hi] = *giver;
                give(lo, b, x[0] == 'o');
                give(hi, c, y[0] == 'o');
            }
        }
    }

    auto part2 = outputs[0] * outputs[1] * outputs[2];
    return aoc::result(part1, part2);
}
