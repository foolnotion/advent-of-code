#include <aoc.hpp>

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
            std::vector<std::string> tok;
            auto split_view = std::views::split(line, ' ');
            std::ranges::transform(split_view, std::back_inserter(tok), [](auto const sv) { return std::string{sv.begin(), sv.end()}; });
            if (tok[0] == "swap") {
                if (tok[1] == "position") {
                    int x{};
                    int y{};
                    std::ignore = scn::scan(line, "swap position {} with position {}", x, y);
                    swap_pos_xy(s, x, y);
                } else if (tok[1] == "letter") {
                    char x{};
                    char y{};
                    std::ignore = scn::scan(line, "swap letter {} with letter {}", x, y);
                    swap_let_xy(s, x, y);
                }
            } else if (tok[0] == "rotate") {
                if (tok[1] == "left") {
                    int k{};
                    std::ignore = scn::scan(line, "rotate left {} steps", k);
                    reverse ? rotate_right(s, k) : rotate_left(s, k);
                } else if (tok[1] == "right") {
                    int k{};
                    std::ignore = scn::scan(line, "rotate right {} steps", k);
                    reverse ? rotate_left(s, k) : rotate_right(s, k);
                } else if (tok[1] == "based") {
                    char x{};
                    std::ignore = scn::scan(line, "rotate based on position of letter {}", x);
                    reverse ? rotate_left_x(s, x) : rotate_right_x(s, x);
                }
            } else if (tok[0] == "reverse") {
                int x{};
                int y{};
                std::ignore = scn::scan(line, "reverse positions {} through {}", x, y);
                reverse_xy(s, x, y);
            } else if (tok[0] == "move") {
                int x{};
                int y{};
                std::ignore = scn::scan(line, "move position {} to position {}", x, y);
                reverse ? move_x_to_y(s, y, x) : move_x_to_y(s, x, y);
            }
        }
        return s;
    };

    part1 = scramble(part1, input);
    part2 = scramble(part2, std::views::reverse(input), /*reverse=*/true);
    return aoc::result(part1, part2);
}