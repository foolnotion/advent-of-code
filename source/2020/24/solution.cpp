#include <aoc.hpp>

namespace detail {
    using point = aoc::point<i32, 3>;

    inline auto add(point a, point b) {
        return [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return point{ std::plus{}(a[Idx], b[Idx])... };
        }(std::make_index_sequence<std::tuple_size_v<point>>{});
    };

    constexpr std::array moves {
        point{ +1,  0, -1 }, // E
        point{  0, +1, -1 }, // SE
        point{ -1, +1,  0 }, // SW
        point{ -1,  0, +1 }, // W
        point{  0, -1, +1 }, // NW
        point{ +1, -1,  0 }  // NE
    };

    constexpr auto generate_neighbours(point const p) {
        return [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return std::array{ add(moves[Idx], p)... };
        }(std::make_index_sequence<moves.size()>{});
    };

    auto str2dir(std::string_view sv) {
        if (sv == "e")  { return 0; }
        if (sv == "se") { return 1; }
        if (sv == "sw") { return 2; }
        if (sv == "w")  { return 3; }
        if (sv == "nw") { return 4; }
        if (sv == "ne") { return 5; } // NOLINT
        throw std::runtime_error(fmt::format("unknown direction: {}\n", sv));
    };

    struct tile {
        point coord;
        u32 count;
    };

    auto parse(std::vector<std::string> const& input) {
        const aoc::dense::set<std::string_view> valid_directions {
            "e", "se", "sw", "w", "nw", "ne"
        };

        aoc::dense::map<u64, tile> flipped;
        for (auto const& line : input) {
            auto i = 0;
            auto j = 0;
            std::vector<int> directions;
            while (j < line.size()) {
                if (j < line.size() - 1) { ++j; }
                std::string_view sv{&line[i], j - i + 1UL};
                if (!valid_directions.contains(sv)) {
                    --j;
                    sv.remove_suffix(1);
                }
                i = j + 1;
                j = i;
                directions.push_back(str2dir(sv));
            }
            point p{0,0,0};
            for (auto dir : directions) { p = add(p, moves[dir]); }
            auto [it, _] = flipped.insert({aoc::util::hash{}(p), tile{p, 0}});
            it->second.count++;
        }
        return flipped;
    }
} // namespace detail

template<>
auto advent2020::day24() -> void {
    auto input = aoc::util::readlines("./source/2020/24/input.txt");
    auto flipped = detail::parse(input);

    using std::ranges::transform;
    using std::ranges::count_if;
    using detail::point;
    using detail::tile;
    using detail::add;

    auto black = count_if(flipped, [](auto it) { return it.second.count % 2; });
    fmt::print("part 1: {}\n", black);

    std::vector<std::tuple<u64, point>> neighbours;
    std::vector<u64> tiles;
    aoc::util::hash hash;

    constexpr int days{100};
    for (int i = 0; i < days; ++i) {
        neighbours.clear();
        tiles.clear();
        for (auto [h, tile] : flipped) {
            bool b = tile.count % 2;
            auto count{0U};
            for (auto&& p : detail::generate_neighbours(tile.coord)) {
                auto h = hash(p);
                auto it = flipped.find(h);
                if (it != flipped.end()) {
                    count += it->second.count % 2;
                } else {
                    neighbours.emplace_back(h, p);
                }
            }
            if ((b && (count == 0 || count > 2)) || (!b && count == 2) ) {
                tiles.push_back(h);
            }
        }
        auto const sz = std::ssize(flipped);
        for (auto [h, n] : neighbours) { flipped.try_emplace(h, tile{n, 0}); }
        for (auto it = flipped.begin() + sz; it != flipped.end(); ++it) {
            auto const tile = it->second;
            bool b = tile.count % 2;
            auto count{0U};

            for (auto&& p : detail::generate_neighbours(tile.coord)) {
                auto it = flipped.find(hash(p));
                if (it == flipped.end()) { continue; }
                count += it->second.count % 2;
            }
            if ((b && (count == 0 || count > 2)) || (!b && count == 2) ) {
                tiles.push_back(it->first);
            }
        }
        for (auto h : tiles) { flipped[h].count++; }
        black = count_if(flipped, [](auto it) { return it.second.count % 2; });
    }
    fmt::print("part 2: {}\n", black);
}
