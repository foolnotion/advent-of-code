#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2024_results {
    /* day 01 */ std::tuple{"2285373", "21142653"},
    /* day 02 */ std::tuple{"606", "644"},
    /* day 03 */ std::tuple{"189600467", "107069718"},
    /* day 04 */ std::tuple{"2618", "2011"},
    /* day 05 */ std::tuple{"6949", "4145"},
    /* day 06 */ std::tuple{"4663", "1530"},
    /* day 07 */ std::tuple{"1260333054159", "162042343638683"},
    /* day 08 */ std::tuple{"320", "1157"},
    /* day 09 */ std::tuple{"6390180901651", "6412390114238"}
};

TEST_CASE("advent 2024 correctness" * doctest::test_suite("2024")) {
    advent2024 advent;
    for (auto i = 0; i < advent2024_results.size(); ++i) {
        CAPTURE(i);
        CHECK_EQ(advent2024_results[i], advent(i+1));
    }
};

TEST_CASE("advent 2024 performance" * doctest::test_suite("2024")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2024 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2024_results.size(); ++i) {
        bench.run(fmt::format("2024/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2024.csv"};
    bench.render(nb::templates::csv(), f);
};
