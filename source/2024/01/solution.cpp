#include <aoc.hpp>

template<>
auto advent2024::day01() -> result {
    auto lines = aoc::util::readlines("./source/2024/01/input.txt");
    std::vector<i64> u; u.reserve(lines.size());
    std::vector<i64> v; v.reserve(lines.size());
    aoc::dense::map<i64, i64> m;

    for (auto const& line : lines) {
        if (auto res = scn::scan<i64, i64>(line, "{}    {}"); res) {
            auto [a, b] = res->values();
            u.push_back(a);
            v.push_back(b);
            auto [it, _] = m.insert({b, 0});
            ++it->second;
        }
    }
    std::ranges::stable_sort(u);
    std::ranges::stable_sort(v);

    auto distance  = [&](auto i) { return std::abs(u[i] - v[i]); };
    auto sim_score = [&](auto x) {
        if (auto it = m.find(x); it != m.end()) { return x * it->second; }
        return 0L;
    };

    auto i = std::views::iota(0L, std::ssize(u));
    auto const p1 = std::transform_reduce(i.begin(), i.end(), 0L, std::plus{}, distance);
    auto const p2 = std::transform_reduce(u.begin(), u.end(), 0L, std::plus{}, sim_score);
    return aoc::result(p1, p2);
}