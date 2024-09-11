#include <aoc.hpp>

template<>
auto advent2020::day10() -> result {
    auto input = aoc::util::readlines("./source/2020/10/input.txt");
    auto read_value = aoc::util::read<i64>;
    auto v = lz::map(input, read_value).toVector();
    std::ranges::sort(v);
    i64 a{0};
    i64 b{1};
    for (size_t i = 0; i < v.size() - 1; ++i) {
        auto diff = v[i+1] - v[i];
        a += diff == 1;
        b += diff == 3;
    }
    a += v.front() == 1;
    auto p1 = a * b;

    std::vector<i64> counts;
    std::vector<i64> u {0};
    std::copy(v.begin(), v.end(), std::back_inserter(u));
    u.push_back(u.back()+3);

    auto get_intervals = [](std::span<i64> u) {
        std::vector<std::span<i64>> intervals;
        for(u64 i = 0; i < u.size(); ++i) {
            auto j = i+1;
            while(j < u.size() && u[j] - u[i] <= 3) {
                ++j;
            }

            if (intervals.empty()) {
                intervals.emplace_back(u.data()+i, j-i);
                continue;
            }
            auto iv = intervals.back();
            auto a = iv.data() - u.data();
            auto b = a + iv.size();
            intervals.push_back(b > i ? std::span{iv.data(), j} : std::span{&u[i], j-i});
        }
        return intervals;
    };

    auto part2 = [](std::span<i64> u) -> u64 {
        std::vector<u64> counts(u.size(), 0);
        counts.back() = 1;

        for(auto i = std::ssize(u)-2L; i >= 0L; --i) {
            for (auto j = i + 1; j < std::ssize(u); ++j) {
                if (u[j] - u[i] > 3) { break; }
                counts[i] += counts[j];
            }
        }
        return counts.front();
    };
    auto p2 = part2(u);
    return aoc::result(p1, p2);
}
