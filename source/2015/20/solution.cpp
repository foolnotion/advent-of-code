#include <aoc.hpp>

namespace {
    inline auto pow(auto x, auto p) -> u64 {
        if (p == 0) { return 1; }
        auto m = x;
        for (auto i = 1; i < p; ++i) { x *= m; }
        return x;
    };

    struct factorcalc {
        static constexpr int N{1 << 16}; // NOLINT
        static constexpr int P{6542}; // number of primes under N NOLINT

        bool marked[N] = {}; // NOLINT
        u64 magic[P] = {}; // NOLINT

        constexpr factorcalc() {
            u32 np{0}; // number of primes
            for (auto i = 2; i < N; ++i) {
                if (marked[i]) { continue; }
                magic[np++] = static_cast<u64>(-1) / i + 1;

                for (int j = 2 * i; j < N; j += i) {
                    marked[j] = true;
                }
            }
        }

        [[nodiscard]] auto find_factor(u64 n) const -> u64 {
            for (auto m : magic) {
                if (m * n < m) {
                    return static_cast<u64>(-1) / m + 1;
                }
            }
            return 1;
        }

        [[nodiscard]] auto factorize(u64 n) const -> std::vector<std::tuple<u64, u64>> {
            std::vector<std::tuple<u64, u64>> fac;
            u64 d{};
            do {
                d = find_factor(n);
                if (fac.empty() || d != std::get<0>(fac.back())) {
                    fac.emplace_back(d, 1);
                } else {
                    std::get<1>(fac.back()) += 1;
                }
                n /= d;
            } while (d != 1);
            return fac;
        }

        [[nodiscard]] auto divisors(u64 n) const -> std::vector<u64> {
            auto fac = factorize(n);
            std::vector<u64> aux;
            std::vector<u64> div{1};
            for (auto i = 0; i < std::ssize(fac); ++i) {
                if (std::get<0>(fac[i]) == 1) { continue; }
                auto [x, c] = fac[i];
                auto const rng = lz::range(c+1);
                auto const tmp = lz::map(lz::view(rng.begin()+1, rng.end()), [x=x](auto p) { return pow(x, p); }).toVector();
                for (auto&& [a,b] : lz::cartesian(div, tmp)) { aux.push_back(a * b); }
                std::copy(aux.begin(), aux.end(), std::back_inserter(div));
                aux.clear();
            }
            return div;
        };
    };
} // namespace

template<>
auto advent2015::day20() -> result {
    constexpr u64 input{33100000};
    constexpr u64 m1{10};
    constexpr u64 m2{11};
    constexpr u64 maxvisit{50};

    constexpr factorcalc calc{};

    aoc::dense::map<u64, u64> count;
    auto i{0};
    for(i = 1; ; ++i) {
        auto div = calc.divisors(i);
        u64 s{0};
        for (auto d : div) { count[d]++; s += d; }
        if (m1 * s >= input) { break; }
    }
    auto p1 = i;

    for(; ; ++i) {
        auto div = calc.divisors(i);
        auto fil = lz::filter(div, [&count](auto d) { return count[d] <= maxvisit; });
        if (m2 * std::reduce(fil.begin(), fil.end()) >= input) { break; }
    }
    auto p2 = i;
    return aoc::result(p1, p2);
}
