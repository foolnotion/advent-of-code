#include <aoc.hpp>

template<>
auto advent2024::day25() -> result {
    constexpr auto height{7};
    constexpr auto npin{5};

    using pins = aoc::point<i32, npin>;
    std::vector<pins> keys;
    std::vector<pins> locks;

    auto const lines = aoc::util::readlines("./source/2024/25/input.txt");
    for (auto i = 0; i < lines.size(); i += height+1) {
        pins p;
        std::ranges::fill(p, 0);
        for (auto j = i; j < i + height; ++j) {
            auto const& line = lines[j];
            for (auto k = 0; k < npin; ++k) {
                p[k] += line[k] == '#';
            }
        }
        if (lines[i][0] == '.') { locks.push_back(p); }
        else                    { keys.push_back(p); }
    }

    auto fit = [npin](pins const& key, pins const& lock) {
        return std::ranges::none_of(std::views::iota(0, npin), [&](auto i) { return key[i]+lock[i] > height; });
    };
    auto count_unlocks = [&](auto const& key) {
        return std::transform_reduce(locks.begin(), locks.end(), 0UL, std::plus{}, [&](auto const& lock) { return fit(key, lock); });
    };
    auto const p1  = std::transform_reduce(keys.begin(), keys.end(), 0UL, std::plus{}, count_unlocks);
    return aoc::result(p1, "");
}