#include <aoc.hpp>
#include "ut.hpp"
#include "nanobench.h"

namespace ut = boost::ut;

ut::suite const advent2020_test_suite = [] {
    using namespace ut; // NOLINT
    using std::tuple;
    namespace nb = ankerl::nanobench;

    std::array results {
        /* day 01 */ tuple{"1018336", "288756720"},
        /* day 02 */ tuple{"625", "391"},
        /* day 03 */ tuple{"278", "9709761600"},
        /* day 04 */ tuple{"242", "186"},
        /* day 05 */ tuple{"959", "527"},
        /* day 06 */ tuple{"7283", "3520"},
        /* day 07 */ tuple{"289", "30055"},
        /* day 08 */ tuple{"1709", "1976"},
        /* day 09 */ tuple{"530627549", "77730285"},
        /* day 10 */ tuple{"1690", "5289227976704"},
        /* day 11 */ tuple{"2166", "1955"},
        /* day 12 */ tuple{"562", "101860"},
        /* day 13 */ tuple{"102", "327300950120029"},
        /* day 14 */ tuple{"13105044880745", "3505392154485"},
        /* day 15 */ tuple{"232", "18929178"},
        /* day 16 */ tuple{"24980", "809376774329"},
        /* day 17 */ tuple{"276", "2136"},
        /* day 18 */ tuple{"3348222486398", "43423343619505"},
        /* day 19 */ tuple{"226", "355"},
        /* day 20 */ tuple{"140656720229539", "1885"},
        /* day 21 */ tuple{"2020", "bcdgf,xhrdsl,vndrb,dhbxtb,lbnmsr,scxxn,bvcrrfbr,xcgtv,"},
        /* day 22 */ tuple{"32815", "30695"},
        /* day 23 */ tuple{"74698532", "286194102744"},
        /* day 24 */ tuple{"485", "3933"},
        /* day 25 */ tuple{"10924063", ""},
    };

    advent2020 advent;

    "correctness"_test = [&] {
        for (auto [i, result] : lz::enumerate(results)) {
            expect(advent(i+1) == result) << fmt::format("2020/{:02d}", i+1);
        }
    };

    "performance"_test = [&] {
        nb::Bench bench;
        bench.output(nullptr);
        for (auto i = 1; i <= 25; ++i) {
            bench.run(fmt::format("2020/{:02d}", i), [&]() { return advent(i); });
        }
        bench.render(nb::templates::csv(), std::cout);
    };
};


