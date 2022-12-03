#include <aoc.hpp>

namespace detail {
struct instructions {
    using hash = aoc::util::hash;
    static constexpr auto cpy { hash{}("cpy") };
    static constexpr auto inc { hash{}("inc") };
    static constexpr auto dec { hash{}("dec") };
    static constexpr auto jnz { hash{}("jnz") };
};

auto run(auto const& input, auto reg)
{
    aoc::util::hash h;
    for (auto i = 0; i < std::ssize(input); ++i) {
        auto const& tokens = input[i];
        auto c = tokens[1].front();
        std::span instruction{tokens[0]};
        switch (h(instruction)) {
        case instructions::inc: {
            ++reg[c - 'a'];
            break;
        }
        case instructions::dec: {
            --reg[c - 'a'];
            break;
        }
        case instructions::cpy: {
            auto a = std::isalpha(c) ? reg[c - 'a'] : scn::scan_value<i32>(tokens[1]).value();
            reg[tokens[2].front() - 'a'] = a;
            break;
        }
        case instructions::jnz: {
            auto a = std::isalpha(c) ? reg[c - 'a'] : scn::scan_value<i32>(tokens[1]).value();
            if (a != 0) {
                auto d = tokens[2].front();
                auto b = std::isalpha(d) ? reg[d - 'a'] : scn::scan_value<i32>(tokens[2]).value();
                i += b - 1;
            }
            break;
        }
        }
    }
    return reg.front();
}
} // namespace detail

template <>
auto advent2016::day12() -> result
{
    auto lines = aoc::util::readlines("./source/2016/12/input.txt");
    auto input = lz::map(lines, [](auto const& s) { return lz::split(s, ' ').toVector(); }).toVector();
    auto part1 = detail::run(input, std::array{0,0,0,0});
    auto part2 = detail::run(input, std::array{0,0,1,0});
    return aoc::result(part1, part2);
}
