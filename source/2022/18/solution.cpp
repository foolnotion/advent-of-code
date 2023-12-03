#include <aoc.hpp>
#include <bits/ranges_algo.h>

namespace{
    using point = aoc::point<i32, 3>;

    template<std::size_t ... Idx>
    using seq_i = std::index_sequence<Idx...>;

    auto constexpr seq_n = std::make_index_sequence<std::tuple_size_v<point>>{};

    // unpack a tuple into an array (all elements must have the same type)
    constexpr auto unpack(auto const& tup) {
        constexpr auto sz = std::tuple_size_v<std::remove_cvref_t<decltype(tup)>>;
        return [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return std::array { std::get<Idx>(tup)... };
        }(std::make_index_sequence<sz>{});
    };

    auto parse(auto const& input) -> std::tuple<std::vector<point>, point, point> {
        std::vector<point> points;
        points.reserve(input.size());
        point pmin; std::ranges::fill(pmin, std::numeric_limits<i32>::max());
        point pmax; std::ranges::fill(pmax, std::numeric_limits<i32>::min());
        for (auto const& line : input) {
            point p;
            (void)scn::scan(line, "{},{},{}", p[0], p[1], p[2]);
            [&]<auto... Idx>(seq_i<Idx...>) {
                auto f = [&](auto i) {
                    pmin[i] = std::min(p[i]-1, pmin[i]);
                    pmax[i] = std::max(p[i]+1, pmax[i]);
                };
                (f(Idx), ...);
            }(seq_n);
            points.push_back(p);
        }
        return { std::move(points), pmin, pmax };
    }
} // namespace

template<>
auto advent2022::day18() -> result {
    auto [points, pmin, pmax] = parse(aoc::util::readlines("./source/2022/18/input.txt"));
    auto hash = [](point const& p) { return aoc::util::hash{}(p); };
    aoc::dense::set<point, decltype(hash)> lava(points.begin(), points.end());
    aoc::dense::set<point, decltype(hash)> air;

    auto cmp = [](auto const& a, auto const& b, auto&& cmp) {
        return [&]<auto... Idx>(seq_i<Idx...>) {
            return (cmp(a[Idx], b[Idx]) || ...);
        }(seq_n);
    };

    auto sides = [](point const& p) {
        auto get_sides = [&](auto i) {
            auto a = p; a[i] -= 1;
            auto b = p; b[i] += 1;
            return std::tuple{a, b};
        };
        return [&]<auto... Idx>(seq_i<Idx...>) {
            return unpack(std::tuple_cat(get_sides(Idx)...));
        }(seq_n);
    };

    auto is_lava = [&](auto const& p) { return lava.contains(p); };
    auto is_air  = [&](auto const& p) { return air.contains(p); };

    std::queue<point> queue;
    queue.push(pmin);
    while(!queue.empty()) {
        auto p = queue.front();
        queue.pop();

        if (cmp(p, pmin, std::less{}) || cmp(p, pmax, std::greater{})) { continue; }

        if (auto [it, ok] = air.insert(p); !ok) { continue; }

        for (auto&& q : sides(p)) {
            if (!is_lava(q)) { queue.push(q); }
        }
    }

    using std::ranges::count_if;
    auto count_p1 = [&](auto const& p) { return count_if(sides(p), std::not_fn(is_lava)); };
    auto count_p2 = [&](auto const& p) { return count_if(sides(p), is_air); };
    auto part1 = std::transform_reduce(points.begin(), points.end(), 0UL, std::plus{}, count_p1);
    auto part2 = std::transform_reduce(points.begin(), points.end(), 0UL, std::plus{}, count_p2);
    return aoc::result(part1, part2);
}
