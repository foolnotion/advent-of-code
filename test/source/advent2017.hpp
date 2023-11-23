#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2017_results {
    /* day 01 */ std::tuple{"1182", "1152"},
    /* day 02 */ std::tuple{"34581", "214"},
    /* day 03 */ std::tuple{"326", "363010"},
    /* day 04 */ std::tuple{"337", "231"},
    /* day 05 */ std::tuple{"373543", "27502966"},
    /* day 06 */ std::tuple{"6681", "2392"},
    /* day 07 */ std::tuple{"ahnofa", "802"},
    /* day 08 */ std::tuple{"2971", "4254"}
};

TEST_CASE("correctness" * doctest::test_suite("correctness")) {
    for (auto [i, result] : lz::enumerate(advent2017_results)) {
        auto label = fmt::format("2017/{}", i+1);
        advent2017 advent;
        SUBCASE(label.c_str()) {
            CHECK_EQ(advent(i+1), result);
        }
    }
};

TEST_CASE("performance" * doctest::test_suite("performance")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2017 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= 25; ++i) {
        bench.run(fmt::format("2017/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2017.csv"};
    bench.render(nb::templates::csv(), f);
};