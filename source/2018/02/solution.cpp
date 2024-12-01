#include <aoc.hpp>

template<>
auto advent2018::day02() -> result {
    auto lines = aoc::util::readlines("source/2018/02/input.txt");
    std::ranges::sort(lines);

    auto exactly_n = [](std::string_view sv, auto n) {
        constexpr auto s = aoc::constants::alphabet.size();
        std::array<u32, s> counts{};
        for (auto c : sv) {
            ++counts[c-'a'];
        }
        return std::ranges::any_of(counts, [n](auto c) { return c == n; });
    };
    auto exactly_two = [&](std::string_view sv) { return exactly_n(sv, 2U); };
    auto exactly_three = [&](std::string_view sv) { return exactly_n(sv, 3U); };

    auto common_letter = [](std::string_view s, std::string_view p) {
        ENSURE(std::ssize(s) == std::ssize(p));
        auto d = std::inner_product(s.begin(), s.end(), p.begin(), 0, std::plus<>{}, std::not_equal_to<>{});
        if (d == 1) {
            std::string result;
            result.reserve(std::size(s)-1);
            for (auto [a,b] : lz::zip(s, p)) {
                if (a == b) { result += a; }
            }
            return result;
        }
        return std::string{};
    };

    auto a{0U};
    auto b{0U};

    std::string p2;
    for (auto const& [i, line] : lz::enumerate(lines)) {
        a += exactly_two(line);
        b += exactly_three(line);

        if (i < std::ssize(line)-1) {
            if (auto s = common_letter(line, lines[i+1UL]); !s.empty()) {
                p2 += s;
            }
        }
    }
    auto const p1{a*b};

    return aoc::result(p1, p2);
}