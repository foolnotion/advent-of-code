#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

namespace {
    auto count_valid(auto const& s, auto const& g) -> i64 {
        aoc::dense::map<u64, i64> seen;
        aoc::util::hash hash;

        auto const n{std::ssize(s)};
        auto const m{std::ssize(g)};

        auto f = [&](i64 i, i64 j, i64 k, auto&& f) -> i64 {
            if (auto it = seen.find(hash(i, j, k)); it != seen.end()) {
                return it->second;
            }

            if (i == n) {
                return j == m && k == 0 || j == m-1 && k == g.back();
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

            seen.insert({hash(i, j, k), valid});
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
        groups g;
        (void)scn::scan_list_ex(std::string_view{s.begin()+i+1, s.end()}, g, scn::list_separator(','));
        input_p1.emplace_back(r, g);
    }

    auto f = [&](auto const& p) { return count_valid(p.first, p.second); };
    auto const p1 = fp::transform_reduce(f, std::plus{}, 0L, input_p1);
    auto const p2 = fp::transform_reduce(f, std::plus{}, 0L, fp::transform(unfold, input_p1));
    return aoc::result(p1, p2);
}