#include <aoc.hpp>
#include <util/md5.h>

namespace {
    using point = Eigen::Array<i32, 2, 1>;

    using brumme::MD5;

    struct state {
        static constexpr auto passcode{"dmypynyp"};
        static constexpr auto doors{"UDLR"};

        std::string path{};
        point pos{0,0};

        [[nodiscard]] static inline auto is_open(char c) { return aoc::equals<'b', 'c', 'd', 'e', 'f'>(c); }
        [[nodiscard]] inline auto neighbors() const {
            std::vector<state> nb;
            auto s = MD5{}(fmt::format("{}{}", passcode, path));

            std::array<point, 4> moves{ point{-1,  0}, {+1,  0}, { 0, -1}, { 0, +1} };
            for (auto i = 0; i < 4; ++i) {
                if (!is_open(s[i])) { continue; }
                auto newpos = pos+moves[i];
                if ((newpos >= 0 && newpos <= 3).all()) {
                    state q{path, newpos};
                    q.path.push_back(doors[i]); // NOLINT
                    nb.push_back(q);
                }
            }
            return nb;
        }
    };
} // namespace

template<>
auto advent2016::day17() -> result {
    std::queue<state> queue;
    queue.push({"", {0,0}});
    std::string shortest_path;
    auto longest_path{0UL};
    point goal{3,3};

    while(!queue.empty()) {
        auto s = queue.front();
        queue.pop();

        if ((s.pos == goal).all()) {
            if (shortest_path.empty()) { shortest_path = s.path; }
            longest_path = std::max(longest_path, s.path.size());
            continue;
        }

        for (auto&& q : s.neighbors()) {
            queue.push(q);
        }
    }
    return aoc::result(shortest_path, longest_path);
}
