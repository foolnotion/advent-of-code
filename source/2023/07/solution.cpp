#include <aoc.hpp>

namespace {
    struct card_strength {
        static constexpr std::array faces = {'T', 'J', 'Q', 'K', 'A'};
        static constexpr std::array strength = {10, 11, 12, 13, 14};
        static constexpr std::array strength_with_joker = {10, 1, 12, 13, 14};

        static auto value(char c, bool joker) {
            if (std::isdigit(c)) { return c - '0'; }
            i32 i{0}; while(c != faces[i]) { ++i; }
            return joker ? strength_with_joker[i] : strength[i];
        }
    };

    struct hand {
        static constexpr auto hand_size{5};
        std::array<char, hand_size> cards{};
        i64 bid{};

        std::pair<i64, i64> card_groups{};
        bool with_joker{false};

        [[nodiscard]] auto winnings(auto i) const { return i * bid; }
        friend auto operator<(hand const& a, hand const& b);
    };

    auto count_groups(auto s, bool with_joker) {
        cppsort::merge_sorter{}(s);
        auto ndup{0}; // number of duplicates
        auto gmax{0}; // max group size
        auto njok = with_joker ? std::ranges::count(s, 'J') : 0;
        if (njok == std::ssize(s)) { return std::make_pair(1L, 5L); }

        for (auto i{0}; i < std::ssize(s)-1;) {
            auto c{0};
            auto j{i+1};
            if (with_joker) {
                while(j < std::ssize(s) && s[i] != 'J' && s[i] == s[j]) { ++j; ++c; }
            } else {
                while(j < std::ssize(s) && s[i] == s[j]) { ++j; ++c; }
            }
            ndup += c;
            gmax = std::max(c+1, gmax);
            i = j;
        }
        return std::make_pair(std::ssize(s) - ndup - njok, gmax + njok);
    };

    auto operator<(hand const& a, hand const& b) {
        auto [n, m] = a.card_groups;
        auto [u, v] = b.card_groups;

        if (n != u) { return n > u; }
        if (m == v) {
            return std::ranges::lexicographical_compare(a.cards, b.cards, [&](auto x, auto y) {
                return card_strength::value(x, a.with_joker) < card_strength::value(y, b.with_joker);
            });
        }
        return m < v;
    }

    auto parse(auto const& input) {
        return lz::map(input, [](auto const& s) {
            std::array<std::string, 2> a;
            (void)scn::scan(s, "{} {}", a[0], a[1]);
            hand h;
            std::ranges::copy(a[0], h.cards.begin());
            (void)scn::scan(a[1], "{}", h.bid);
            return h;
        }).toVector();
    }
} // namespace

template<>
auto advent2023::day07() -> result {
    auto input = aoc::util::readlines("./source/2023/07/input.txt");
    auto hands = parse(input);

    auto winnings = [](auto& hands, bool with_joker) {
        for (auto& hand : hands) {
            hand.with_joker  = with_joker;
            hand.card_groups = count_groups(hand.cards, with_joker);
        }
        cppsort::merge_sorter{}(hands);
        auto ranked = lz::enumerate(hands, 1);
        return std::transform_reduce(ranked.begin(), ranked.end(), 0L, std::plus{},
            [](auto const& t) { return t.second.winnings(t.first); });
    };

    auto const p1 = winnings(hands, /*with_joker=*/false);
    auto const p2 = winnings(hands, /*with_joker=*/true);
    return aoc::result(p1, p2);
}