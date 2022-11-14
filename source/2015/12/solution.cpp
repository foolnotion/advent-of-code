#include <aoc.hpp>
#include <nlohmann/json.hpp>

template<>
auto advent2015::day12() -> result {
    using json = nlohmann::json;

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
    auto p1 = sum(data, [](auto const&) { return false; }, sum);
    auto p2 = sum(data, contains_red, sum);
    return aoc::result(p1, p2);
}
