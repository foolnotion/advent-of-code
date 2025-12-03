#include <algorithm>
#include <aoc.hpp>
#include <cpp-sort/sorters/merge_sorter.h>

namespace {
    constexpr auto digits_p1 = 2;
    constexpr auto digits_p2 = 12;

    auto joltage(std::string_view bank, size_t n_digits) {
        std::vector<int> index(bank.size());
        std::iota(std::begin(index), std::end(index), 0);

        cppsort::merge_sorter{}(index, [&](auto i, auto j) {
            return bank[i] > bank[j];
        });

        auto const bank_size = std::ssize(bank);

        auto valid_index = [&](auto curr_index, auto last_index, auto n_filled) -> bool {
            auto still_needed = n_digits - n_filled;
            auto available = bank_size - curr_index;
            return curr_index > last_index && available >= still_needed;
        };

        auto value{0UL};
        auto last_index = -1;
        for (auto n = 0UL; n < n_digits; ++n) {
            auto current_index = std::ranges::find_if(index, [&](auto i) -> bool {
                return valid_index(i, last_index, n);
            });
            ASSERT(current_index != index.end());
            value = 10 * value + (bank[*current_index]-'0');
            last_index = *current_index;
        }
        return value;
    }

    auto joltage_p1(std::string_view line) -> u64 { return joltage(line, digits_p1); }
    auto joltage_p2(std::string_view line) -> u64 { return joltage(line, digits_p2); }
} // namespace

template<>
auto advent2025::day03() -> result
{
    auto lines = aoc::util::readlines("source/2025/03/input.txt");
    auto const p1 = std::transform_reduce(lines.begin(), lines.end(), 0UL, std::plus{}, joltage_p1);
    auto const p2 = std::transform_reduce(lines.begin(), lines.end(), 0UL, std::plus{}, joltage_p2);
    return aoc::result(p1, p2);
}
