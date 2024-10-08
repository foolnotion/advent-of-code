#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2018_results {
    /* day 01 */ std::tuple{"518", "72889"},
    /* day 02 */ std::tuple{"4980", "qysdtrkloagnfozuwujmhrbvx"},
    /* day 03 */ std::tuple{"118223", "412"},
    /* day 04 */ std::tuple{"3212", "4966"},
    /* day 05 */ std::tuple{"9172", "6550"},
    /* day 06 */ std::tuple{"3933", "41145"},
    /* day 07 */ std::tuple{"MNOUBYITKXZFHQRJDASGCPEVWL", "893"},
};

TEST_CASE("advent 2018 correctness" * doctest::test_suite("2018")) {
    advent2018 advent;
    for (auto i = 0; i < advent2018_results.size(); ++i) {
        CHECK_EQ(advent2018_results[i], advent(i+1));
    }
};

TEST_CASE("advent 2018 performance" * doctest::test_suite("2018")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2018 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2018_results.size(); ++i) {
        bench.run(fmt::format("2018/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2018.csv"};
    bench.render(nb::templates::csv(), f);
};