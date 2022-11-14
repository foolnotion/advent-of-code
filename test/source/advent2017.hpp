#include <aoc.hpp>
#include "ut.hpp"
#include "nanobench.h"

namespace ut = boost::ut;

ut::suite const advent2017_test_suite = [] {
    using namespace ut; // NOLINT
    using std::tuple;
    namespace nb = ankerl::nanobench;

    std::array results {
        /* day 01 */ tuple{"1182", "1152"},
        /* day 02 */ tuple{"34581", "214"},
        /* day 03 */ tuple{"326", "363010"},
        /* day 04 */ tuple{"337", "231"},
        /* day 05 */ tuple{"373543", "27502966"},
        /* day 06 */ tuple{"6681", "2392"},
        /* day 07 */ tuple{"ahnofa", "802"},
    };

    advent2017 advent;

    "correctness"_test = [&] {
        for (auto [i, result] : lz::enumerate(results)) {
            expect(advent(i+1) == result) << fmt::format("2015/{:02d}", i+1);
        }
    };

    "performance"_test = [&] {
        nb::Bench bench;
        bench.output(nullptr);
        for (auto i = 1; i <= 7; ++i) {
            bench.run(fmt::format("2017/{:02d}", i), [&]() { return advent(i); });
        }
        bench.render(nb::templates::csv(), std::cout);
    };
};


