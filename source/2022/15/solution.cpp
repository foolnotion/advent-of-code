#include <aoc.hpp>
#include <util/dynamic_bitset.hpp>

namespace {
    using point = Eigen::Matrix<i64, 2, 1>;
    using interval = aoc::math::interval<i64>;

    struct sensor {
        point b; // beacon
        point c; // center
        point::Scalar r{}; // radius

        [[nodiscard]] auto intersect(auto x) const {
            auto a = std::abs(c[0] - x);
            if (a > r) { return interval::empty(); }
            return interval{c[1] - r + a, c[1] + r - a};
        }
    };
} // namespace

template<>
auto advent2022::day15() -> result {
    auto input = aoc::util::readlines("./source/2022/15/input.txt");

    using point = point;
    auto xmin{std::numeric_limits<i64>::max()};
    auto xmax{std::numeric_limits<i64>::min()};
    auto ymin{std::numeric_limits<i64>::max()};
    auto ymax{std::numeric_limits<i64>::min()};
    aoc::dense::map<u64, point> bmap;
    auto sensors = lz::map(input, [&](auto const& line) {
        sensor r;
        (void)scn::scan(line, "Sensor at x={}, y={}: closest beacon is at x={}, y={}", r.c[1], r.c[0], r.b[1], r.b[0]);
        r.r = (r.c - r.b).matrix().template lpNorm<1>();
        xmin = std::min({xmin, r.c[0]-r.r});
        xmax = std::max({xmax, r.c[0]+r.r});
        ymin = std::min({ymin, r.c[1]-r.r});
        ymax = std::max({ymax, r.c[1]+r.r});
        return r;
    }).toVector();

    aoc::util::hash h;
    for (auto& r : sensors) {
        point p{xmin, ymin};
        r.c -= p;
        r.b -= p;
    }

    // part 1
    auto constexpr row_p1{2'000'000L};
    auto intervals = lz::map(sensors, [&](auto const& s) { return s.intersect(row_p1 - xmin); }).toVector();
    sul::dynamic_bitset<u64> bs(/*nbits=*/ymax-ymin+1, /*init_val=*/false);
    for (auto [a, b] : intervals) {
        if (a > b) { continue; }
        bs.set(a, b-a+1, /*value=*/true);
    }
    for (auto const& s : sensors) {
        if(s.b[0] == row_p1 - xmin) { bs.reset(s.b[1]); }
    }
    auto part1 = bs.count();

    // part 2
    auto constexpr lim{4'000'000L};
    auto part2{0L};

    auto sz{0L};
    for (auto i = lim-xmin; i >= 0; --i) {
        auto z{0L};
        intervals.clear();
        for (auto const& s : sensors) {
            auto iv = s.intersect(i);
            if (!iv.isempty()) {
                intervals.push_back(iv);
                z += iv.width();
            }
        }
        // if the total sum of the intervals didn't change, then we can skip this row
        if (z == sz) { continue; }
        sz = z;
        if (intervals.empty()) { continue; }
        std::ranges::sort(intervals, [](auto const& lhs, auto const& rhs) {
            return lhs.a < rhs.a;
        });
        std::optional<i64> y;
        auto u = intervals.front();
        for (auto v : std::span{intervals.begin()+1, intervals.end()}) {
            if (u.b == u.a || u.intersects(v)) {
                u = u | v;
            } else {
                y = { u.b + 1 };
                break;
            }
        }
        if (y.has_value()) {
            auto x = *y + ymin;
            part2 = lim * x + i + xmin;
            break;
        }
    }
    return aoc::result(part1, part2);
}
