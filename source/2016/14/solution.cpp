#include <aoc.hpp>
#include <util/md5.h>

namespace {
    auto find_reps(auto const sv, i32 n) -> std::optional<char> {
        ENSURE(sv.size() >= n);
        for (auto a = sv.begin(), b = a + n; b <= sv.end(); ++a, ++b) {
            std::string_view s{ a, b };
            ENSURE(s.size() == n);
            if (std::ranges::all_of(s, [&](auto c) { return c == *a; })) {
                return *a;
            }
        }
        return {};
    }

    auto find_64th_key(auto const salt, i32 times) {
        constexpr auto target{64UL};
        constexpr auto horizon{1000};
        aoc::dense::set<u32> keys;

        std::array<uint8_t, 16> sig{};
        std::string hex;

        aoc::dense::map<char, std::vector<u32>> rep3;
        for (auto i = 0U; keys.size() < target; ++i) {
            auto message = salt + std::to_string(i);

            for (auto k = 0; k < times; ++k) {
                hashing::md5::hash(message, sig.data(), hex);
                std::swap(message, hex);
                hex.clear();
            }

            // check for 3 repeated characters, only consider the first such triple (break)
            constexpr auto n = 3;
            if (auto res = find_reps(message, n); res) {
                rep3[res.value()].push_back(i);
            }

            // check for 5 repeated characters
            constexpr auto m = 5;
            std::string_view sv { message };
            for (auto res = find_reps(sv, m); sv.size() >= m && res.has_value(); sv.remove_prefix(m)) {
                if (auto it = rep3.find(*res); !(it == rep3.end() || it->second.empty())) {
                    auto& v = it->second;
                    auto valid = v | std::views::filter([&](auto j){ return i > j && i-j < horizon; });
                    keys.insert(valid.begin(), valid.end());
                    v = std::vector<u32>(valid.begin(), valid.end());
                }
            }
        }
        std::vector<u32> vec;
        std::ranges::copy(keys.values(), std::back_inserter(vec));
        std::ranges::sort(vec);
        return vec[target-1];
    }
} // namespace

template<>
auto advent2016::day14() -> result {
    // constexpr auto salt{"abc"};
    constexpr auto salt{"ihaygndm"};
    auto part1 = find_64th_key(salt, 1);
    auto part2 = find_64th_key(salt, 2017);
    return aoc::result(part1, part2);
}
