#include <aoc.hpp>

namespace {
struct monkey {
    std::vector<u64> worry;
    char op;
    auto update(auto w) const {
        auto v = val == 0 ? w : val;
        if (op == '+') { return w + v; }
        if (op == '*') { return w * v; }
        throw std::runtime_error("unknown op");
    }
    u64 val{};
    u64 div{};
    i32 id_true{};
    i32 id_false{};
    u64 inspected{0};
};
} // namespace

template <>
auto advent2022::day11() -> result
{
    auto input = aoc::util::readlines("./source/2022/11/input.txt");

    std::vector<monkey> monkeys;
    for (auto i = 0L; i < std::ssize(input); i += 7L) { // NOLINT
        auto const& s = input[i + 1L];
        auto j = 0;
        while (!std::isdigit(s[j])) { ++j; }
        std::string_view sv { &s[j], s.size() - j };
        auto worry = lz::map(lz::split(sv, ','), aoc::util::read<u64>).toVector();
        auto [op, arg] = scn::scan<char, std::string>(input[i + 2], "  Operation: new = old {} {}")->values();
        auto val = arg == "old" ? 0 : aoc::util::read<u64>(arg);
        // if (arg == "old") {
            // val = 0;
        // }
        auto div = scn::scan<u64>(input[i + 3], "  Test: divisible by {}")->value();
        auto id_true = scn::scan<i32>(input[i + 4], "    If true: throw to monkey {}")->value();
        auto id_false = scn::scan<i32>(input[i + 5], "    If false: throw to monkey {}")->value(); // NOLINT
        monkey m{std::move(worry), op, val, div, id_true, id_false};
        monkeys.push_back(m);
    }

    constexpr auto rounds_p1 { 20 };
    constexpr auto rounds_p2 { 10'000 };
    constexpr auto den_p1{3};
    constexpr auto den_p2{1};
    auto mod = std::transform_reduce(monkeys.begin(), monkeys.end(), 1UL, std::multiplies{}, [](auto const& m){ return m.div; });

    auto monkey_business = [](auto monkeys, auto rounds, auto den, auto mod) {
        for (auto i = 1; i <= rounds; ++i) {
            for (auto& m : monkeys) {
                m.inspected += m.worry.size();
                for (auto w : m.worry) {
                    w = m.update(w) / den;
                    if (mod != 1) { w %= mod; }
                    auto id = w % m.div == 0 ? m.id_true : m.id_false;
                    monkeys[id].worry.push_back(w);
                }
                m.worry.clear();
            }
        }
        std::ranges::sort(monkeys, std::greater{}, &monkey::inspected);
        return monkeys[0].inspected * monkeys[1].inspected;
    };

    auto part1 = monkey_business(monkeys, rounds_p1, den_p1, 1);
    auto part2 = monkey_business(monkeys, rounds_p2, den_p2, mod);
    return aoc::result(part1, part2);
}
