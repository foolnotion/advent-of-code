#include <aoc.hpp>

template<>
auto advent2023::day01() -> result {
    aoc::dense::map<std::string_view, i32> values {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}
    };
    auto input = aoc::util::readlines("source/2023/01/input.txt");

    auto calibration_value_p1 = [](auto const& s) -> i64 {
        auto a = *std::ranges::find_if(s, [](auto c) { return std::isdigit(c); });
        auto b = *std::ranges::find_if(std::views::reverse(s), [](auto c) { return std::isdigit(c); });
        return 10 * (a-'0') + (b-'0');
    };

    auto calibration_value_p2 = [&](auto const& s) {
        std::pair<u64, u64> min_digit = { std::string::npos, 0 };
        std::pair<u64, u64> max_digit = { 0, 0 };
        for (auto [i, c] : lz::enumerate(s)) {
            if (std::isdigit(c)) {
                if (min_digit.first > i) { min_digit = { i, c-'0' }; }
                if (max_digit.first <= i) { max_digit = { i, c-'0' }; }
            }
        }
        for (auto&& [k, v] : values) {
            for (auto p = s.find(k); p != std::string::npos; p = s.find(k, p+1)) {
                if (min_digit.first > p) { min_digit = {p, v}; }
                if (max_digit.first <= p) { max_digit = {p, v}; }
            }
        }
        return 10 * min_digit.second + max_digit.second;
    };

    auto const p1 = std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, calibration_value_p1);
    auto const p2 = std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, calibration_value_p2);
    return aoc::result(p1, p2);
}