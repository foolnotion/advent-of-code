#include <aoc.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<>
auto advent2015::day12() const -> void {
    std::ifstream f("./source/2015/12/input.txt");
    json data = json::parse(f);

    auto contains_red = [](json const& obj) -> bool {
        return obj.is_object() && std::ranges::any_of(obj, [](auto const& v) { return v == "red"; });
    };

    auto sum = [&](json const& obj, auto&& f, auto&& sum) -> i64 {
        if (obj.is_number()) { return obj.get<i64>(); }
        if (obj.is_primitive()) { return 0; }
        if (f(obj)) { return 0; }
        return std::transform_reduce(obj.begin(), obj.end(), 0L, std::plus{}, [&](auto const& v) { return sum(v, f, sum); });
    };

    fmt::print("part 1: {}\n", sum(data, [](auto const&) { return false; }, sum));
    fmt::print("part 2: {}\n", sum(data, contains_red, sum));
}
