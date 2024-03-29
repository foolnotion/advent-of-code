#include <aoc.hpp>
#include <util/md5.h>

template <>
auto advent2015::day04() -> result {
    using brumme::MD5;
    constexpr auto input { "iwrupvqb" };
    std::array<u8, 16> buf{};

    auto count = [&](int n) -> std::optional<i64> {
        for (auto i = 0;; ++i) {
            auto const s = fmt::format("{}", i);
            MD5 hash;
            hash.add(input, sizeof(input));
            hash.add(s.data(), s.size());
            hash.getHash(buf.data());
            auto cnt = std::transform_reduce(buf.begin(), buf.begin() + n/2 + n%2, 0UL, std::plus{},
                    [&](auto c) { return ((c & 0xF0) == 0) + ((c & 0x0F) == 0); }); // NOLINT

            if (cnt >= n) { return std::optional{i}; }
        }
        return std::nullopt;
    };
    return aoc::result(count(5).value(), count(6).value());
}
