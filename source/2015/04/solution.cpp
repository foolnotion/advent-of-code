#include <aoc.hpp>
#include <openssl/md5.h>

template <>
auto advent<2015>::day04() const -> void
{
    constexpr auto input { "iwrupvqb" };
    std::array<unsigned char, 16> buf;

    auto count = [&](int n) -> std::optional<i64> {
        for (auto i = 0; ; ++i) {
            auto const s = fmt::format("{}{}", input, i);
            auto const* val = MD5(reinterpret_cast<unsigned char const*>(s.data()), s.size(), buf.data());

            int c = 0;
            constexpr u64 mask{0xFF};
            for (int j = 0; j < n/2 + n%2; ++j) {
                auto v = buf[j];
                c += (static_cast<u64>(v >> 4UL) & mask) == 0;
                c += (v & mask) == 0;
            }

            if (c >= n) {
                return std::optional{i};
            }
        }
        return std::nullopt;
    };

    fmt::print("part 1: {}\n", count(5).value()); // NOLINT
    fmt::print("part 2: {}\n", count(6).value()); // NOLINT
}
