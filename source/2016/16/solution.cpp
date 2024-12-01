#include <aoc.hpp>

template<>
auto advent2016::day16() -> result {
    auto checksum = [](auto& s, auto len) {
        EXPECT(len <= std::ssize(s));
        std::string_view v{s.data(), len};
        do {
            for (auto i = 0, j = 0; i < std::ssize(v); i += 2, j += 1) {
                s[j] = '0' + (v[i] == v[i+1L]);
            }
            v.remove_suffix(v.size()/2);
        } while (v.size() % 2 == 0);
        return std::string{v};
    };

    auto grow = [](auto& s) {
        auto sz{std::ssize(s)};
        s.resize(2 * s.size() + 1);
        s[sz] = '0';
        std::transform(s.begin(), s.begin()+sz, s.begin()+sz+1, [&](auto c) { return c == '1' ? '0' : '1'; });
        std::reverse(s.begin()+sz+1, s.end());
    };

    auto fill = [&](auto s, auto len) {
        while (s.size() < len) { grow(s); }
        return checksum(s, len);
    };

    std::string s = "01000100010010111";
    constexpr auto disk_p1{272UL};
    constexpr auto disk_p2{35651584UL};
    return aoc::result(fill(s, disk_p1), fill(s, disk_p2));
}
