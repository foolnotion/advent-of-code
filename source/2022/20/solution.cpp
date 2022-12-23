#include <aoc.hpp>

namespace {
    auto advance(auto& list, auto& it, auto n) {
        EXPECT(n >= 0);
        EXPECT(it != list.end());
        while(n-- > 0) {
            std::advance(it, 1);
            if (it == list.end()) {
                it = list.begin();
            }
        }
    };

    auto find_nth(auto& list, auto n) {
        auto it = std::ranges::find(list, 0);
        advance(list, it, n % list.size());
        return *it;
    };

    auto decrypt(auto list, auto rounds, auto key) {
        auto const sz = std::ssize(list);
        std::vector<typename decltype(list)::iterator> iters;
        auto it = list.begin();

        for (auto i = 0; i < sz; ++i) {
            *it *= key;
            iters.push_back(it);
            std::advance(it, 1);
        }

        for (auto i = 0; i < rounds; ++i) {
            for (auto it : iters) {
                auto p = it;
                auto n = aoc::math::mod_euclid(*it, sz-1) + 1;
                advance(list, p, n);
                list.splice(p, list, it);
            }
        }

        constexpr std::array coordinates { 1000, 2000, 3000 };
        return std::transform_reduce(coordinates.begin(), coordinates.end(), i64{0}, std::plus{}, [&](auto n) {
            return find_nth(list, n);
        });
    };
} // namespace

template<>
auto advent2022::day20() -> result {
    auto input = aoc::util::readlines("./source/2022/20/input.txt");
    auto list = lz::map(input, [](auto const& s) { return scn::scan_value<i64>(s).value(); }).to<std::list<i64>>();

    auto constexpr mix_p1{1L};
    auto constexpr key_p1{1L};
    auto constexpr mix_p2{10L};
    auto constexpr key_p2{811589153L};

    auto part1 = decrypt(list, mix_p1, key_p1);
    auto part2 = decrypt(list, mix_p2, key_p2);
    return aoc::result(part1, part2);
}
