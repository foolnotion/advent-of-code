#ifndef ADVENT_OF_CODE_HPP
#define ADVENT_OF_CODE_HPP

#include <charconv>
#include <cmath>
#include <cstdint>

#include <array>
#include <deque>
#include <iterator>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <tuple>

#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>

#include <type_traits>

#include <fstream>
#include <ostream>

#include <cpp-sort/sorters.h>
#include <cpp-sort/fixed_sorters.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <robin_hood.h>
#include <scn/scn.h>
#include <ankerl/unordered_dense.h>

#include <Eigen/Core>

#undef LZ_STANDALONE
#include <Lz/Lz.hpp>

#include <util/constexpr-xxh3.h>
#include <util/xxhash.hpp>

// NOLINTBEGIN(*)
#define EXPECT(cond) \
    if(!(cond)) \
    { \
        fmt::print("Precondition {} failed at {}: {}\n", fmt::format(fmt::fg(fmt::terminal_color::green), "{}", #cond), __FILE__, __LINE__); \
        std::terminate(); \
    }

#define ENSURE(cond) \
    if(!(cond)) \
    { \
        fmt::print("Postcondition {} failed at {}: {}\n", fmt::format(fmt::fg(fmt::terminal_color::green), "{}", #cond), __FILE__, __LINE__); \
        std::terminate(); \
    }
// NOLINTEND(*)

// convenience aliases
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

template<int YEAR>
struct advent {
    static constexpr int year = YEAR;
    using result = std::tuple<std::string, std::string>;

    auto operator()(int day) const -> result {
        assert(day >= 1 && day <= 25);
        return days[day-1]();
    }

    static auto day01() -> result;
    static auto day02() -> result;
    static auto day03() -> result;
    static auto day04() -> result;
    static auto day05() -> result;
    static auto day06() -> result;
    static auto day07() -> result;
    static auto day08() -> result;
    static auto day09() -> result;
    static auto day10() -> result;
    static auto day11() -> result;
    static auto day12() -> result;
    static auto day13() -> result;
    static auto day14() -> result;
    static auto day15() -> result;
    static auto day16() -> result;
    static auto day17() -> result;
    static auto day18() -> result;
    static auto day19() -> result;
    static auto day20() -> result;
    static auto day21() -> result;
    static auto day22() -> result;
    static auto day23() -> result;
    static auto day24() -> result;
    static auto day25() -> result;

    private:
    static constexpr std::array<result(*)(), 25> days = { // NOLINT
        &advent<year>::day01,
        &advent<year>::day02,
        &advent<year>::day03,
        &advent<year>::day04,
        &advent<year>::day05,
        &advent<year>::day06,
        &advent<year>::day07,
        &advent<year>::day08,
        &advent<year>::day09,
        &advent<year>::day10,
        &advent<year>::day11,
        &advent<year>::day12,
        &advent<year>::day13,
        &advent<year>::day14,
        &advent<year>::day15,
        &advent<year>::day16,
        &advent<year>::day17,
        &advent<year>::day18,
        &advent<year>::day19,
        &advent<year>::day20,
        &advent<year>::day21,
        &advent<year>::day22,
        &advent<year>::day23,
        &advent<year>::day24,
        &advent<year>::day25,
    };
};

using advent2015 = advent<2015>; // NOLINT
using advent2016 = advent<2016>; // NOLINT
using advent2017 = advent<2017>; // NOLINT
using advent2018 = advent<2018>; // NOLINT
using advent2019 = advent<2019>; // NOLINT
using advent2020 = advent<2020>; // NOLINT
using advent2021 = advent<2021>; // NOLINT
using advent2022 = advent<2022>; // NOLINT

namespace aoc {
template<typename T1, typename T2>
auto result(T1 t1, T2 t2) -> std::tuple<std::string, std::string> {
    return std::tuple{fmt::format("{}", t1), fmt::format("{}", t2)};
}

template<typename T, std::size_t S=2>
using point = std::array<T, S>;

// convenience
namespace dense = ankerl::unordered_dense; // NOLINT

// useful for hashing most things
namespace util {
struct hash {
    using is_transparent = void;

    static constexpr auto hash_bits{64UL};
    consteval inline auto operator()(constexpr_xxh3::BytesType auto const& array) const -> u64 {
        return constexpr_xxh3::XXH3_64bits_const(array);
    }

    constexpr inline auto operator()(std::basic_string<char> const& s) const -> u64 {
        return xxh::xxhash3<hash_bits>(s.begin(), s.end());
    }

    constexpr inline auto operator()(std::ranges::range auto&& r) const -> u64 {
        return xxh::xxhash3<hash_bits>(r.begin(), r.end());
    }

    constexpr inline auto operator()(auto... args) const -> u64 {
        std::array arr = { args... };
        return (*this)(arr);
    }
};

inline auto readlines(std::string const& path) {
    std::ifstream f(path);
    if (f.is_open()) {
        std::vector<std::string> lines;
        for (std::string s; std::getline(f, s); ) {
            lines.push_back(s);
        }
        return lines;
    }
    throw std::runtime_error(fmt::format("could not open path {}\n", path));
}

inline auto replace_all(std::string& inout, std::string_view what, std::string_view with) -> std::size_t
{
    std::size_t count{};
    for (std::string::size_type pos{};
         std::string::npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}
 
inline auto remove_all(std::string& inout, std::string_view what) -> std::size_t {
    return replace_all(inout, what, "");
}

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

inline auto sgn(arithmetic auto a) -> i32 {
    auto z = a-a; return (z < a) - (a < z); // NOLINT
}
} // namespace util

namespace eigen {
    // utilities to enhance working with eigen
    auto block(auto const& mat, aoc::point<i64> x, aoc::point<i64> y) { // NOLINT
        auto [xmin, ymin] = x;
        auto [xmax, ymax] = y;
        xmin = std::clamp(xmin, i64{0}, mat.rows()-1);
        xmax = std::clamp(xmax, i64{0}, mat.rows()-1);
        ymin = std::clamp(ymin, i64{0}, mat.cols()-1);
        ymax = std::clamp(ymax, i64{0}, mat.cols()-1);
        return mat.block(xmin, ymin, xmax-xmin+1, ymax-ymin+1);
    }
} // namespace eigen

namespace constants {
    constexpr std::array alphabet{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
} // namespace constants

namespace math {
template<typename T>
requires std::is_integral_v<T>
inline auto mul_mod(T a, T b, T m) -> T
{
    if (m == 0) { return a * b; }
    T r{0};
    while (a > 0) {
        if (a & 1) {
            if ((r += b) > m) {
                r %= m;
            }
        }
        a >>= 1;
        if ((b <<= 1) > m) {
            b %= m;
        }
    }
    return r;
}

// modular exponentiation function
template<typename T>
requires std::is_integral_v<T>
inline auto pow_mod(T a, T n, T m) -> T // NOLINT
{
    T r{1};
    while (n > 0) {
        if (n & 1) {
            r = mul_mod(r, a, m);
        }
        a = mul_mod(a, a, m);
        n >>= 1;
    }
    return r;
}
} // namespace math

inline auto contains(std::string_view s, std::string_view q) {
    return s.find(q) != std::string::npos; // NOLINT
}

template<char... C>
auto equals(char c) {
    return ((c == C) || ...);
}
} // namespace aoc

template<typename T>
struct std::hash<aoc::point<T>> {
    constexpr auto operator()(aoc::point<T> p) const noexcept -> std::size_t {
        return aoc::util::hash{}(p);
    }
};

#endif
