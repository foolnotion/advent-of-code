#include <aoc.hpp>

namespace {
auto valid(std::string_view sv) {
    return sv.substr(0, 3) == "mul" && std::ranges::all_of(sv.substr(3), [](auto c) {
        return std::isdigit(c) || c == ',' || c == '(' || c == ')';
    });
};

auto eval(std::string_view sv) {
    if (auto res = scn::scan<i64, i64>(sv, "mul({},{})"); res) {
        auto [a, b] = res->values();
        return a * b;
    }
    return 0L;
};
} // namespace

template<>
auto advent2024::day03() -> result {
    auto lines = aoc::util::readlines("./source/2024/03/input.txt");
    auto p1{0L};
    auto p2{0L};
    bool b{true};
    for (auto const& line : lines) {
        for (auto i = 0; i < std::ssize(line); ++i) {
            auto const* p = line.data();
            auto const c = line[i];

            if (c == 'd') {
                if (auto sv = std::string_view{p+i, 4}; sv == "do()") {
                    b = true;
                } else if (auto sv = std::string_view{p+i, 7}; sv == "don't()") {
                    b = false;
                }
                continue;
            }

            if (c == 'm' && std::string_view{p+i, 4} == "mul(") {
                auto j = line.find(')', i+4);
                std::string_view sv{p+i, j-i+1};
                if (valid(sv)) {
                    auto const v = eval(sv);
                    p1 += v;
                    p2 += b ? v : 0;
                }
            }
        }
    }
    return aoc::result(p1, p2);
}
