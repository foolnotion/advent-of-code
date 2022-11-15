#include <aoc.hpp>
#include <Eigen/Core>

template<>
auto advent2016::day02() -> result {
    auto input = aoc::util::readlines("./source/2016/02/input.txt");
    using point = Eigen::Array2i;
    aoc::dense::map<char, point> dirs {
        { 'U', {-1,  0} }, // up
        { 'D', {+1,  0} }, // down
        { 'L', { 0, -1} }, // left
        { 'R', { 0, +1} }, // right
    };

    Eigen::Array<int, 3, 3, Eigen::RowMajor> pad1;
    std::iota(pad1.data(), pad1.data() + pad1.size(), 1); // NOLINT

    Eigen::Array<int, 5, 5> pad2; // NOLINT
    pad2.setConstant(0);

    int m{2};
    int r{0};
    int c{1};
    for (auto i = 0; i < pad2.rows(); ++i) {
        for (auto j = m-r; j <= m+r; ++j) {
            pad2(i, j) = c++;
        }
        r += i < m ? 1 : -1;
    }

    auto valid_pad1 = [](auto const&, auto const& p) { return (p >= 0 && p < 3).all(); };
    auto valid_pad2 = [](auto const& pad, auto const& p) { return (p >= 0 && p < 5).all() && pad(p[0], p[1]) > 0; }; // NOLINT

    auto getcode = [&dirs](auto const& input, auto const& pad, auto&& valid) {
        point p{1, 1};
        std::string code;
        for (auto const& s : input) {
            for (auto c : s) {
                auto q = p + dirs[c];
                if (valid(pad, q)) { p = q; }
            }
            fmt::format_to(std::back_inserter(code), "{:X}", pad(p[0], p[1]));
        }
        return code;
    };
    auto part1 = getcode(input, pad1, valid_pad1);
    auto part2 = getcode(input, pad2, valid_pad2);
    return aoc::result(part1, part2);
}
