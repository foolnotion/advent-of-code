#include <aoc.hpp>

namespace detail {
    // this function demonstrates an implementation using std::deque,
    // simulating the rotation around the circle while balancing the halves
    // (I was then able to derive an analytical formula by analyzing its output)
    // https://gist.github.com/foolnotion/be152bd90556461edf86d3bc93407001
    auto find_elf(auto n) {
        std::deque<u32> s(n/2);
        std::deque<u32> q(n/2+n%2);

        std::iota(s.begin(), s.end(), 1);
        std::iota(q.begin(), q.end(), s.size()+1);

        while(s.size() + q.size() > 1) {
            auto a = s.front(); s.pop_front();
            auto b = s.front(); q.pop_front();
            if(s.size() < q.size()) {
                s.push_back(q.front()); q.pop_front();
            }
            q.push_back(a);
        }
        return s.empty() ? q.front() : s.front();
    };
} // namespace detail

template<>
auto advent2016::day19() -> result {
    constexpr auto n{3018458U};

    auto part1 = [](auto n) { return 2 * (n - std::bit_floor(n)) + 1; };
    auto part2 = []<std::integral T>(T n) {
        auto p = static_cast<T>(std::floor(std::log(n) / std::log(3)));
        auto m = static_cast<T>(std::pow(3, p));
        if (n == m) { return n; }
        return n - m > m ? 2 * n - 3 * m : n - m;
    };

    // part 1: https://en.wikipedia.org/wiki/Josephus_problem
    return aoc::result(part1(n), part2(n));
}
