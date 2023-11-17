#include <aoc.hpp>
#include <util/md5.h>

namespace detail {
    auto has_repeats(std::string_view sv, auto len) -> std::optional<char> {
        for (auto i = 0; i < std::ssize(sv)-len+1; ++i) {
            std::string_view v{&sv[i], len};
            auto x = sv[i];
            if (std::ranges::all_of(v, [&](char c) { return x == c; })) {
                return {x};
            }
        }
        return {};
    }

    auto has_repeats(std::string_view sv, char x, auto len) {
        for (auto i = 0; i < std::ssize(sv)-len+1; ++i) {
            if (sv[i] != x) { continue; }
            std::string_view v{&sv[i], len};
            if (std::ranges::all_of(v, [&](char c) { return x == c; })) {
                return true;
            }
        }
        return false;
    }

    auto find_64th_key(auto const salt, i32 times) {
        constexpr auto target{64};
        constexpr auto horizon{1000};
        std::vector<std::tuple<char, i32>> candidates;
        aoc::dense::set<i32> keys;

        MD5 md5;
        std::string s;
        s.reserve(2UL * MD5::HashBytes);
        for (auto i = 0; keys.size() < target; ++i) {
            s = salt;
            fmt::format_to(std::back_inserter(s), "{}", i);

            for (auto k = 0; k < times; ++k) {
                s = md5(s.data(), s.size());
            }

            candidates.erase(std::remove_if(candidates.begin(), candidates.end(), [&](auto const& t) {
                auto j = std::get<1>(t);
                return j + horizon < i || keys.contains(j);
            }), candidates.end());

            for (auto const [c, j] : candidates) {
                if (detail::has_repeats(s, c, 5UL)) { // NOLINT
                    keys.insert(j);
                }
            }

            if (std::ssize(keys) < target) {
                if (auto res = detail::has_repeats(s, 3UL); res) {
                    candidates.emplace_back(*res, i);
                }
            }
        }

        // std::ranges::sort(keys);
        return keys.values()[target-1];
    }
} // namespace detail

template<>
auto advent2016::day14() -> result {
    constexpr auto salt{"ihaygndm"};
    auto part1 = detail::find_64th_key(salt, 1);
    auto part2 = detail::find_64th_key(salt, 2017); // NOLINT
    return aoc::result(part1, part2);
}
