#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

namespace {
    auto count_valid(auto const& s, auto const& g) -> i64 {
        aoc::dense::map<u32, i64> seen;
        auto const n{std::ssize(s)};
        auto const m{std::ssize(g)};

        // we know that the three numbers that we cache (the string index, the groups index, and the current group)
        // are quite small: they will fit inside a u8. therefore, we can build a more efficient hash function
        auto hash = [](u32 a, u32 b, u32 c) -> u32 { return a << 16U | b << 8U | c; }; // NOLINT

        auto f = [&](u8 i, u8 j, u8 k, auto&& f) -> i64 {
            auto h = hash(i, j, k);
            if (auto it = seen.find(h); it != seen.end()) {
                 return it->second;
            }

            if (i == n) {
                return (j == m && k == 0) || (j == m-1 && k == g.back());
            }

            auto valid{0L};

            if (s[i] == '.' || s[i] == '?') {
                if (k == 0) {
                    valid += f(i+1, j, 0, f);
                } else {
                    if (j == n) {
                        return 0;
                    }
                    if (k == g[j]) {
                        valid += f(i+1, j+1, 0, f);
                    }
                }
            }

            if (s[i] == '#' || s[i] == '?') {
                valid += f(i+1, j, k+1, f);
            }

            seen.insert({h, valid});
            return valid;
        };

        return f(0, 0, 0, f);
    }
} // namespace

template<>
auto advent2023::day12() -> result {
    auto input = aoc::util::readlines("./source/2023/12/input.txt");

    auto unfold = [](auto const& p) {
        auto const& [s, g] = p;
        std::string q; q.reserve(s.size()*5 + 5);
        std::vector<i64> h; h.reserve(g.size() * 5);

        for (auto i = 0; i < 5; ++i) {
            std::ranges::copy(s, std::back_inserter(q));
            if (i < 4) { q.push_back('?'); }
            std::ranges::copy(g, std::back_inserter(h));
        }
        return std::pair{ std::move(q), std::move(h )};
    };

    using record = std::string;
    using groups = std::vector<i64>;

    std::vector<std::pair<record, groups>> input_p1;

    for (auto const& s : input) {
        auto i = s.find(' ');
        record r{s.begin(), s.begin()+i};
        std::string_view v{s.begin()+i+1, s.end()};
        auto read_value = [](auto&& r){ return scn::scan_value<i64>(r)->value(); };
        auto g = lz::map(std::views::split(v, ','), read_value).toVector();
        input_p1.emplace_back(r, g);
    }

    auto f = [&](auto const& p) { return count_valid(p.first, p.second); };
    auto const p1 = fp::transform_reduce(f, std::plus{}, 0L, input_p1);
    auto const p2 = fp::transform_reduce(f, std::plus{}, 0L, fp::transform(unfold, input_p1));
    return aoc::result(p1, p2);
}