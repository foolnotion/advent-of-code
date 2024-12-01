#include <aoc.hpp>
#include <util/md5.h>

template <>
auto advent2015::day04() -> result {
    constexpr auto input { "iwrupvqb" };
    std::array<u8, 16> buf{};

    auto zeros = [&](auto c) { return ((c & 0xF0) == 0) + ((c & 0x0F) == 0); };

    auto count = [&](auto i, auto n) {
        hashing::md5::hash_bs(input + std::to_string(i), buf.data());
        return std::transform_reduce(buf.begin(), buf.begin() + n/2 + n%2, 0UL, std::plus{}, zeros);
    };

    auto find = [&](u64 n) {
        return *std::ranges::find_if(std::views::iota(0), [&](auto i) { return count(i, n) >= n; });
    };

    constexpr auto n{5};
    return aoc::result(find(n), find(n+1));
}
