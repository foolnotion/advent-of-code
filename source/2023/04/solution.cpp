#include <aoc.hpp>

namespace {
    template<std::size_t N>
    struct bitset {
        std::array<uint64_t, N> bits;

        auto set(i32 x) {
            constexpr u64 d = std::numeric_limits<uint64_t>::digits;
            bits[x / d] |= (1UL << x % d);
        }

        auto operator[](i32 x) const -> bool {
            constexpr u64 d = std::numeric_limits<uint64_t>::digits;
            return bits[x / d] & (1UL << x % d);
        }
    };
} // namespace

template<>
auto advent2023::day04() -> result {
    auto input = aoc::util::readlines("./source/2023/04/input.txt");
    auto const sz = std::ssize(input);
    std::vector<i32> counts(sz, 1);

    auto p1{0UL};
    for (auto k = 0L; k < std::ssize(input); ++k) {
        auto const& s = input[k];
        auto i = s.find(':');
        auto j = s.find('|');
        std::string_view u{s.begin()+i+2, s.begin()+j-1};
        std::string_view v{s.begin()+j+2, s.end()};

        bitset<2> bs{};
        for (auto const t : std::views::split(u, ' ')) {
            i32 x{}; (void)scn::scan(t, "{}", x);
            bs.set(x);
        }
        u32 n{0};
        for (auto const t : std::views::split(v, ' ')) {
            if (t.empty()) { continue; }
            i32 x{}; (void)scn::scan(t, "{}", x);
            n += bs[x];
        }
        if (n > 0) { p1 += 1U << (n-1); }

        for (auto l = k+1; l < std::min(k+n+1, sz); ++l) {
            counts[l] += counts[k];
        }
    }
    auto const p2 = std::reduce(counts.begin(), counts.end(), 0L);
    return aoc::result(p1, p2);
}