#include <aoc.hpp>

namespace {
    auto pow5(auto p) {
        auto constexpr n{5L};
        auto r{1L};
        for (auto i = 0; i < p; ++i) { r *= n; }
        return r;
    }

    auto snafu2dec(auto const& s) -> i64 {
        static aoc::dense::map<char, i64> val{
            { '=', -2 },
            { '-', -1 },
            { '0',  0 },
            { '1',  1 },
            { '2',  2 }
        };
        auto n{0LL};
        auto const sz(std::ssize(s));
        for (auto i = sz-1; i >= 0; --i) {
            n += val[s[i]] * pow5(sz-i-1);
        }
        return n;
    }

    auto dec2snafu(auto n) -> std::string {
        static aoc::dense::map<char, char> neg{
            {'=', '2'},
            {'-', '1'},
            {'0', '0'},
            {'1', '-'},
            {'2', '='}
        };
        static aoc::dense::map<i64, char> val{
            {0, '0'}, {1, '1'}, {2, '2'}, {3, '='}, {4, '-'}
        };
        if (n < 0) {
            return lz::map(dec2snafu(std::abs(n)), [&](auto c) { return neg[c]; }).template to<std::string>();
        }
        std::string s;
        auto constexpr b{5};

        while (n > 0) {
            auto [q, r] = std::ldiv(n, b);
            n = n / b + (r > 2);
            s += val[r];
        }
        std::ranges::reverse(s);
        return s;
    }
} // namespace

template<>
auto advent2022::day25() -> result {
    auto const& input = aoc::util::readlines("./source/2022/25/input.txt");
    std::add_pointer_t<i64(std::string const&)> ptr{ &snafu2dec };
    auto part1 = std::transform_reduce(input.begin(), input.end(), i64{0}, std::plus{}, ptr);
    return aoc::result(dec2snafu(part1), "");
}
