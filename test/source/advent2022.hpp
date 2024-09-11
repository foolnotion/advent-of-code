#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2022_results {
    /* day 01 */ std::tuple{"67450", "199357"},
    /* day 02 */ std::tuple{"12586", "13193"},
    /* day 03 */ std::tuple{"7701", "2644"},
    /* day 04 */ std::tuple{"547", "843"},
    /* day 05 */ std::tuple{"HNSNMTLHQ", "RNLFDJMCT"},
    /* day 06 */ std::tuple{"1343", "2193"},
    /* day 07 */ std::tuple{"1581595", "1544176"},
    /* day 08 */ std::tuple{"1779", "172224"},
    /* day 09 */ std::tuple{"6236", "2449"},
    /* day 10 */ std::tuple{"14540", "EHZFZHCZ"},
    /* day 11 */ std::tuple{"50616", "11309046332"},
    /* day 12 */ std::tuple{"497", "492"},
    /* day 13 */ std::tuple{"5366", "23391"},
    /* day 14 */ std::tuple{"763", "23921"},
    /* day 15 */ std::tuple{"5100463", "11557863040754"},
    /* day 16 */ std::tuple{"1694", "2582"},
    /* day 17 */ std::tuple{"3219", "1582758620701"},
    /* day 18 */ std::tuple{"4504", "2556"},
    /* day 19 */ std::tuple{"2193", "7200"},
    /* day 20 */ std::tuple{"6640", "11893839037215"},
    /* day 21 */ std::tuple{"38914458159166", "3665520865940"},
    /* day 22 */ std::tuple{"93226", "37415"},
    /* day 23 */ std::tuple{"16868", "1079"},
    /* day 24 */ std::tuple{"343", "960"},
    /* day 25 */ std::tuple{"2-0-020-1==1021=--01", ""},
};

TEST_CASE("advent 2022 correctness" * doctest::test_suite("2022")) {
    advent2022 advent;
    for (auto i = 0; i < advent2022_results.size(); ++i) {
        CAPTURE(i);
        CHECK_EQ(advent2022_results[i], advent(i+1));
    }
};

TEST_CASE("advent 2022 performance" * doctest::test_suite("2022")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2022 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2022_results.size(); ++i) {
        bench.run(fmt::format("2022/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2022.csv"};
    bench.render(nb::templates::csv(), f);
};