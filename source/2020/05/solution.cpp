#include <aoc.hpp>
#include <bitset>

template<>
auto advent2020::day05() -> result {
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
    std::sort(ids.begin(), ids.end());
    u32 seat_id = 0;
    for (size_t i = 1; i < ids.size(); ++i) {
        if (ids[i] - ids[i-1] == 2) {
            seat_id = ids[i] - 1;
            break;
        }
    }
    return aoc::result(max_id, seat_id);
}
