#include <aoc.hpp>
#include <Fastor/Fastor.h>

template<>
auto advent2017::day14() -> result {
    constexpr auto size{128};
    std::string input{"ljoxqyyw"};

    auto char2bin = [](char c) -> u8 { return std::isalpha(c) ? c - 'a' + 10 : c - '0'; };

    Fastor::Tensor<i32, size, size> grid{};
    grid.zeros();

    // part 1
    aoc::util::knot_hash hash{};
    auto p1{0};
    for (auto i : std::views::iota(0, size)) {
        auto h = hash(fmt::format("{}-{}", input, i));
        for (auto [j, c] : lz::enumerate(h)) {
            auto k = j * 4;
            for (auto x = char2bin(c); x > 0; x &= (x-1)) {
                ++p1;
                grid(i, k + 3 - std::countr_zero(x)) = 1;
            }
        }
    }

    // part 2
    auto p2{0};
    std::queue<std::pair<i32, i32>> queue;
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            if (grid(i, j) <= 0) { continue; }
            p2 += 1;
            queue.emplace(i, j);

            while (!queue.empty()) {
                auto [x, y] = queue.front();
                queue.pop();
                if (grid(x, y) <= 0) { continue; }
                grid(x, y) = -1;
                if (x-1 >= 0)   { queue.emplace(x-1, y); }
                if (x+1 < size) { queue.emplace(x+1, y); }
                if (y-1 >= 0)   { queue.emplace(x, y-1); }
                if (y+1 < size) { queue.emplace(x, y+1); }
            }
        }
    }
    return aoc::result(p1, p2);
}