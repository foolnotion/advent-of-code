#ifndef ADVENT_OF_CODE_HPP
#define ADVENT_OF_CODE_HPP

#include <charconv>
#include <cmath>
#include <cstdint>

#include <array>
#include <deque>
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

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <robin_hood.h>
#include <scn/scn.h>
#include <ankerl/unordered_dense.h>

#include <Eigen/Core>
#include <Lz/Lz.hpp>

#define XXH_INLINE_ALL
#include <xxh3.h>

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

    auto operator()(int day) const -> void
    {
        assert(day >= 1 && day <= 25);
        days[day-1]();
    }

    static auto day01() -> void;
    static auto day02() -> void;
    static auto day03() -> void;
    static auto day04() -> void;
    static auto day05() -> void;
    static auto day06() -> void;
    static auto day07() -> void;
    static auto day08() -> void;
    static auto day09() -> void;
    static auto day10() -> void;
    static auto day11() -> void;
    static auto day12() -> void;
    static auto day13() -> void;
    static auto day14() -> void;
    static auto day15() -> void;
    static auto day16() -> void;
    static auto day17() -> void;
    static auto day18() -> void;
    static auto day19() -> void;
    static auto day20() -> void;
    static auto day21() -> void;
    static auto day22() -> void;
    static auto day23() -> void;
    static auto day24() -> void;
    static auto day25() -> void;

    private:
    static constexpr std::array<void(*)(), 25> days = { // NOLINT
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
// convenience
namespace dense = ankerl::unordered_dense;

// useful for hashing most things
namespace util {
struct hash {
    template<std::ranges::sized_range R>
        inline auto operator()(R&& r) const -> u64 {
            using T = decltype(std::declval<R>()[0]); // NOLINT
            return XXH_INLINE_XXH3_64bits(r.data(), r.size() * sizeof(T));
        }

    template<typename... Args>
        inline auto operator()(Args... args) const -> u64 {
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
} // namespace util

inline auto contains(std::string_view s, std::string_view q) {
    return s.find(q) != std::string::npos;
}

template<typename T, std::size_t S=2>
using point = std::array<T, S>;

template<char... C>
auto equals(char c) {
    return ((c == C) || ...);
}
} // namespace aoc

template<typename T>
struct std::hash<aoc::point<T>> {
    auto operator()(aoc::point<T> p) const noexcept -> std::size_t {
        return aoc::util::hash{}(p);
    }
};

#endif
