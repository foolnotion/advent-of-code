#include <aoc.hpp>

using std::views::split, std::views::transform;
using std::ranges::all_of,std::ranges::any_of, std::ranges::copy, std::ranges::count_if;

template<>
auto advent2024::day02() -> result {
    auto lines = aoc::util::readlines("./source/2024/02/input.txt");
    std::vector<std::vector<i64>> values;

    for (auto const& line : lines) {
        auto v = split(line, ' ') | transform([](auto x) { return std::string_view{x.begin(), x.end()}; }) | transform(aoc::util::read<i64>);
        values.emplace_back(v.begin(), v.end());
    }

    auto safe = [](std::vector<i64> const& values) {
        auto s = aoc::util::sgn(values[0]-values[1]);
        auto check = [&](auto i) {
            auto const v = values[i]-values[i+1];
            return aoc::util::sgn(v) == s && std::abs(v) >= 1 && std::abs(v) <= 3;
        };
        return all_of(std::views::iota(0L, std::ssize(values)-1), check);
    };

    auto safe_tol = [&safe](auto const& values) {
        std::vector<i64> tmp; tmp.reserve(values.size());
        return any_of(std::views::iota(0L, std::ssize(values)), [&](auto i) {
            tmp.clear();
            copy(values.begin(), values.begin()+i, std::back_inserter(tmp));
            copy(values.begin()+i+1, values.end(), std::back_inserter(tmp));
            return safe(tmp);
        });
    };

    auto const p1 = count_if(values, safe);
    auto const p2 = count_if(values, safe_tol);
    return aoc::result(p1, p2);
}
