#include <aoc.hpp>

namespace {
using point = aoc::point<i32, 2>;
using instr = std::pair<char, u8>;

auto operator-(point a, point b) -> point { return { a[0]-b[0], a[1]-b[1] }; }
struct numbers{};
struct arrows{};

[[maybe_unused]] auto distance(point const p, point const q) -> i32 {
    return std::abs(p[0]-q[0]) + std::abs(p[1]-q[1]);
}

template<typename T>
auto coord(char /*unused*/) -> point { throw std::runtime_error("unsupported"); }

template<>
constexpr auto coord<numbers>(char c) -> point {
    switch (c) {
    case '7': return { 0, 0 };
    case '8': return { 0, 1 };
    case '9': return { 0, 2 };
    case '4': return { 1, 0 };
    case '5': return { 1, 1 };
    case '6': return { 1, 2 };
    case '1': return { 2, 0 };
    case '2': return { 2, 1 };
    case '3': return { 2, 2 };
    case ' ': return { 3, 0 };
    case '0': return { 3, 1 };
    case 'A': return { 3, 2 };
    default: throw std::runtime_error(fmt::format("numbers: unsupported char '{}'", c));
    }
}

template<>
constexpr auto coord<arrows>(char c) -> point {
    switch (c) {
    case ' ': return { 0, 0 };
    case '^': return { 0, 1 };
    case '<': return { 1, 0 };
    case 'v': return { 1, 1 };
    case '>': return { 1, 2 };
    case 'A': return { 0, 2 };
    default: throw std::runtime_error(fmt::format("arrows: unsupported char '{}'", c));
    }
}

template<typename T>
auto check_gap(instr i, point p) -> bool {
    auto [c, n] = i;
    auto const q = coord<T>(' ');
    return (c == '<' && point{p[0], p[1]-n} == q) ||
           (c == '>' && point{p[0], p[1]+n} == q) ||
           (c == 'v' && point{p[0]+n, p[1]} == q) ||
           (c == '^' && point{p[0]-n, p[1]} == q);
};

template<i64 D>
struct complexity_sum {
private:
    [[maybe_unused]] std::array<point, D+1> padpos_{coord<numbers>('A')};
    aoc::dense::map<std::tuple<point, instr, i32>, std::tuple<i64, point>> cache_;

public:
    explicit complexity_sum() {
        for (auto i = 1; i <= D; ++i) { padpos_[i] = coord<arrows>('A'); }
    }

    auto recursive_length(instr in, i32 depth) -> i64 { // NOLINT
        auto const p = padpos_[depth];
        if (auto it = cache_.find({p, in, depth}); it != cache_.end()) {
            auto [length, newpos] = it->second;
            padpos_[depth] = newpos;
            return length;
        }

        auto const [c, n] = in;
        auto const q = depth == 0 ? coord<numbers>(c) : coord<arrows>(c);
        padpos_[depth] = q; // update position

        // get possible moves
        std::vector<instr> moves;
        auto [x, y] = q-p;
        if (x != 0) { moves.emplace_back(x > 0 ? 'v' : '^', std::abs(x)); }
        if (y != 0) { moves.emplace_back(y > 0 ? '>' : '<', std::abs(y)); }

        std::ranges::sort(moves, std::greater{}, [&](auto const m){
            auto const gap = depth == 0 ? check_gap<numbers>(m, p) : check_gap<arrows>(m, p);
            return gap ? -1 : distance(coord<arrows>(m.first), coord<arrows>('A'));
        });
        moves.emplace_back('A', n);

        auto length{0L};
        for (auto const m : moves) {
            length += depth == D ? m.second : recursive_length(m, depth+1);
        }
        cache_[{p, in, depth}] = {length, q};
        return length;
    }

    auto operator()(instr i) -> i64 {
        return recursive_length(i, 0);
    }
};
} // namespace

template <>
auto advent2024::day21() -> result
{
    auto const input = aoc::util::readlines("./source/2024/21/input.txt");
    auto convert = [](std::string_view code) {
        auto transform = code | std::views::transform([](auto const c) { return instr{c, static_cast<u8>(1)}; });
        return std::vector(transform.begin(), transform.end());
    };
    auto complexity_p1 = complexity_sum<2>{};
    auto complexity_p2 = complexity_sum<25>{};

    auto [p1, p2] = std::tuple{0L, 0L};
    for (auto const& s : input) {
        auto const n = scn::scan<i32>(s.substr(0, 3), "{}")->value();
        auto const code = convert(s);
        p1 += std::transform_reduce(code.begin(), code.end(), 0L, std::plus{}, complexity_p1) * n;
        p2 += std::transform_reduce(code.begin(), code.end(), 0L, std::plus{}, complexity_p2) * n;
    }
    return aoc::result(p1, p2);
}
