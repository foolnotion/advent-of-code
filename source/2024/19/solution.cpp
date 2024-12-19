#include <aoc.hpp>

template<>
auto advent2024::day19() -> result {
    auto const lines = aoc::util::readlines("./source/2024/19/input.txt");
    aoc::dense::map<char, std::vector<std::string_view>> designs;
    for (auto const x : std::views::split(lines.front(), ',')) {
        std::string_view sv{x.begin(), x.end()};
        if (sv.front() == ' ') { sv.remove_prefix(1); }
        designs[sv.front()].push_back(sv);
    }
    aoc::dense::map<std::string_view, i64> counts;
    std::function<i64(std::string_view)> count = [&](std::string_view s) -> i64 {
        if (s.empty()) { return 1L; }
        if (auto it = counts.find(s); it != counts.end()) {
            return it->second;
        }
        auto const& v = designs[s.front()];
        auto f = [&](auto const d) { return d == s.substr(0, d.size()) ? count(s.substr(d.size())) : 0; };
        return counts[s] = std::transform_reduce(v.begin(), v.end(), 0L, std::plus{}, f);
    };
    auto v = lines | std::views::drop(2);
    auto const p1 = std::transform_reduce(v.begin(), v.end(), 0UL, std::plus{}, [&](auto const& x) { return count(x) > 0; });
    auto const p2 = std::transform_reduce(v.begin(), v.end(), 0UL, std::plus{}, count);
    return aoc::result(p1, p2);
}
