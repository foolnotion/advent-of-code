#include <aoc.hpp>

template<>
auto advent2017::day09() -> result {

    auto input = aoc::util::readlines("./source/2017/09/input.txt").front();
    std::vector<i32> groups;

    int depth{0}; // group depth
    int gc{0}; // garbage count
    int nc{0}; // cancelled count
    bool garbage = false;

    for (auto i = 0, j = 0; i < input.size(); ++i) {
        auto c = input[i];
        if (c == '!') {
            i += 1;
            nc += 2;
            continue;
        }
        if (c == '<') {
            if (!garbage) { j = i+1; }
            garbage = true;
        } else if (c == '>') {
            gc += i-j;
            garbage = false;
        }

        if (garbage) {
            continue;
        }

        if (c == '{') {
            depth += 1;
            groups.push_back(depth);
        }
        if (c == '}') {
            depth -= 1;
        }
    }
    auto p1 = std::reduce(groups.begin(), groups.end());
    auto p2 = gc - nc;
    return aoc::result(p1, p2);
}
