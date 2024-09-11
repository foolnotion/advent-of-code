#include <aoc.hpp>
#include <util/md5.h>

template<>
auto advent2016::day05() -> result {
    constexpr auto input{"ugkcyxxp"};
    constexpr auto len{8}; // password length
    std::array<u8, 16> buf{}; // NOLINT
    std::string s;
    std::string part1;
    std::string part2(len, '*');
    constexpr auto dec2hex{"0123456789abcdef"};
    for (auto i = 0, k = 0; k < len; ++i) {
        hashing::md5::hash_bs(input + std::to_string(i), buf.data());

        constexpr auto mask{15U};
        if (buf[0] == 0 && buf[1] == 0 && (buf[2] >> 4U) == 0) {
            auto pos = buf[2] & mask;
            part1.push_back(dec2hex[pos]); // NOLINT
            if (pos < len && part2[pos] == '*') {
                part2[pos] = dec2hex[mask & (buf[3] >> 4U)]; // NOLINT
                ++k;
            }
        }
    }

    return aoc::result(part1.substr(0, len), part2);
}
