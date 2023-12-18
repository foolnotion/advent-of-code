#include <aoc.hpp>

template<>
auto advent2019::day04() -> result {
    constexpr auto range = std::pair{108457, 562041};
    constexpr auto base  = 10;
    constexpr auto size  = 6;

    auto get_digits = [](auto n) {
        std::array<u8, size> digits;
        for (auto i = 1, x = n % base; n > 0; n /= base, x = n % base, i += 1) {
            digits[size-i] = x;
        }
        return digits;
    };

    auto has_double_digit = [](auto const& x, bool p2 = false) {
        for (auto i = 0; i < x.size()-1; ++i) {
            auto j = i;
            while(j < x.size() && x[j] == x[i]) { ++j; }
            if (p2 && j-i == 2) { return 1L; }
            if (!p2 && j-i >= 2) { return 1L; }
            i = j-1;
        }
        return 0L;
    };

    auto a = get_digits(range.first);
    auto b = get_digits(range.second);

    auto compare = [&](auto x, auto y) {
        for (auto i = 0; i < size; ++i) {
            if (x[i] == y[i]) { continue; }
            return x[i] < y[i];
        }
        return true;
    };

    auto f = [&](auto x, auto i, auto p2, auto&& f) -> i64 {
        auto c{0};
        auto m = i == 0 ? a[i] : x[i-1];
        for (auto k = m; k < base; ++k) {
            x[i] = k;
            if (!compare(x, b)) { break; }
            c += i == size-1 ? has_double_digit(x, p2) : f(x, i+1, p2, f);
        }
        return c;
    };

    auto const p1 = f(a, 0, false, f);
    auto const p2 = f(a, 0, true, f);
    return aoc::result(p1, p2);
}