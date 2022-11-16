#include <aoc.hpp>
#include <functional>

template <>
auto advent2016::day04() -> result {
    auto input = aoc::util::readlines("./source/2016/04/input.txt");

    auto part1{0};
    auto part2{0};
    for (auto const& s : input) {
        auto a = s.find_last_of('-');
        auto b = s.find_first_of('[');
        std::string_view room { s.data(), a };
        std::string_view id { &s[a + 1], b - a - 1 };
        std::string_view code { &s[b + 1], s.size() - b - 2 };

        auto let = aoc::constants::alphabet;
        std::array<u32, let.size()> counts{};
        std::fill(counts.begin(), counts.end(), 0);
        for (auto c : lz::filter(room, std::not_fn(aoc::equals<'-'>))) {
            ++counts[c-'a'];
        }
        std::ranges::sort(let, [&](auto lhs, auto rhs) {
            return std::tie(counts[rhs-'a'], lhs) < std::tie(counts[lhs-'a'], rhs);
        });
        auto const zipped = lz::zip(std::span{let.data(), code.size()}, code);
        auto id_value = scn::scan_value<int>(id).value();

        if (std::ranges::all_of(zipped, [](auto t) { auto [a,b] = t; return a == b; })) {
            part1 += id_value;
            // part 2
            std::string name;
            for (auto c : room) {
                if (c == '-') { break; }
                name.push_back(aoc::constants::alphabet[(c - 'a' + id_value) % let.size()]);
            }
            if (name == "northpole") {
                part2 = id_value;
            }
        }
    }

    return aoc::result(part1, part2);
}
