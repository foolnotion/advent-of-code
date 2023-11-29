#include <aoc.hpp>

namespace {
    struct move {
        char c;
        u8 a;
        u8 b;
    };

    using aoc::constants::alphabet;

    auto parse(auto const& input) {
        std::vector<move> moves;
        for (auto const s : std::views::split(input, ',')) {
            std::string_view v{s.begin(), s.end()};
            move m{v.front(), 0, 0};

            switch(m.c) {
                case 's': {
                    (void)scn::scan(v, "s{}", m.a);
                    break;
                }
                case 'x': {
                    (void)scn::scan(v, "x{}/{}", m.a, m.b);
                    break;
                }
                case 'p': {
                    char a{};
                    char b{};
                    (void)scn::scan(v, "p{}/{}", a, b);
                    m.a = a - 'a';
                    m.b = b - 'a';
                    break;
                }
                default: {
                    throw std::runtime_error("unexpected input");
                }
            }
            moves.push_back(m);
        }
        return moves;
    }
} // namespace

template<>
auto advent2017::day16() -> result {
    auto input = aoc::util::readlines("./source/2017/16/input.txt").front();
    constexpr auto first{'a'};
    constexpr auto last{'p'};

    std::array<char, last-first+1> programs{};
    std::copy_n(alphabet.begin(), last-first+1, programs.begin());

    auto dance = [](auto& programs, auto const& moves) {
        for (auto [c, a, b] : moves) {
            switch(c) {
                case 's': {
                    std::ranges::rotate(programs, programs.end()-a);
                    break;
                }
                case 'x': {
                    std::swap(programs[a], programs[b]);
                    break;
                }
                case 'p': {
                    auto [i, j] = std::pair{0, 0};
                    for (auto k = 0; k < last-first+1; ++k) {
                        if (programs[k] == alphabet[a]) { i = k; }
                        else if (programs[k] == alphabet[b]) { j = k; }
                    }
                    std::swap(programs[i], programs[j]);
                    break;
                }
                default: { break; }
            }
        }
    };

    auto moves = parse(input);
    dance(programs, moves);
    std::string const p1{programs.begin(), programs.end()};

    // part 2
    aoc::dense::set<u64> seen;
    constexpr auto n{1'000'000'000};
    std::copy_n(alphabet.begin(), last-first+1, programs.begin());
    for(auto i = 0; i < n; ++i) {
        auto h = aoc::util::hash{}(programs);
        if (auto [it, ok] = seen.insert(h); !ok) {
            i = n - n % i;
        }
        dance(programs, moves);
    }
    std::string const p2{programs.begin(), programs.end()};
    return aoc::result(p1, p2);
}