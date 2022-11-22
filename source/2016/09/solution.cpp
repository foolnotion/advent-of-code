#include <aoc.hpp>

namespace detail {
    auto length(std::string_view s, bool part2 = false) -> u64 { // NOLINT
        auto len{0UL};
        for (auto i = 0UL; i < std::ssize(s); ++i, ++len) {
            auto c = s[i];
            if (c == '(') {
                auto j = s.find(')', i);
                std::string_view v{&s[i], j-i};
                u64 a, b; // NOLINT
                (void)scn::scan(v, "({}x{})", a, b);
                auto c = part2 ? length({&s[j+1], a}, part2) : a;
                len += b * c - 1;
                i = j + a;
                continue;
            }
        }
        return len;
    };
} // namespace detail

template<>
auto advent2016::day09() -> result {
    auto input = aoc::util::readlines("./source/2016/09/input.txt").front();
    auto part1 = detail::length(input, /*part2=*/false);
    auto part2 = detail::length(input, /*part2=*/true);
    return aoc::result(part1, part2);
}
