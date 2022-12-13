#include <aoc.hpp>

template <>
auto advent2022::day12() -> result
{
    auto input = aoc::util::readlines("./source/2022/12/input.txt");
    auto const nrow{ std::ssize(input) };
    auto const ncol{ std::ssize(input.front()) };

    Eigen::Array<char, -1, -1> map(nrow, ncol);
    using point = Eigen::Array2i;
    point s{}, q{};
    for (auto&& [i, line] : lz::enumerate(input)) {
        map.row(i) = Eigen::Map<Eigen::Array<char, -1, 1>>(line.data(), line.size());
        if (auto j = line.find('S'); j != -1) {
            s = point{ i, j };
            map(i, j) = 'a';
        }
        if (auto j = line.find('E'); j != -1) {
            q = point { i, j };
            map(i, j) = 'z';
        }
    }

    auto find = [](auto const& map, point s, point q) -> std::optional<std::pair<u32, u32>> {
        Eigen::Array<u8, -1, -1> vis = decltype(vis)::Zero(map.rows(), map.cols());
        std::queue<std::tuple<point, u32>> queue;
        queue.push({ s, 0U });

        auto d{ std::numeric_limits<u32>::max() };
        while (!queue.empty()) {
            auto [p, c] = queue.front();
            queue.pop();
            if (vis(p[0], p[1])) {
                continue;
            }
            if ((p == q).all()) {
                return { std::pair { c, d } };
            }
            vis(p[0], p[1]) = 1;
            auto a = map(p[0], p[1]);
            if (a == 'a') {
                d = std::min(c, d);
            }

            for (auto r : { point{ 0, -1 }, { 0, +1 }, { -1, 0 }, { +1, 0 } }) {
                point t{p + r};
                auto [x, y] = std::tuple{t[0], t[1]};
                if (x < 0 || y < 0 || x >= map.rows() || y >= map.cols()) {
                    continue;
                }
                auto b = map(x, y);
                if (!(a > b && a - b > 1)) {
                    queue.push({ t, c + 1 });
                }
            }
        }
        return std::nullopt;
    };

    auto [part1, part2] = find(map, q, s).value();
    return aoc::result(part1, part2);
}
