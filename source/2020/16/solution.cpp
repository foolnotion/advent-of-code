#include <algorithm>
#include <aoc.hpp>

namespace {
struct range {
    u64 a;
    u64 b;

    [[nodiscard]] auto contains(u64 v) const { return v >= a && v <= b; }
    auto operator<(range const r) const { return std::tie(a, b) < std::tie(r.a, r.b); }
};
} // namespace

template<>
auto advent2020::day16() -> result {
    std::ifstream in("./source/2020/16/input.txt");
    std::string line;

    std::vector<std::array<range, 2>> ranges;
    std::vector<std::string> range_names;

    // parse valid ranges
    while (std::getline(in, line) && !line.empty()) {
        auto p = line.find(':');
        std::string_view sv(line.data(), p);
        range_names.emplace_back(sv);

        i64 i = p+2;
        i64 j = 0;
        while(line[i+j] != 'o') ++j;
        sv = std::string_view(line.data() + i, j-1);
        auto [a1, b1] = scn::scan<u64, u64>(sv, "{}-{}")->values();
        range r1 { a1, b1 };

        i += j + 3;
        j = std::ssize(line) - i;
        sv = std::string_view(&line[i], j);
        auto [a2, b2] = scn::scan<u64, u64>(sv, "{}-{}")->values();
        range r2 { a2, b2 };

        ranges.push_back({ r1, r2 });
    };

    std::vector<u64> myticket;

    std::getline(in, line); // skip empty line
    std::getline(in, line); // skip the line that says your ticket:
    // my ticket
    auto tok = lz::split(line, ',');
    auto read_value = aoc::util::read<u64>;

    std::transform(tok.begin(), tok.end(), std::back_inserter(myticket), read_value);

    // other tickets
    std::getline(in, line); // skip empty line
    std::getline(in, line); // skip the line that says nearby:
    std::vector<std::vector<u64>> nearby;
    while (std::getline(in, line) && !line.empty()) {
        auto tok = lz::split(line, ',');
        std::vector<u64> other;
        std::transform(tok.begin(), tok.end(), std::back_inserter(other), read_value);
        nearby.push_back(other);
    }

    auto is_valid = [&](auto const& ticket) {
        auto error_rate = 0UL;
        for (auto v : ticket) {
            if (std::none_of(ranges.begin(), ranges.end(), [&](auto const& r) { return r[0].contains(v) || r[1].contains(v); })) {
                error_rate += v;
            }
        }
        return std::pair{ error_rate, error_rate == 0 };
    };

    auto p1 = 0UL;
    for (auto const& t : nearby) {
        auto [err, valid] = is_valid(t);
        p1 += err;
    }

    std::vector<std::vector<u64>> valid_tickets;
    std::copy_if(nearby.begin(), nearby.end(), std::back_inserter(valid_tickets), [&](auto const& t) { return is_valid(t).second; });

    Eigen::Matrix<u64, -1, -1> m(valid_tickets.size(), ranges.size());
    for (auto&& [i, t] : lz::enumerate(valid_tickets)) {
        m.row(i) = Eigen::Map<Eigen::Array<u64, -1, 1>>(t.data(), t.size());
    }

    auto part2 = [&]() {
        Eigen::Matrix<int64_t, -1, -1, Eigen::ColMajor> cache(m.cols(), m.cols());
        cache.setZero();

        u64 p{1};
        std::vector<std::pair<int, int>> counts(ranges.size());
        for (auto i = 0L; i < cache.rows(); ++i) {
            auto range = ranges[i];
            for (auto j = 0L; j < cache.cols(); ++j) {
                auto values = std::span<u64>(m.col(j).data(), m.col(j).size());
                cache(i, j) = std::ranges::all_of(values, [&](auto v) { return range[0].contains(v) || range[1].contains(v); });
            }
            counts[i] = { i, (cache.row(i).array() == 1).count() };
        }
        std::sort(counts.begin(), counts.end(), [](auto a, auto b) { return a.second < b.second; });

        for (auto [index, count] : counts) {
            auto row = cache.row(index);
            int idx{};
            row.maxCoeff(&idx);
            cache.col(idx).setZero();

            if (range_names[index].find("departure") != std::string::npos) {
                p *= myticket[idx];
            }
        }
        return p;
    };
    auto p2 = part2();
    return aoc::result(p1, p2);
}
