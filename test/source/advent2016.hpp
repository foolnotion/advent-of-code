#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2016_results {
    /* day 01 */ std::tuple{"273", "115"},
    /* day 02 */ std::tuple{"97289", "9A7DC"},
    /* day 03 */ std::tuple{"982", "1826"},
    /* day 04 */ std::tuple{"173787", "548"},
    /* day 05 */ std::tuple{"d4cd2ee1", "f2c730e5"},
    /* day 06 */ std::tuple{"afwlyyyq", "bhkzekao"},
    /* day 07 */ std::tuple{"110", "242"},
    /* day 08 */ std::tuple{"116", "UPOJFLBCEZ"},
    /* day 09 */ std::tuple{"110346", "10774309173"},
    /* day 10 */ std::tuple{"101", "37789"},
    /* day 11 */ std::tuple{"33", "57"},
    /* day 12 */ std::tuple{"318117", "9227771"},
    /* day 13 */ std::tuple{"86", "127"},
    /* day 14 */ std::tuple{"14937", "19636"},
    /* day 15 */ std::tuple{"317371", "2080951"},
    /* day 16 */ std::tuple{"10010010110011010", "01010100101011100"},
    /* day 17 */ std::tuple{"RDRDUDLRDR", "386"},
    /* day 18 */ std::tuple{"1989", "19999894"},
    /* day 19 */ std::tuple{"1842613", "1424135"},
    /* day 20 */ std::tuple{"14975795", "101"},
    /* day 21 */ std::tuple{"baecdfgh", "cegdahbf"},
    /* day 22 */ std::tuple{"901", "238"},
    /* day 23 */ std::tuple{"14445", "479011005"},
    /* day 24 */ std::tuple{"464", "652"},
    /* day 25 */ std::tuple{"189", ""}
};

TEST_CASE("correctness" * doctest::test_suite("2016")) {
    for (auto [i, result] : lz::enumerate(advent2016_results)) {
        auto label = fmt::format("2016/{}", i+1);
        advent2016 advent;
        SUBCASE(label.c_str()) {
            CHECK_EQ(advent(i+1), result);
        }
    }
};

TEST_CASE("performance" * doctest::test_suite("2016")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2016 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= 25; ++i) {
        bench.run(fmt::format("2016/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2016.csv"};
    bench.render(nb::templates::csv(), f);
};