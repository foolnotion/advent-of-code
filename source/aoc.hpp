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
        (this->* days[day-1])();
    }

    auto day01() const -> void;
    auto day02() const -> void;
    auto day03() const -> void;
    auto day04() const -> void;
    auto day05() const -> void;
    auto day06() const -> void;
    auto day07() const -> void;
    auto day08() const -> void;
    auto day09() const -> void;
    auto day10() const -> void;
    auto day11() const -> void;
    auto day12() const -> void;
    auto day13() const -> void;
    auto day14() const -> void;
    auto day15() const -> void;
    auto day16() const -> void;
    auto day17() const -> void;
    auto day18() const -> void;
    auto day19() const -> void;
    auto day20() const -> void;
    auto day21() const -> void;
    auto day22() const -> void;
    auto day23() const -> void;
    auto day24() const -> void;
    auto day25() const -> void;

    private:
    std::array<void(advent::*)() const, 11> days = { // NOLINT
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
    };
};

namespace aoc {
namespace util {
    // useful for hashing most things
    struct hash {
        template<std::ranges::sized_range R>
        inline auto operator()(R&& r) const -> u64 {
            using T = decltype(std::declval<R>()[0]);
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
} // namespace util

template<typename T>
using point = std::array<T, 2>;

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
