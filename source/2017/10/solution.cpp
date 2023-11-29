#include <aoc.hpp>
#include <functional>

namespace {
    struct converter {
        static constexpr std::array suffix{17, 31, 73, 47, 23};
        auto operator()(std::string const& input) {
            std::vector<u8> bytes;
            bytes.reserve(input.size());
            std::ranges::copy(input, std::back_inserter(bytes));
            std::ranges::copy(suffix, std::back_inserter(bytes));
            return bytes;
        }
    };
} // namespace

template<>
auto advent2017::day10() -> result {
    std::vector<i32> lengths{94, 84, 0, 79, 2, 27, 81, 1, 123, 93, 218, 23, 103, 255, 254, 243}; // NOLINT
    constexpr auto size{256};

    // part one
    std::array<u8, size> list{};
    std::iota(list.begin(), list.end(), 0);
    auto hash = [](auto& salt, auto const& input, i32 p = 0, i32 s = 0) {
        for (auto n : input) {
            auto i = p;
            auto j = (i + n-1) % size;
            for (auto k = 0; k < n/2; ++k) {
                std::swap(salt[i], salt[j]);
                i = (i+1) % size;
                j = (j-1) % size;
                if (j < 0) { j += size; }
            }
            p = (p + n + s++) % size;
        }
        return std::tuple{p, s};
    };
    hash(list, lengths);
    auto const p1 = list[0] * list[1];
    auto const p2 = aoc::util::knot_hash{}("94,84,0,79,2,27,81,1,123,93,218,23,103,255,254,243");
    return aoc::result(p1, p2);
}
