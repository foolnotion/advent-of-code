#include <aoc.hpp>
#include <util/md5.h>

template <>
auto advent2015::day04() const -> void
{
    constexpr auto input { "iwrupvqb" };
    std::array<u8, 16> buf{};
    MD5 md5;

    auto count = [&](int n) -> std::optional<i64> {
        for (auto i = 0;; ++i) {
            auto const s = fmt::format("{}{}", input, i);
            md5.reset();
            md5.add(s.data(), s.size());
            md5.getHash(buf.data());
            auto cnt = std::transform_reduce(buf.begin(), buf.begin() + n/2 + n%2, 0UL, std::plus{},
                    [&](auto c) { return ((c & 0xF0) == 0) + ((c & 0x0F) == 0); }); // NOLINT

            if (cnt >= n) { return std::optional{i}; }
        }
        return std::nullopt;
    };

    fmt::print("part 1: {}\n", count(5).value()); // NOLINT
    fmt::print("part 2: {}\n", count(6).value()); // NOLINT
}
