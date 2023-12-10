#include <aoc.hpp>

namespace {
    struct blueprint {
        i64 value;
        bool move;
        char next;
    };

    auto parse(auto const& input) {
        char initial_state{};
        (void)scn::scan(input.front(), "Begin in state {}.", initial_state);
        i64 steps{};
        (void)scn::scan(*(input.begin()+1), "Perform a diagnostic checksum after {}} steps.", steps);

        std::array<std::array<blueprint, 2>, aoc::constants::alphabet.size()> map{};
        std::span rules{input.begin()+3, input.end()};

        const std::string right{"right."};
        const std::string left{"left."};

        for (auto it = rules.begin(); it < rules.end(); ++it) {
            if (it->empty()) { continue; }
            char current_state{};
            (void)scn::scan(*it++, "In state {}:", current_state);
            std::array<blueprint, 2> bp;
            std::string location;
            ++it;
            (void)scn::scan(*it++, "    - Write the value {}.", bp[0].value);
            (void)scn::scan(*it++, "    - Move one slot to the {}.", location);
            bp[0].move = location == right;
            (void)scn::scan(*it++, "    - Continue with state {}.", bp[0].next);
            ++it;
            (void)scn::scan(*it++, "    - Write the value {}.", bp[1].value);
            (void)scn::scan(*it++, "    - Move one slot to the {}.", location);
            bp[1].move = location == right;
            (void)scn::scan(*it++, "    - Continue with state {}.", bp[1].next);
            map[current_state-'A'] = bp;
        }
        return std::tuple{initial_state, steps, map};
    }
} // namespace

template<>
auto advent2017::day25() -> result {
    auto input = aoc::util::readlines("./source/2017/25/input.txt");
    auto [start, steps, map] = parse(input);

    char state{'A'};
    std::list<i64> values{0};
    std::deque<i64> tape;

    auto it = values.begin();
    for (auto i = 0; i < steps; ++i) {
        auto const& rule = map[state-'A'][*it];
        *it = rule.value;
        state = rule.next;

        if (rule.move) {
            it = std::next(it) == values.end()
                ? values.insert(std::next(it), 0)
                : std::next(it);
        } else {
            it = it == values.begin()
                ? values.insert(it, 0)
                : std::prev(it);
        }
    }
    auto const p1 = std::reduce(values.begin(), values.end());

    return aoc::result(p1, "");
}