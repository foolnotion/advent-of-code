#include <aoc.hpp>
#include <bits/ranges_algo.h>

using std::ranges::find_if;
using std::ranges::binary_search;

namespace detail {
auto find(auto const& vec, u32 n, u32 s, u32 p=1) -> std::optional<int64_t>
{
    if (s <= 0) { return std::nullopt; }
    if (n == 2) {
        auto it = find_if(vec, [&](auto x) { return binary_search(vec, s-x); });
        if (it != vec.end()) { return { *it * (s - *it) * p }; }
    } else if (n > 2) {
        for (auto x : lz::filter(vec, [s](auto x) { return !(x > s); })) {
            if (auto res = find(vec, n-1, s-x, x*p); res.has_value()) {
                return res;
            }
        }
    }
    return std::nullopt;
}
} // namespace detail

template<>
auto advent2020::day01() -> result {
    auto input = aoc::util::readlines("./source/2020/01/input.txt");
    auto values = lz::map(input, [](auto const& s) { return scn::scan_value<u32>(s).value(); }).toVector();
    std::sort(values.begin(), values.end());
    constexpr auto sum{2020};
    auto p1 = detail::find(values, 2, sum).value();
    auto p2 = detail::find(values, 3, sum).value();
    return aoc::result(p1, p2);
}
