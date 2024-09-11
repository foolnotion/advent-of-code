#include <aoc.hpp>

template<>
auto advent2017::day02() -> result {
    std::fstream f("./source/2017/02/input.txt");
    std::vector<std::vector<i64>> values;
    for (std::string line; std::getline(f, line); ) {
        auto v = lz::map(lz::split(line, '\t'), [](auto const& s) { return scn::scan_value<i64>(s)->value(); }).toVector();
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
    auto p1 = transform_reduce(values.begin(), values.end(), i64{0}, std::plus{}, minmax_diff);
    auto p2 = transform_reduce(values.begin(), values.end(), i64{0}, std::plus{}, even_div);
    return aoc::result(p1, p2);
}
