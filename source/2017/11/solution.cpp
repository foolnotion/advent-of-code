#include <aoc.hpp>
#include <Fastor/Fastor.h>

template<>
auto advent2017::day11() -> result {
    using point = Fastor::Tensor<i32, 3>;

    point const n  = point{+1, -1, 0};
    point const nw = point{0, -1, +1};
    point const sw = point{-1, 0, +1};
    point const s  = point{-1, +1, 0};
    point const se = point{0, +1, -1};
    point const ne = point{+1, 0, -1};

    aoc::dense::map<std::string_view, point> move {
        { "n" , n  },
        { "nw", nw },
        { "sw", sw },
        { "s" , s  },
        { "se", se },
        { "ne", ne }
    };

    auto input = aoc::util::readlines("./source/2017/11/input.txt").front();
    auto distance = [](auto const p) { return Fastor::max(Fastor::abs(p)); };

    point p{0,0,0};
    auto p1{0};
    auto p2{0};
    for (auto const s : std::views::split(input, ',')) {
        p += move[std::string_view{s.begin(), s.end()}];
        p2 = std::max(p2, distance(p));
    }
    p1 = distance(p);

    return aoc::result(p1, p2);
}
