#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2023_results {
    /* day 01 */ std::tuple{"54159", "53866"},
    /* day 02 */ std::tuple{"2204", "71036"},
};

TEST_CASE("correctness" * doctest::test_suite("2023")) {
    for (auto [i, result] : lz::enumerate(advent2023_results)) {
        auto label = fmt::format("2023/{}", i+1);
        advent2023 advent;
        SUBCASE(label.c_str()) {
            CHECK_EQ(advent(i+1), result);
        }
    }
};

TEST_CASE("performance" * doctest::test_suite("2023")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2023 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2023_results.size(); ++i) {
        bench.run(fmt::format("2023/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2023.csv"};
    bench.render(nb::templates::csv(), f);
};