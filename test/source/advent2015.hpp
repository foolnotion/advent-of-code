#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2015_results {
    /* day 01 */ std::tuple{"232", "1783"},
    /* day 02 */ std::tuple{"1606483", "3842356"},
    /* day 03 */ std::tuple{"2565", "2639"},
    /* day 04 */ std::tuple{"346386", "9958218"},
    /* day 05 */ std::tuple{"236", "51"},
    /* day 06 */ std::tuple{"377891", "14110788"},
    /* day 07 */ std::tuple{"16076", "2797"},
    /* day 08 */ std::tuple{"1350", "2085"},
    /* day 09 */ std::tuple{"141", "736"},
    /* day 10 */ std::tuple{"252594", "3579328"},
    /* day 11 */ std::tuple{"vzbxxyzz", "vzcaabcc"},
    /* day 12 */ std::tuple{"191164", "87842"},
    /* day 13 */ std::tuple{"733", "725"},
    /* day 14 */ std::tuple{"2640", "1102"},
    /* day 15 */ std::tuple{"13882464", "11171160"},
    /* day 16 */ std::tuple{"40", "241"},
    /* day 17 */ std::tuple{"654", "57"},
    /* day 18 */ std::tuple{"821", "886"},
    /* day 19 */ std::tuple{"518", "200"},
    /* day 20 */ std::tuple{"776160", "786240"},
    /* day 21 */ std::tuple{"111", "188"},
    /* day 22 */ std::tuple{"1824", "1937"},
    /* day 23 */ std::tuple{"307", "160"},
    /* day 24 */ std::tuple{"11846773891", "80393059"},
    /* day 25 */ std::tuple{"9132360", ""},
};

TEST_CASE("advent 2015 correctness" * doctest::test_suite("2015")) {
    advent2015 advent;
    for (auto i = 0; i < advent2015_results.size(); ++i) {
        CHECK_EQ(advent2015_results[i], advent(i+1));
    }
};

TEST_CASE("advent 2015 performance" * doctest::test_suite("2015")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2015 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= 25; ++i) {
        bench.run(fmt::format("2015/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2015.csv"};
    bench.render(nb::templates::csv(), f);
};