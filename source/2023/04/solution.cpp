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
        bitset<2> bs{};
        auto idx = static_cast<i64>(s.find(':'));
        std::string_view sv{s.begin()+idx+1, s.end()};

        auto f{false};
        u32 n{0};
        while (!sv.empty()) {
            // eat empty space
            if (sv.front() == ' ') {
                sv.remove_prefix(1); continue;
            }

            // eat the |
            if (sv.front() == '|') {
                f = true;
                sv.remove_prefix(1);
                continue;
            }

            // eat digit chars and parse into number
            auto i = std::min(sv.find(' '), sv.size());
            std::string_view qv{sv.data(), i};
            sv.remove_prefix(qv.size());

            auto x = scn::scan_value<i32>(qv)->value();

            // update bitset
            if (!f) { bs.set(x); } else { n += bs[x]; }
        }
        if (n > 0) { p1 += 1U << (n-1); }

        for (auto l = k+1; l < std::min(k+n+1, sz); ++l) {
            counts[l] += counts[k];
        }
    }
    auto const p2 = std::reduce(counts.begin(), counts.end(), 0L);
    return aoc::result(p1, p2);
}