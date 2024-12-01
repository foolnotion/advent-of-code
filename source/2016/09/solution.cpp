#include <aoc.hpp>

namespace {
    auto length(std::string_view s, bool part2 = false) -> u64 { // NOLINT
        auto len{0UL};
        for (auto i = 0L; i < std::ssize(s); ++i, ++len) {
            auto c = s[i];
            if (c == '(') {
                auto j = s.find(')', i);
                std::string_view v{&s[i], j-i+1};
                auto [a, b] = scn::scan<u64, u64>(v, "({}x{})")->values();
                auto c = part2 ? length({&s[j+1], a}, part2) : a;
                len += b * c - 1;
                i = j + a;
                continue;
            }
        }
        return len;
    };
} // namespace

template<>
auto advent2016::day09() -> result {
    auto input = aoc::util::readlines("./source/2016/09/input.txt").front();
    auto part1 = length(input, /*part2=*/false);
    auto part2 = length(input, /*part2=*/true);
    return aoc::result(part1, part2);
}
