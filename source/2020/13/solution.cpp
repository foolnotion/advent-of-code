#include <aoc.hpp>

template<>
auto advent2020::day13() -> result {
    auto input = aoc::util::readlines("./source/2020/13/input.txt");
    auto s = aoc::util::read<u64>(input.front());

    auto tokens = lz::split(input.back(), ',').toVector();
    std::vector<std::pair<u64, u64>> buses;

    size_t i = 0;
    for (auto& tok : tokens) {
        if (auto res = scn::scan_value<u64>(tok); res.has_value()) {
            buses.emplace_back( res->value(), i );
        }
        ++i;
    }

    u64 ans{std::numeric_limits<u64>::max()};
    u64 id{};
    for (auto [time, period] : buses) {
        auto wait = time - s % time;
        if (ans > wait) {
            ans = wait;
            id = time;
        }
    }
    auto p1 = ans * id;

    // part 2
    u64 a{0};
    u64 b{1};
    for (auto [interval, offset] : buses) {
        for (size_t k = 0; k < interval; ++k) {
            auto c = a + k * b;
            if ((c + offset) % interval == 0) {
                a = c;
                b = std::lcm(b, interval);
                break;
            }
        }
    }
    auto p2 = a;
    return aoc::result(p1, p2);
}
