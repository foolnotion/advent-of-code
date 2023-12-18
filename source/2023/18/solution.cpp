#include <aoc.hpp>

namespace {
    using point = Eigen::Array<i64, 2, 1>;
    using instr = std::pair<point, i64>;

    auto parse(auto const& input) {
        std::vector<instr> in1;
        std::vector<instr> in2;
        in1.reserve(input.size());
        in2.reserve(input.size());

        std::array directions{ point{0, +1}, point{+1, 0}, point{0, -1}, point{-1, 0} };
        aoc::dense::map<char, i64> dirmap{ {'R', 0}, {'D', 1}, {'L', 2}, {'U', 3} };
        constexpr auto b{16L};

        for (auto const& s : input) {
            char c{};
            i64 t{};
            i64 v{};
            std::string str;
            (void)scn::scan(s, "{} {} (#{})", c, t, str);
            (void)scn::scan("0x" + str, "{:x}", v);
            in1.emplace_back(directions[dirmap[c]], t);
            in2.emplace_back(directions[v % b], v / b);
        }

        return std::tuple{std::move(in1), std::move(in2)};
    }

    auto volume(auto const& instructions) -> i64 {
        auto b{0L}; // boundary points
        auto i{0L}; // interior points
        point p{0,0};
        for (auto [dir, steps] : instructions) {
            auto const q = p + steps * dir;
            i += (p(0)+q(0)) * (p(1)-q(1));
            b += steps;
            p = q;
        }
        return std::abs(i)/2 + b/2 + 1;
    }
} // namespace

template<>
auto advent2023::day18() -> result {
    auto input = aoc::util::readlines("./source/2023/18/input.txt");
    auto [in1, in2] = parse(input);
    auto const p1 = volume(in1);
    auto const p2 = volume(in2);
    return aoc::result(p1, p2);
}