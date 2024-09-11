#include <aoc.hpp>

template<>
auto advent2016::day20() -> result {
    using interval = aoc::math::interval<uint32_t>;
    auto input = aoc::util::readlines("./source/2016/20/input.txt");
    std::ranges::sort(input);
    std::vector<interval> intervals;
    intervals.reserve(input.size());

    for (auto const& s : input){
        auto [a, b] = scn::scan<u32, u32>(s, "{}-{}")->values();
        intervals.push_back({a, b});
    }

    auto compare = [](auto const& a, auto const& b) {
        return a.as_tuple() < b.as_tuple();
    };

    std::ranges::sort(intervals, compare);
    std::vector<interval> allowed;
    auto m = std::numeric_limits<uint32_t>::max();
    for (auto i = 0; i < std::ssize(intervals)-1; ++i) {
        auto j = i+1;
        while (intervals[i].intersects(intervals[j]) || (intervals[i].b < m && intervals[i].b+1 == intervals[j].a)) {
            intervals[j] = intervals[i] | intervals[j];
            i = j++;
        }
        auto [a, b] = intervals[i];
        auto [c, d] = intervals[i+1];
        if (b < m) {
            allowed.push_back({b+1, c-1});
        }
    }
    auto part1 = allowed.front().a;
    auto part2 = std::transform_reduce(allowed.begin(), allowed.end(), 0, std::plus{}, [](auto iv) { return iv.width(); });
    return aoc::result(part1, part2);
}