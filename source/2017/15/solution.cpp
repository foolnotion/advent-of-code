#include <aoc.hpp>

namespace {
    struct generator {
        static constexpr u64 divisor{2147483647};
        static constexpr u64 mask{0xffff};

        u64 factor{};
        u64 value{};

        auto operator()() -> u32 {
            value = value * factor % divisor;
            return value & mask;
        }

        auto operator()(u64 d) -> u32 {
            do { (*this)(); } while(value % d != 0); // NOLINT
            return value & mask;
        }
    };
} // namespace

template<>
auto advent2017::day15() -> result {
    constexpr auto params_a = std::pair{16807UL, 722UL};
    constexpr auto params_b = std::pair{48271UL, 354UL};
    constexpr auto pairs_p1{40'000'000};
    constexpr auto pairs_p2{5'000'000};

    auto count_p1 = [](auto pa, auto pb, auto n) {
        generator a{pa.first, pa.second};
        generator b{pb.first, pb.second};
        auto v = std::views::iota(0, n);
        return std::transform_reduce(v.begin(), v.end(), 0, std::plus{}, [&](auto) { return a() == b(); });
    };

    auto count_p2 = [](auto pa, auto pb, auto n) {
        generator a{pa.first, pa.second};
        generator b{pb.first, pb.second};
        constexpr auto ma{4};
        constexpr auto mb{8};
        auto u = std::views::iota(0, n);
        return std::transform_reduce(u.begin(), u.end(), 0, std::plus{}, [&](auto) { return a(ma) == b(mb); });
    };

    auto const p1 = count_p1(params_a, params_b, pairs_p1);
    auto const p2 = count_p2(params_a, params_b, pairs_p2);
    return aoc::result(p1, p2);
}