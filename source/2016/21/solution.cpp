#include <aoc.hpp>
#include <fplus/fplus.hpp>

template<>
auto advent2016::day21() -> result {
    using std::ranges::reverse;
    using std::ranges::rotate;

    auto input = aoc::util::readlines("./source/2016/21/input.txt");
    std::string part1 = "abcdefgh";
    std::string part2 = "fbgdceah";

    auto rotate_right = [](auto& s, int k) {
        rotate(s, s.end() - k);
    };

    auto rotate_left  = [](auto& s, int k) {
        rotate(s, s.begin() + k);
    };

    auto const sz = std::ssize(part1);
    std::vector<int> rmap(sz, 0);
    for (auto i = 0; i < sz; ++i) {
        int r = 1 + i + static_cast<int>(i >= 4);
        rmap[(i+r) % sz] = r % sz;
    }

    auto rotate_right_x = [&](auto& s, char x) {
        auto i = s.find(x);
        auto k = (1 + i + static_cast<int>(i >= 4)) % s.size();
        rotate_right(s, k);
    };

    auto rotate_left_x = [&](auto& s, char x) {
        auto i = s.find(x);
        rotate_left(s, rmap[s.find(x)]);
    };

    auto reverse_xy   = [](auto& s, int x, int y) {
        reverse(s.begin()+x, s.begin()+y+1);
    };

    auto move_x_to_y  = [](auto& s, int x, int y) {
        if (x == y) { return; }
        auto a = s.begin()+x;
        auto b = s.begin()+y+1;
        if (x < y) { rotate(a, a+1, b); }
        else       { rotate(b, b+x-y, a); }
    };

    auto swap_pos_xy  = [](auto& s, int x, int y) { std::swap(s[x], s[y]); };
    auto swap_let_xy  = [](auto& s, char x, char y) {
        auto i = s.find(x);
        auto j = s.find(y);
        std::swap(s[i], s[j]);
    };

    auto scramble = [&](auto s, auto const& input, bool reverse = false) {
        for (auto const& line : input) {
            auto tok = fplus::split(' ', false, line);
            if (tok[0] == "swap") {
                if (tok[1] == "position") {
                    auto [x, y] = scn::scan<i32, i32>(line, "swap position {} with position {}")->values();
                    swap_pos_xy(s, x, y);
                } else if (tok[1] == "letter") {
                    auto [x, y] = scn::scan<char, char>(line, "swap letter {} with letter {}")->values();
                    swap_let_xy(s, x, y);
                }
            } else if (tok[0] == "rotate") {
                if (tok[1] == "left") {
                    auto k = scn::scan<i32>(line, "rotate left {} step")->value();
                    reverse ? rotate_right(s, k) : rotate_left(s, k);
                } else if (tok[1] == "right") {
                    auto k = scn::scan<i32>(line, "rotate right {} step")->value();
                    reverse ? rotate_left(s, k) : rotate_right(s, k);
                } else if (tok[1] == "based") {
                    auto x = scn::scan<char>(line, "rotate based on position of letter {}")->value();
                    reverse ? rotate_left_x(s, x) : rotate_right_x(s, x);
                }
            } else if (tok[0] == "reverse") {
                auto [x, y] = scn::scan<i32, i32>(line, "reverse positions {} through {}")->values();
                reverse_xy(s, x, y);
            } else if (tok[0] == "move") {
                auto [x, y] = scn::scan<i32, i32>(line, "move position {} to position {}")->values();
                reverse ? move_x_to_y(s, y, x) : move_x_to_y(s, x, y);
            }
        }
        return s;
    };

    part1 = scramble(part1, input);
    part2 = scramble(part2, std::views::reverse(input), /*reverse=*/true);
    return aoc::result(part1, part2);
}