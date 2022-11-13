#include <aoc.hpp>
#include <bitset>

template<>
auto advent2020::day05() -> void {
    auto input = aoc::util::readlines("./source/2020/05/input.txt");

    u32 max_id = 0;
    std::vector<u32> ids;
    for (auto const& line : input) {
        std::bitset<10> bits;
        size_t i = 10;
        for (auto c : line) {
            bits[--i] = (c == 'R' || c == 'B');
        }
        u32 id = bits.to_ulong();
        max_id = std::max(id, max_id);
        ids.push_back(id);
    }
    fmt::print("part 1: {}\n", max_id);
    std::sort(ids.begin(), ids.end());
    u32 seat_id = 0;
    for (size_t i = 1; i < ids.size(); ++i) {
        if (ids[i] - ids[i-1] == 2) {
            seat_id = ids[i] - 1;
            break;
        }
    }
    fmt::print("part 2: {}\n", seat_id);
}
