#include <aoc.hpp>
#include <doctest/doctest.h>
#include "nanobench.h"

std::array const advent2020_results {
    /* day 01 */ std::tuple{"1018336", "288756720"},
    /* day 02 */ std::tuple{"625", "391"},
    /* day 03 */ std::tuple{"278", "9709761600"},
    /* day 04 */ std::tuple{"242", "186"},
    /* day 05 */ std::tuple{"959", "527"},
    /* day 06 */ std::tuple{"7283", "3520"},
    /* day 07 */ std::tuple{"289", "30055"},
    /* day 08 */ std::tuple{"1709", "1976"},
    /* day 09 */ std::tuple{"530627549", "77730285"},
    /* day 10 */ std::tuple{"1690", "5289227976704"},
    /* day 11 */ std::tuple{"2166", "1955"},
    /* day 12 */ std::tuple{"562", "101860"},
    /* day 13 */ std::tuple{"102", "327300950120029"},
    /* day 14 */ std::tuple{"13105044880745", "3505392154485"},
    /* day 15 */ std::tuple{"232", "18929178"},
    /* day 16 */ std::tuple{"24980", "809376774329"},
    /* day 17 */ std::tuple{"276", "2136"},
    /* day 18 */ std::tuple{"3348222486398", "43423343619505"},
    /* day 19 */ std::tuple{"226", "355"},
    /* day 20 */ std::tuple{"140656720229539", "1885"},
    /* day 21 */ std::tuple{"2020", "bcdgf,xhrdsl,vndrb,dhbxtb,lbnmsr,scxxn,bvcrrfbr,xcgtv,"},
    /* day 22 */ std::tuple{"32815", "30695"},
    /* day 23 */ std::tuple{"74698532", "286194102744"},
    /* day 24 */ std::tuple{"485", "3933"},
    /* day 25 */ std::tuple{"10924063", ""},
};

TEST_CASE("correctness" * doctest::test_suite("2020")) {
    advent2020 advent;
    for (auto i = 0; i < advent2020_results.size(); ++i) {
        CHECK_EQ(advent2020_results[i], advent(i+1));
    }
};

TEST_CASE("performance" * doctest::test_suite("2020")) {
    namespace nb = ankerl::nanobench;
    nb::Bench bench;
    advent2020 advent;
    // bench.output(nullptr);
    aoc::util::hash hash;
    auto h{0UL};
    for (auto i = 1; i <= advent2020_results.size(); ++i) {
        bench.run(fmt::format("2020/{:02d}", i), [&](){
            auto const& [p1, p2] = advent(i);
            h += hash(p1) + hash(p2);
        });
    }
    std::ofstream f{"advent_2020.csv"};
    bench.render(nb::templates::csv(), f);
};