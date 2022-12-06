#include <aoc.hpp>

template<>
auto advent2022::day06() -> result {
    auto input = aoc::util::readlines("./source/2022/06/input.txt").front();
    constexpr auto start_marker{4UL};
    constexpr auto state_marker{14UL};

    auto find_marker = [&](auto const& input, auto marker_size) {
        EXPECT(input.size() >= marker_size);
        auto range = lz::range(input.size() - marker_size);
        return marker_size + std::distance(range.begin(), std::ranges::find_if(range, [&](auto i) {
            std::string_view s{&input[i], marker_size};
            u64 mask{0}; // need at most 14 bits
            for (auto c : s) { mask |= 1UL << static_cast<u8>(c - 'a'); }
            return std::popcount(mask) == marker_size;
        }));
    };
    return aoc::result(find_marker(input, start_marker), find_marker(input, state_marker));
}
