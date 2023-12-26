#include <aoc.hpp>

namespace {
    using connection = std::pair<u64, u64>;

    auto parse(auto const& input) {
        aoc::dense::map<u64, std::vector<u64>> connections;
        aoc::dense::map<u64, std::string> names;
        aoc::util::hash hash;

        for (auto const& s : input) {
            auto p = s.find(':');
            std::string_view v{s.begin(), s.begin()+p};
            std::string_view u{s.begin()+p+2, s.end()};

            auto hv = hash(v);
            names[hv] = v;

            for (auto const x : u | std::views::split(' ')) {
                std::string_view w{x.begin(), x.end()};
                fmt::print("{} -- {}\n", v, w);

                auto hx = hash(w);
                names[hx] = w;

                connections[hv].push_back(hx);
                connections[hx].push_back(hv);
            }
        }
        return std::tuple{std::move(connections), std::move(names)};
    }
} // namespace

template<>
auto advent2023::day25() -> result {
    auto input = aoc::util::readlines("./source/2023/25/input.txt");
    auto [conn, names] = parse(input);

    auto remove = [&](u64 a, u64 b) {
        std::erase(conn[a], b);
        std::erase(conn[b], a);
    };

    aoc::util::hash hash;

    // right now solved by visual inspection
    remove(hash("zlx"), hash("chr"));
    remove(hash("hlx"), hash("cpq"));
    remove(hash("hqp"), hash("spk"));

    auto count = [&](u64 start) {
        std::queue<u64> q;
        q.push(start);
        aoc::dense::set<u64> seen;
        while(!q.empty()) {
            auto n = q.front();
            q.pop();

            if (!seen.insert(n).second) {
                continue;
            }

            for (auto m : conn[n]) {
                q.push(m);
            }
        }
        return seen.size();
    };

    auto const p1 = count(hash("hlx")) * count(hash("zlx"));
    return aoc::result(p1, "");
}
