#include <aoc.hpp>

template <>
auto advent2015::day03() -> result
{
    std::fstream f("./source/2015/03/input.txt");
    std::string s;
    std::getline(f, s);

    using point = aoc::point<i64>;
    auto count_houses = [](auto const& input, bool robo) {
        ankerl::unordered_dense::map<point, u64> map;
        point p{0, 0}; // santa
        point q{0, 0}; // robo-santa
        for (auto c : input) {
            auto& [x, y] = p;
            switch (c) {
                case '>':
                    map[{x++, y}]++;
                    break;
                case 'v':
                    map[{x, y++}]++;
                    break;
                case '<':
                    map[{x--, y}]++;
                    break;
                case '^':
                    map[{x, y--}]++;
                    break;
                default:
                    break;
            }
            if (robo) { std::swap(p, q); }
        }
        map[p]++;
        map[q]++;
        return map.size();
    };
    return aoc::result(count_houses(s, false), count_houses(s,  true));
}
