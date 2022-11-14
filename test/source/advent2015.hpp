#include <aoc.hpp>
#include "ut.hpp"
#include "nanobench.h"

namespace ut = boost::ut;

ut::suite const advent2015_test_suite = [] {
    using namespace ut; // NOLINT
    using std::tuple;
    namespace nb = ankerl::nanobench;

    std::array results {
        /* day 01 */ tuple{"232", "1783"},
        /* day 02 */ tuple{"1606483", "3842356"},
        /* day 03 */ tuple{"2565", "2639"},
        /* day 04 */ tuple{"346386", "9958218"},
        /* day 05 */ tuple{"236", "51"},
        /* day 06 */ tuple{"377891", "14110788"},
        /* day 07 */ tuple{"16076", "2797"},
        /* day 08 */ tuple{"1350", "2085"},
        /* day 09 */ tuple{"141", "736"},
        /* day 10 */ tuple{"252594", "3579328"},
        /* day 11 */ tuple{"vzbxxyzz", "vzcaabcc"},
        /* day 12 */ tuple{"191164", "87842"},
        /* day 13 */ tuple{"733", "725"},
        /* day 14 */ tuple{"2640", "1102"},
        /* day 15 */ tuple{"13882464", "11171160"},
        /* day 16 */ tuple{"40", "241"},
        /* day 17 */ tuple{"654", "57"},
        /* day 18 */ tuple{"821", "886"},
        /* day 19 */ tuple{"518", "200"},
        /* day 20 */ tuple{"776160", "786240"},
        /* day 21 */ tuple{"111", "188"},
        /* day 22 */ tuple{"1824", "1937"},
        /* day 23 */ tuple{"307", "160"},
        /* day 24 */ tuple{"11846773891", "80393059"},
        /* day 25 */ tuple{"9132360", ""},
    };

    advent2015 advent;

    "correctness"_test = [&] {
        for (auto [i, result] : lz::enumerate(results)) {
            expect(advent(i+1) == result) << fmt::format("2015/{:02d}", i+1);
        }
    };

    "performance"_test = [&] {
        nb::Bench bench;
        bench.output(nullptr);
        for (auto i = 1; i <= 25; ++i) {
            bench.run(fmt::format("2015/{:02d}", i), [&]() { return advent(i); });
        }
        bench.render(nb::templates::csv(), std::cout);
    };
};


