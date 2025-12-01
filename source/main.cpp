#include <aoc.hpp>

template<int Y>
auto print_results(advent<Y> const& adv) {
    for (auto d = 1; d <= 25; ++d) { // NOLINT
        auto&& res = adv(d);
        fmt::print("day {:02d}: {}\n", d, std::forward<typename advent<Y>::result const>(res));
    }
};

auto main() -> int {
    constexpr int year{2025};
    constexpr int day{1};

    auto result = advent<year>{}(day);
    fmt::print("{}/{:02d}: {}\n", year, day, result);

    // advent<year> a{};
    // print_results(a);

    return 0;
}
