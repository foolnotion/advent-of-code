#include <aoc.hpp>
#include <util/dynamic_bitset.hpp>
#define MDSPAN_IMPL_STANDARD_NAMESPACE std
#define MDSPAN_IMPL_PROPOSED_NAMESPACE experimental
#include <mdspan/mdspan.hpp>

using std::mdspan;
using std::extents;

namespace {
    using point = std::complex<i32>;
    using beam  = std::pair<point, point>;
    using grid  = mdspan<char, extents<u64, std::dynamic_extent, std::dynamic_extent>>;

    struct facing {
        static constexpr point left{0, -1};
        static constexpr point right{0, +1};
        static constexpr point up{-1, 0};
        static constexpr point down{+1, 0};

        static auto index(point p) {
            if (p == left)  { return 0; }
            if (p == right) { return 1; }
            if (p == up)    { return 2; }
            if (p == down)  { return 3; }
            throw std::runtime_error("invalid facing");
        }
    };

    auto encounter_vertical(auto& beams, auto dir, auto pos) {
        if (dir == facing::up || dir == facing::down) {
            beams.emplace_back(dir, pos + dir);
        } else {
            // beam splits up and down
            beams.emplace_back(facing::up, pos + facing::up);
            beams.emplace_back(facing::down, pos + facing::down);
        }
    };

    auto encounter_horizontal(auto& beams, auto dir, auto pos) {
        if (dir == facing::left || dir == facing::right) {
            beams.emplace_back(dir, pos + dir);
        } else {
            // beam splits left and right
            beams.emplace_back(facing::left, pos + facing::left);
            beams.emplace_back(facing::right, pos + facing::right);
        }
    }

    auto encounter_90degree_left(auto& beams, auto dir, auto pos) {
        if (dir == facing::right) { dir = facing::up; }
        else if (dir == facing::left) { dir = facing::down; }
        else if (dir == facing::up)   { dir = facing::right; }
        else if (dir == facing::down) { dir = facing::left; }
        beams.emplace_back(dir, pos + dir);
    }

    auto encounter_90degree_right(auto& beams, auto dir, auto pos) {
        if (dir == facing::right) { dir = facing::down; }
        else if (dir == facing::left) { dir = facing::up; }
        else if (dir == facing::up)   { dir = facing::left; }
        else if (dir == facing::down) { dir = facing::right; }
        beams.emplace_back(dir, pos + dir);
    }

    auto energize(grid map, beam b) {
        auto const nrows = map.extent(0);
        auto const ncols = map.extent(1);

        auto valid = [&](point p) {
            auto [x, y] = std::tuple{p.real(), p.imag()};
            return x >= 0 && x < nrows && y >= 0 && y < ncols;
        };

        std::array<sul::dynamic_bitset<u64>, 4> cache;
        for (auto& b : cache) { b.resize(nrows * ncols); }

        std::deque<beam> beams;
        beams.push_back(b);

        while(!beams.empty()) {
            auto [dir, pos] = beams.front();
            beams.pop_front();

            if (!valid(pos)) {
                continue;
            }

            auto const p{pos.real() * nrows + pos.imag()};
            auto& s = cache[facing::index(dir)];
            if (s.test(p)) {
                continue;
            }
            s.set(p);

            auto [i, j] = std::tuple{pos.real(), pos.imag()};
            auto c      = map(i, j);

            switch(c) {
                case '.': {
                    beams.emplace_back(dir, pos + dir);
                    break;
                }
                case '|': {
                    encounter_vertical(beams, dir, pos);
                    break;
                }
                case '-': {
                    encounter_horizontal(beams, dir, pos);
                    break;
                }
                case '/': {
                    encounter_90degree_left(beams, dir, pos);
                    break;
                }
                case '\\': {
                    encounter_90degree_right(beams, dir, pos);
                    break;
                }
                default: {
                    throw std::runtime_error(fmt::format("unknown location: {}", c));
                }
            }
        }
        return [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return ((cache[Idx] | ...)).count();
        }(std::make_index_sequence<cache.size()>{});
    };
} // namespace

template<>
auto advent2023::day16() -> result {
    auto input = aoc::util::readlines("./source/2023/16/input.txt");
    auto const nrows{std::ssize(input)};
    auto const ncols{std::ssize(input.front())};

    std::vector<char> buf;
    buf.reserve(nrows * ncols);
    auto copy = [&](auto const& s) { std::ranges::copy(s, std::back_inserter(buf)); };
    std::ranges::for_each(input, copy);

    grid grid(buf.data(), nrows, ncols);

    auto f = [&](beam b) { return energize(grid, b); };
    auto const p1 = f({ facing::right, point{0, 0} });

    auto f1 = [&](auto i) {
        return std::max(f({ facing::right, {i, 0} }), f({ facing::left, {i, static_cast<i32>(ncols-1)} }));
    };

    auto f2 = [&](auto i) {
        return std::max(f({ facing::down, {0, i} }), f({ facing::up, {static_cast<i32>(nrows-1), i} }));
    };

    auto max = [](auto i, auto&& f) {
        return std::ranges::max(std::views::iota(0, i) | std::views::transform(f));
    };
    auto const p2 = std::max(max(nrows, f1), max(ncols, f2));

    return aoc::result(p1, p2);
}