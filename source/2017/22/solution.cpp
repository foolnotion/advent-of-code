#include <aoc.hpp>

namespace {
    using point = std::complex<i32>;

    struct turn {
        static constexpr auto left  = point{0, +1};
        static constexpr auto right = point{0, -1};
        static constexpr auto back  = point{-1, 0};
        static constexpr auto stay  = point{+1, 0};
    };
} // namespace

template<>
auto advent2017::day22() -> result {
    auto input = aoc::util::readlines("./source/2017/22/input.txt");
    auto nrows = std::ssize(input);
    auto ncols = std::ssize(input.front());

    // a more efficient hash for 2d points
    auto hash = [](auto const p) {
        return (static_cast<u64>(p.real()) << 32U) | static_cast<u32>(p.imag());
    };

    aoc::dense::map<point, u8, decltype(hash)> start_grid;
    for (auto i = 0; i < nrows; ++i) {
        auto const& s = input[i];
        for (auto j = 0; j < ncols; ++j) {
            if (s[j] == '#') {
                start_grid.insert({point{i, j}, 2});
            }
        }
    }

    // part 1
    i64 p1{};
    {
        point pos{nrows/2, ncols/2};
        point dir{-1, 0}; // facing up
        constexpr auto bursts{10'000};
        auto infected{0L};
        auto grid = start_grid;

        for (auto i = 0; i < bursts; ++i) {
            auto [it, _] = grid.insert({pos, 0});
            dir *= it->second == 2 ? turn::right : turn::left;
            pos += dir;
            auto c = it->second == 2 ? 0 : 2;
            infected += c == 2;
            it->second = c;
        }
        p1 = infected;
    }

    // part 2
    constexpr std::array dirs{ turn::left, turn::stay, turn::right, turn::back };
    i64 p2{};
    {
        point pos{nrows/2, ncols/2};
        point dir{-1, 0}; // facing up
        constexpr auto bursts{10'000'000};
        auto infected{0L};
        auto grid = start_grid;

        for (auto i = 0; i < bursts; ++i) {
            auto [it, _] = grid.insert({pos, 0});
            auto c = it->second;
            dir *= dirs[c];
            pos += dir;
            ++c %= 4;
            infected += c == 2;
            it->second = c;
        }
        p2 = infected;
    }

    return aoc::result(p1, p2);
}