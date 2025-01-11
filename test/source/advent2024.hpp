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
    /* day 09 */ std::tuple{"6390180901651", "6412390114238"},
    /* day 10 */ std::tuple{"587", "1340"},
    /* day 11 */ std::tuple{"193607", "229557103025807"},
    /* day 12 */ std::tuple{"1304764", "811148"},
    /* day 13 */ std::tuple{"31761", "90798500745591"},
    /* day 14 */ std::tuple{"230172768", "8087"},
    /* day 15 */ std::tuple{"1486930", "1492011"},
    /* day 16 */ std::tuple{"99488", "516"},
    /* day 17 */ std::tuple{"4,6,1,4,2,1,3,1,6", "202366627359274"},
    /* day 18 */ std::tuple{"432", "56,27"},
    /* day 19 */ std::tuple{"247", "692596560138745"},
    /* day 20 */ std::tuple{"1497", "1030809"},
    /* day 21 */ std::tuple{"162740", "203640915832208"},
    /* day 22 */ std::tuple{"14476723788", "1630"},
    /* day 23 */ std::tuple{"1327", "df,kg,la,mp,pb,qh,sk,th,vn,ww,xp,yp,zk"},
    /* day 24 */ std::tuple{"58639252480880", "bkr,mqh,rnq,tfb,vvr,z08,z28,z39"},
    /* day 25 */ std::tuple{"3356", ""}
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
    using namespace std::literals;
    // bench.performanceCounters(true).timeUnit(1s, "s");
    advent2024 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};

    SUBCASE("day") {
        for (auto i = 1; i <= advent2024_results.size(); ++i) {
            bench.run(fmt::format("2024/{:02d}", i), [&](){
                auto const& [p1, p2] = advent(i);
                h += hash(p1) + hash(p2);
            });
        }
        std::ofstream f{"advent_2024.csv"};
        bench.render(nb::templates::csv(), f);
    }

    SUBCASE("year") {
        bench.run(fmt::format("advent2024"), [&](){
            for (auto i = 1; i <= advent2024_results.size(); ++i) {
                auto const& [p1, p2] = advent(i);
                h += hash(p1) + hash(p2);
            }
        });
        bench.render(nb::templates::csv(), std::cout);
    }
};
