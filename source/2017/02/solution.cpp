#include <aoc.hpp>

template<>
auto advent2017::day02() const -> void {
    std::fstream f("./source/2017/02/input.txt");

    std::vector<std::vector<i64>> values;

    for (std::string line; std::getline(f, line); ) {
        std::vector<i64> v;
        (void) scn::scan_list_ex(line, v, scn::list_separator('\t'));
        std::ranges::sort(v);
        values.push_back(v);
    }

    auto minmax_diff = [](auto const& row) { return row.back() - row.front(); };
    auto even_div = [](auto const& row) {
        for (auto v : row) {
            for (auto u = v; u + v <= row.back(); u += v) {
                if (std::ranges::binary_search(row, u + v)) {
                    return (u + v) / v;
                }
            }
        }
        return 0L;
    };

    using std::transform_reduce;
    fmt::print("part 1: {}\n", transform_reduce(values.begin(), values.end(), i64{0}, std::plus{}, minmax_diff));
    fmt::print("part 2: {}\n", transform_reduce(values.begin(), values.end(), i64{0}, std::plus{}, even_div));
}
