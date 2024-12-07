#include <aoc.hpp>

namespace {
struct equation {
    u64 result;
    std::vector<u64> arguments;

    [[nodiscard]] auto solvable(u64 res, u64 i, bool part2) const -> bool {
        if (i == 0) { return res == arguments[i]; }
        auto x = arguments[i];
        auto d = 1; for (auto v = x; v != 0; v /= 10) { d *= 10; }
        return (res > x && solvable(res-x, i-1, part2)) ||
               (res % x == 0 && solvable(res/x, i-1, part2)) ||
               (part2 && res % d == x && solvable(res/d, i-1, part2));
    }

    [[nodiscard]] auto sum(bool part2) const -> u64 {
        return solvable(result, arguments.size()-1, part2) ? result : 0;
    }
};
} // namespace

template<>
auto advent2024::day07() -> result {
    auto lines = aoc::util::readlines("./source/2024/07/input.txt");
    std::vector<equation> eqs;

    for (auto const& line : lines) {
        auto const i = line.find(':');
        auto result = scn::scan_value<u64>(line.substr(0, i))->value();
        auto args_view = std::views::split(line.substr(i+2), ' ') |
                         std::views::transform([](auto x) {
                            return aoc::util::read<u64>({x.begin(), x.end()});
                         });
        eqs.push_back({result, {args_view.begin(), args_view.end()} });
    }

    std::array res{0UL, 0UL};
    for (auto const& eq : eqs) {
        res[0] += eq.sum(/*part2=*/false);
        res[1] += eq.sum(/*part2=*/true);
    }
    return aoc::result(res[0], res[1]);
}