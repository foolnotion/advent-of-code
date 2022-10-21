#include <aoc.hpp>
#include <scn/detail/file.h>

template<>
auto advent<2017>::day05() const -> void {

    scn::owning_file f("./source/2017/05/input.txt", "r");
    std::vector<i64> vec;
    auto result = scn::scan_list(f, vec);
    auto tmp = vec;

    u64 steps{0};
    for (i64 i = 0; i < vec.size();) {
        i += vec[i]++;
        ++steps;
    }
    fmt::print("part 1: {}\n", steps);

    steps = 0;
    std::swap(vec, tmp);
    for (i64 i = 0; i < vec.size();) {
        auto& v = vec[i];
        i += v;
        v += v < 3 ? 1 : -1;
        ++steps;
    }
    fmt::print("part 2: {}\n", steps);
}
