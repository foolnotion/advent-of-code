#include <aoc.hpp>

template<>
auto advent2018::day06() -> result {
    auto lines = aoc::util::readlines("source/2018/06/input.txt");
    using point = Eigen::Vector<i32, 2>;

    std::vector<point> points;
    aoc::dense::map<u64, u64> area;

    for (auto const& line : lines) {
        auto [x, y] = scn::scan<i32, i32>(line, "{}, {}")->values();
        points.emplace_back(x, y);
        auto const h = aoc::util::hash{}(x, y);
        area.insert({h, 0});
    }

    auto region_size{0};
    aoc::dense::set<u64> infinite_area;
    constexpr auto max_distance{10'000};

    auto [amin, amax] = std::ranges::minmax_element(points, std::less{}, [](auto p) { return p(0); });
    auto [bmin, bmax] = std::ranges::minmax_element(points, std::less{}, [](auto p) { return p(1); });
    auto [xmin, xmax] = std::make_tuple((*amin)(0), (*amax)(0));
    auto [ymin, ymax] = std::make_tuple((*bmin)(1), (*bmax)(1));

    for (auto i = xmin-1; i < xmax+2; ++i) {
        for (auto j = ymin-1; j < ymax+2; ++j) {
            point u(i, j);
            point closest;
            auto min_distance(std::numeric_limits<i32>::max());
            auto total_distance{0};
            auto equally_far{false};

            for (auto const& v : points) {
                auto d = (u-v).template lpNorm<1>();
                if (d == min_distance) { equally_far = true; }
                if (d < min_distance) {
                    min_distance = d;
                    equally_far = false;
                    closest = v;
                }
                total_distance += d;
            }

            if (total_distance < max_distance) { ++region_size; }
            auto const h = aoc::util::hash{}(closest);

            if (aoc::util::hash{}(u) != h && !equally_far) {
                area[h]++;
                if (i == xmin-1 || i == xmax+1 || j == ymin-1 || j == ymax+1) { infinite_area.insert(h); }
            }
        }
    }
    for (auto v : infinite_area) { area.erase(v); }

    auto const p1 = std::ranges::max(area.values(), std::less{}, [](auto t){ return t.second; }).second + 1;
    auto const p2 = region_size;
    return aoc::result(p1, p2);
}