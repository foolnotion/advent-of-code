#include <aoc.hpp>
#include <2019/intcode/intcode.hpp>

template<>
auto advent2019::day05() -> result {
    auto input = aoc::util::readlines("./source/2019/05/input.txt");
    std::vector<i64> intcode;
    (void)scn::scan_list_ex(input.front(), intcode, scn::list_separator(','));

    using aoc::interpreters::intcode::interpreter;

    auto run_diagnostic = [](auto intcode, auto in) {
        interpreter t(intcode);
        t.input = in;
        t();
        return t.output.value();
    };

    auto const p1 = run_diagnostic(intcode, 1);
    auto const p2 = run_diagnostic(intcode, 5);
    return aoc::result(p1, p2);
}