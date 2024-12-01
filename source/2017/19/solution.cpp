#include <aoc.hpp>

namespace {
    using point  = std::complex<i32>;

    struct direction {
        [[maybe_unused]] static constexpr auto right = point{0, +1};
        [[maybe_unused]] static constexpr auto down  = point{+1, 0};
        [[maybe_unused]] static constexpr auto left  = point{0, -1};
        [[maybe_unused]] static constexpr auto up    = point{-1, 0};
    };

    struct turn {
        static constexpr auto left  = point{0, +1};
        static constexpr auto right = point{0, -1};
    };
} // namespace

template<>
auto advent2017::day19() -> result {
    auto input = aoc::util::readlines("./source/2017/19/input.txt");
    auto rows = std::ssize(input);
    auto cols = std::ssize(*std::ranges::max_element(input, std::less{}, &std::string::size));

    Eigen::Array<char, -1, -1> map = decltype(map)::Constant(rows, cols, ' ');

    for (auto i = 0; i < rows; ++i) {
        std::ranges::transform(input[i], map.row(i).begin(), [](auto c) {
            if (std::isalpha(c)) {
                return c;
            }
            return c != ' ' ? '.' : ' ';
        });
    }

    point dir = direction::down;
    point start{0, 0};
    while (map(start.real(), start.imag()) != '.') {
        start += direction::right;
    }
    aoc::dense::set<char> seen;
    std::queue<point> queue;
    queue.push(start);

    auto valid = [rows, cols](auto const& map, point p) {
        auto [x, y] = std::tuple{p.real(), p.imag()};
        return x >= 0 && x < rows && y >= 0 && y < cols && map(x, y) != ' ';
    };

    auto p2 = 0L;

    auto steps{0L};
    while(!queue.empty()) {
        auto p = queue.front();
        queue.pop();

        ++steps;

        auto [i, j] = std::tuple{p.real(), p.imag()};
        auto c = map(i, j);

        if (std::isalpha(c)) {
            if (auto [it, ok] = seen.insert(c); !ok) {
                continue;
            }
            p2 = steps;
        }
        auto q = p + dir;
        auto r = p + dir * turn::left;
        auto s = p + dir * turn::right;

        if      (valid(map, q)) { queue.push(q); }
        else if (valid(map, r)) { queue.push(r); dir *= turn::left; }
        else if (valid(map, s)) { queue.push(s); dir *= turn::right; }
    }

    auto const& values = seen.values();
    auto p1 = std::string(values.begin(), values.end());

    return aoc::result(p1, p2);
}