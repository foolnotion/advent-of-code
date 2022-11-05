#include <aoc.hpp>

struct bitset {
    u64 bits{};
    auto set(u32 i) { bits |= (u64{1} << i); }
    auto reset(u32 i) { bits &= ~(u64{1} << i); }
    auto operator[](u32 i) const -> bool { return bits & (u64{1} << i); }
};

struct state {
    u64 count;
    u64 sum;
    u64 product;
};

template<>
auto advent2015::day24() -> void {
    auto input = aoc::util::readlines("./source/2015/24/input.txt");
    auto pkgs = lz::map(input, [](auto const& s) { return scn::scan_value<u64>(s).value(); }).toVector();
    std::reverse(pkgs.begin(), pkgs.end());
    auto minc = std::numeric_limits<u64>::max();
    auto minp = std::numeric_limits<u64>::max();
    auto const sz = std::ssize(pkgs);
    aoc::dense::set<u64> seen;
    bitset used;

    auto find_sum = [&](auto i, auto t, state st, auto&& f) {
        auto [c, s, p] = st;
        if (std::tie(c, p) > std::tie(minc, minp)) { return; }
        if (auto [it, ok] = seen.insert(used.bits); !ok) { return; }
        if (s == t) {
            minc = c;
            minp = p;
            return;
        }
        if (i == sz) { return; }
        for (auto j = i; j < sz; ++j) {
            if (used[j] || c+1 > minc || p*pkgs[j] > minp || s+pkgs[j] > t) { continue; }
            used.set(j);
            f(i+1, t, {c+1, s+pkgs[j], p*pkgs[j]}, f);
            used.reset(j);
        }
    };

    auto const s = std::reduce(pkgs.begin(), pkgs.end());
    find_sum(0, s/3, {0, 0, 1}, find_sum);
    fmt::print("part 1: {}\n", minp);

    seen.clear();
    used.bits = 0;
    minc = std::numeric_limits<u64>::max();
    minp = std::numeric_limits<u64>::max();
    find_sum(0, s/4, {0, 0, 1}, find_sum);
    fmt::print("part 2: {}\n", minp);
}
