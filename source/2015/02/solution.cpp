#include <aoc.hpp>

template<>
auto advent2015::day02() -> result {
    std::fstream f("./source/2015/02/input.txt");

    u64 paper{0};
    u64 ribbon{0};
    for (std::string s; std::getline(f, s); ) {
        auto result = scn::scan<u64, u64, u64>(s, "{}x{}x{}");
        if (!result) {
            throw std::runtime_error(fmt::format("scn: scan failed for string {}\n", s));
        }
        auto [l, w, h] = result->values();
        auto a = l * w;
        auto b = w * h;
        auto c = h * l;
        paper += 2 * (a+b+c) + std::min({a, b, c});
        ribbon += 2 * std::min({l+w, w+h, h+l}) + l*w*h;
    }
    return aoc::result(paper, ribbon);
}
