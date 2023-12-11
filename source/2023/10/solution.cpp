#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace {
    using point = aoc::point<i64, 2>;

    auto parse(auto const& input) {
        auto const nrows = std::ssize(input);
        auto const ncols = std::ssize(input.front());

        point start;
        Eigen::Array<char, -1, -1, Eigen::RowMajor> grid(nrows, ncols);
        for(auto i = 0; i < nrows; ++i) {
            auto const& s = input[i];
            for (auto j = 0; j < ncols; ++j) {
                grid(i, j) = s[j];
                if (s[j] == 'S') {
                    start = point{i, j};
                }
            }
        }
        return std::tuple{start, std::move(grid)};
    }
} // namespace

template<>
auto advent2023::day10() -> result {
    namespace fp = fplus;

    auto input = aoc::util::readlines("./source/2023/10/input.txt");
    auto [start, grid] = parse(input);
    auto const nrows{grid.rows()};
    auto const ncols{grid.cols()};

    auto valid = [&](point p) {
        auto [x, y] = p;
        return x >= 0 && y >= 0 && x < nrows && y < ncols && grid(x, y) != '.';
    };

    auto valid_pipe = [&](auto x , auto y, auto... c) {
        return valid({x, y}) && ((grid(x, y) == c) || ...);
    };

    auto neighbors = [&](point p) {
        auto [x, y] = p;
        switch(grid(x, y)) {
            case '|': return std::vector{ point{x+1, y}, point{x-1, y} }; // down, then up
            case '-': return std::vector{ point{x, y-1}, point{x, y+1} }; // left, then right
            case '7': return std::vector{ point{x, y-1}, point{x+1, y} }; // left, then down
            case 'J': return std::vector{ point{x, y-1}, point{x-1, y} }; // left, then up
            case 'L': return std::vector{ point{x, y+1}, point{x-1, y} }; // right, then up
            case 'F': return std::vector{ point{x+1, y}, point{x, y+1} }; // down, then right
            case 'S': {
                std::vector<point> nb; nb.reserve(4);
                if (valid_pipe(x+1, y, 'L', 'J', '|')) { nb.push_back({x+1, y}); }
                if (valid_pipe(x, y+1, 'J', '7', '-')) { nb.push_back({x, y+1}); }
                if (valid_pipe(x, y-1, 'F', 'L', '-')) { nb.push_back({x, y-1}); }
                if (valid_pipe(x-1, y, 'F', '7', '|')) { nb.push_back({x-1, y}); }
                return nb;
            }
            default: throw std::runtime_error(fmt::format("unknown grid char\n"));
        }
    };

    std::queue<point> queue;
    queue.emplace(start);
    i64 max_distance{0};
    Eigen::Array<i64, -1, -1> dist = decltype(dist)::Constant(nrows, ncols, 0);

    while(!queue.empty()) {
        auto p = queue.front();
        queue.pop();
        auto d = dist(p[0], p[1]);
        max_distance = std::max(d, max_distance);

        for (auto q : fp::keep_if(valid, neighbors(p))) {
            auto [x, y] = q;
            if (dist(x, y) > 0) { continue; }
            dist(x, y) = d+1;
            queue.push(q);
        }
    }
    auto const p1 = max_distance;

    // part 2
    std::vector<point> polygon;
    dist.setConstant(0);
    std::deque<point> stack{start};
    while(!stack.empty()) {
        auto p = stack.back();
        stack.pop_back();
        auto[i, j] = p;
        if (auto d = std::exchange(dist(i, j), 1); d) { continue; }
        polygon.push_back(p);
        std::ranges::copy(fp::keep_if(valid, neighbors(p)), std::back_inserter(stack));
    }
    polygon.push_back(start);

    auto f = [&](auto x, auto i) -> i64 {
        auto [a, b] = polygon[i];
        auto [c, d] = polygon[i+1];
        return x + (a+c)*(b-d);
    };
    auto a = fp::fold_left(f, 0L, fp::numbers(0L, std::ssize(polygon)-1));
    auto b = std::ssize(polygon) / 2;
    auto const p2 = std::abs(a)/2 + 1 - b;

    return aoc::result(p1, p2);
}