#include <aoc.hpp>

using std::ranges::any_of;

template<>
auto advent2020::day09() -> result {
    auto input = aoc::util::readlines("./source/2020/09/input.txt");
    auto vec = lz::map(input, [](auto const& s) { return scn::scan_value<u64>(s)->value(); }).toVector();
    constexpr auto n{25};

    auto part1 = [&]() -> std::optional<u64> {
        aoc::dense::set<u64> set(vec.begin(), vec.begin() + n);
        for (auto i = n; i < std::ssize(vec); ++i) {
            auto v = vec[i];
            if (any_of(std::span{&vec[i-n], n}, [&](auto x) { return set.contains(v-x); })) {
                set.erase(v-n);
                set.insert(v);
                continue;
            }
            return { vec[i] };
        }
        return std::nullopt;
    };

    auto part2 = [&](u64 target) -> std::optional<u64> {
        auto l = vec.begin();
        auto r = vec.begin();
        auto total = u64{0};

        while ((total != target || r - l < 2) && l < vec.end() && r < vec.end()) {
            total = total < target ? total + *r++ : total - *l++;
        }
        auto [min, max] = std::minmax_element(l, r);
        return { *min + *max };
    };

    auto p1 = part1();
    auto p2 = part2(p1.value());
    return aoc::result(p1.value(), p2.value());
}
