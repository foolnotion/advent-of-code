#include <aoc.hpp>

template<int Y>
auto print_results(advent<Y> adv) {
    for (auto d = 1; d <= 25; ++d) { // NOLINT
        auto [p1, p2] = adv(d);
        fmt::print(R"(/* day {:02d} */ tuple{{"{}", "{}"}},)", d, p1, p2);
        fmt::print("\n");
    }
};

auto main() -> int {
    constexpr int year{2022};
    constexpr int day{22};

    auto result = advent<year>{}(day);
    fmt::print("{}/{}: {}\n", year, day, result);
    return 0;
}
