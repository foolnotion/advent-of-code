#include <aoc.hpp>

template<>
auto advent2015::day02() -> void {
    std::fstream f("./source/2015/02/input.txt");

    u64 paper{0};
    u64 ribbon{0};
    for (std::string s; std::getline(f, s); ) {
        u64 l{0}; // length
        u64 w{0}; // width
        u64 h{0}; // height
        (void) scn::scan(s, "{}x{}x{}", l, w, h);
        auto a = l * w;
        auto b = w * h;
        auto c = h * l;
        paper += 2 * (a+b+c) + std::min({a, b, c});
        ribbon += 2 * std::min({l+w, w+h, h+l}) + l*w*h;
    }
    fmt::print("part 1: {}\n", paper);
    fmt::print("part 2: {}\n", ribbon);
}
