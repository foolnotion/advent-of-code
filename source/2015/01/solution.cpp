#include <aoc.hpp>

template<>
auto advent<2015>::day01() const -> void {

    std::fstream f("./source/2015/01/input.txt");
    std::string line;
    std::getline(f, line);

    i64 floor{0};
    i64 pos{-1};
    for (auto i = 0; i < std::ssize(line); ++i) {
        auto c = line[i];
        floor += c == '(';
        floor -= c == ')';

        if (floor == -1 && pos == -1) {
            pos = i+1;
        }
    }
    fmt::print("part 1: {}\n", floor);
    fmt::print("part 2: {}\n", pos);
}
