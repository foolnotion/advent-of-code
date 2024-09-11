#include <aoc.hpp>

namespace {
    using interval = aoc::math::interval<i64>;

    auto parse_seeds(auto const& input) {
        auto const& s = input.front();
        std::string_view sv{s.begin()+s.find(':')+2, s.end()};
        return lz::map(lz::split(sv, ' '), aoc::util::read<i64>).toVector();
    }

    auto parse_ranges(auto const& input) {
        aoc::dense::map<u64, std::vector<std::pair<interval, interval>>> ranges;

        for (auto i = 2; i < std::ssize(input); ++i) {
            auto const& s = input[i];
            if (s.empty()) { continue; }
            if (auto const p = s.find('-'); p != std::string::npos) {
                std::string_view sv{s.begin(), s.begin()+s.find('-')};

                auto h = aoc::util::hash{}(sv);
                auto j = i+1;

                std::vector<std::pair<interval, interval>> map;
                for (; j < std::ssize(input); ++j) {
                    if (input[j].empty()) { break; }
                    auto [dst, src, len] = scn::scan<i64, i64, i64>(input[j], "{} {} {}")->values();
                    interval destination_interval{dst, dst+len-1};
                    interval source_interval{src, src+len-1};
                    map.emplace_back(source_interval, destination_interval);
                }
                ranges.insert({h, map});
                i = j;
            }
        }
        return ranges;
    }
} // namespace

template<>
auto advent2023::day05() -> result {
    auto input = aoc::util::readlines("./source/2023/05/input.txt");
    auto seeds  = parse_seeds(input);
    auto ranges = parse_ranges(input);

    std::vector<interval> ranges_p1;
    std::vector<interval> ranges_p2;
    ranges_p2.reserve(seeds.size()/2);
    for (auto i = 0L; i < std::ssize(seeds); i += 2) {
        ranges_p1.emplace_back(seeds[i], seeds[i]);
        ranges_p1.emplace_back(seeds[i+1], seeds[i+1]);
        ranges_p2.emplace_back(seeds[i], seeds[i]+seeds[i+1]-1);
    }

    auto cmp = [](interval lhs, interval rhs) {
        return std::tie(lhs.a, lhs.b) < std::tie(rhs.a, rhs.b);
    };

    auto lowest_location = [&](auto seed_ranges) {
        for (auto const& [_, v] : ranges) {
            std::vector<interval> mapped;
            for (auto r : seed_ranges) {
                std::vector<interval> unmapped{r};
                std::vector<interval> tmp;

                for (auto [src, dst]: v) {
                    for (auto p : unmapped) {
                        auto x = p & src;
                        if (!x.isempty()) {
                            mapped.emplace_back(x.a-src.a+dst.a, x.b-src.b+dst.b);
                            if (x.a > p.a) { tmp.emplace_back(p.a, x.a-1); }
                            if (x.b < p.b) { tmp.emplace_back(x.b+1, p.b); }
                        } else {
                            tmp.emplace_back(p.a, p.b);
                        }
                    }
                    std::swap(tmp, unmapped);
                    tmp.clear();
                }
                std::ranges::copy(unmapped, std::back_inserter(mapped));
            }
            std::swap(mapped, seed_ranges);
        }
        return std::ranges::min(seed_ranges, cmp);
    };

    auto const p1 = lowest_location(ranges_p1);
    auto const p2 = lowest_location(ranges_p2);
    return aoc::result(p1.a, p2.a);
}