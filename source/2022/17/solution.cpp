#include <aoc.hpp>

namespace {
    using shape = Eigen::Array<u8, -1, -1>;

    struct shape_library {
        static auto constexpr count{5L};
        using array = Eigen::Array<u8, -1, -1>;
        static auto r1() { array r(1, 4); r << 1, 1, 1, 1; return r; }
        static auto r2() { array r(3, 3); r << 0, 1, 0, 1, 1, 1, 0, 1, 0; return r; }
        static auto r3() { array r(3, 3); r << 0, 0, 1, 0, 0, 1, 1, 1, 1; return r; }
        static auto r4() { array r(4, 1); r << 1, 1, 1, 1; return r; }
        static auto r5() { array r(2, 2); r << 1, 1, 1, 1; return r; }
    };

    struct state {
        u64 shape_index;
        u64 jet_index;
        u64 rocks;
        u64 highest;
        u64 prevhigh;
    };

    auto iterate(auto& cave, auto const& jets, auto state) {
        auto nrow{cave.rows()};
        auto ncol{cave.cols()};

        static const std::array shapes{ shape_library::r1(), shape_library::r2(), shape_library::r3(), shape_library::r4(), shape_library::r5() };
        const auto& s = shapes[state.shape_index];
        auto w = s.cols();
        auto h = s.rows();
        // Each rock appears so that its left edge is two units away from the left wall
        // and its bottom edge is three units above the highest rock in the room
        auto x = state.highest - h - 3L;
        auto y = 2L;

        auto i{state.jet_index};
        while(true) {
            auto c = jets[i++ % jets.size()];
            if (c == '<' && y > 0 && ((s + cave.block(x, y-1, h, w)) < 2).all()) { --y; }
            if (c == '>' && y+w < ncol && ((s + cave.block(x, y+1, h, w)) < 2).all()) { ++y; }
            if (x + h < nrow && ((s + cave.block(x+1, y, h, w)) < 2).all()) {
                x += 1;
                continue;
            }
            break;
        }
        cave.block(x, y, s.rows(), s.cols()) += s;

        // prepare next iter
        state.prevhigh = state.highest;
        state.highest = std::min(state.highest, x);
        state.jet_index = i % jets.size();
        state.shape_index = (state.shape_index + 1) % shape_library::count;
        state.rocks++;
        return state;
    }

    auto get_heights(auto const& cave, auto highest) {
        std::vector<u64> heights(cave.cols(), highest);
        for (auto i = 0; i < heights.size(); ++i) {
            while (heights[i] < cave.rows() && cave(heights[i], i) == 0) { ++heights[i]; }
        }
        std::ranges::transform(heights, heights.begin(), [&](auto x) { return x - highest; });
        return heights;
    }

    auto tower_height(auto cave, auto const& jets, auto n, bool detect_cycle = false) -> u64 {
        aoc::dense::map<u64, state> last;
        auto const nrow{cave.rows()};
        auto const ncol{cave.cols()};

        auto height{0UL};
        for (state s{0, 0, 0, static_cast<u64>(nrow)}; s.rocks <= n; s = iterate(cave, jets, s)) {
            height = nrow - s.highest;

            if (detect_cycle) {
                auto heights = get_heights(cave, s.highest);
                aoc::util::hash hash;
                auto h = hash(hash(heights), s.jet_index, s.shape_index);

                if (auto [it, ok] = last.insert({h, s}); !ok) {
                    auto s0     = it->second;         // cycle begin state
                    auto len    = s.rocks - s0.rocks; // cycle length
                    auto [q, r] = std::ldiv(n - s0.rocks, len);
                    auto res    = nrow - s0.prevhigh + (s0.prevhigh - s.prevhigh) * q;
                    // continue to iterate this state for the remainder
                    auto x = s.prevhigh;
                    for (auto i = 0; i < r; ++i) { s = iterate(cave, jets, s); }
                    return res + x - s.highest;
                }
            }
        }
        return height;
    }
} // namespace

template<>
auto advent2022::day17() -> result {
    std::string jets = aoc::util::readlines("./source/2022/17/input.txt").front();

    constexpr auto height{10000};
    constexpr auto width{7};

    std::vector<shape> shapes;
    using point = aoc::point<u32, 2>;
    Eigen::Array<u8, -1, -1, Eigen::RowMajor> chamber(height, width);
    chamber.setConstant(0);
    auto constexpr rocks_p1{2022UL};
    auto constexpr rocks_p2{1'000'000'000'000UL};
    auto part1 = tower_height(chamber, jets, rocks_p1);
    auto part2 = tower_height(chamber, jets, rocks_p2, /*detect_cycle=*/true);
    return aoc::result(part1, part2);
}
