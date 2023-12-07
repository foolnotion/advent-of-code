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
    /* day 08 */ std::tuple{"2971", "4254"},
    /* day 09 */ std::tuple{"10820", "5547"},
    /* day 10 */ std::tuple{"23715", "541dc3180fd4b72881e39cf925a50253"},
    /* day 11 */ std::tuple{"650", "1465"},
    /* day 12 */ std::tuple{"378", "204"},
    /* day 13 */ std::tuple{"788", "3905748"},
    /* day 14 */ std::tuple{"8316", "1074"},
    /* day 15 */ std::tuple{"612", "285"},
    /* day 16 */ std::tuple{"kgdchlfniambejop", "fjpmholcibdgeakn"},
    /* day 17 */ std::tuple{"417", "34334221"},
    /* day 18 */ std::tuple{"2951", "7366"},
    /* day 19 */ std::tuple{"GINOWKYXH", "16636"},
    /* day 20 */ std::tuple{"300", "502"},
    /* day 21 */ std::tuple{"184", "2810258"},
    /* day 22 */ std::tuple{"5411", "2511416"},
};

TEST_CASE("advent 2017 correctness" * doctest::test_suite("2017")) {
    advent2017 advent;
    for (auto i = 0; i < advent2017_results.size(); ++i) {
        CHECK_EQ(advent2017_results[i], advent(i+1));
    }
};

TEST_CASE("advent 2017 performance" * doctest::test_suite("2017")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2017 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2017_results.size(); ++i) {
        bench.run(fmt::format("2017/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2017.csv"};
    bench.render(nb::templates::csv(), f);
};