#include <aoc.hpp>

using std::ranges::count_if;
using vec = std::vector<u32>;

namespace {
struct rule {
    u32 index {};
    char literal {'_'};
    std::vector<std::vector<rule const*>> subrules;

    [[nodiscard]] auto operator()(std::string_view msg) const -> bool {
        auto matched = false;
        (void)match(msg, matched);
        return matched;
    }

    [[nodiscard]] auto match(std::string_view msg, bool& matched) const -> vec {
        if (matched) { return {}; }
        if (literal != '_') {
            matched = literal == msg[0] && msg.size() == 1;
            return literal == msg[0] ? vec{1} : vec{};
        }
        vec offsets;
        vec aux;
        for (auto const& rules : subrules) {
            vec matchcounts{0};
            for (auto const* rule : rules) {
                for (auto c : matchcounts) {
                    if (c == msg.size()) {
                        matched = false; // not a match if the rule is longer than the remaining message
                        continue;
                    }
                    for (auto&& d : rule->match({&msg[c], msg.size()-c}, matched)) {
                        if (d > 0) { aux.push_back(c+d); }
                    }
                }
                matchcounts.swap(aux);
                aux.clear();
                if (matchcounts.empty()) { break; }
            }
            std::copy(matchcounts.begin(), matchcounts.end(), std::back_inserter(offsets));
        }
        return offsets;
    };
};
} // namespace

namespace day19 {
auto parse(std::vector<std::string> const& input) {
    // find out the max rule index
    auto max { 0U };
    for (auto line = input.begin(); !line->empty(); ++line) {
        auto const& s = *line;
        auto n = s.find(':');
        std::string_view sv { s.data(), n };
        auto idx = aoc::util::read<u32>(sv);
        max = std::max(max, idx);
    }

    std::vector<rule> rules(max+1);
    std::vector<std::string> messages;

    auto line = input.begin();

    auto add_subrule = [&](auto& r, auto sv) {
        r.subrules.push_back(lz::map(lz::split(sv, ' '), [&](auto qv){
                auto v = aoc::util::read<u32>(qv);
                return static_cast<rule const*>(&rules[v]);
        }).toVector());
    };

    for (; !line->empty(); ++line) {
        auto const& s = *line;
        auto n = s.find(':');
        std::string_view sv { s.data(), n };
        auto idx = aoc::util::read<u32>(sv);
        auto& r = rules[idx];
        r.index = idx;

        sv = std::string_view { &s[n + 2], s.size() - n - 2};
         // check if this rule is a literal
        if (n = sv.find('"'); n != std::string::npos) {
            r.literal = sv[n + 1];
            continue;
        }
        if (auto n = sv.find('|'); n != std::string::npos) {
            add_subrule(r, std::string_view{sv.begin(), sv.begin()+n-1});
            add_subrule(r, std::string_view{sv.begin()+n+2, sv.end()});
        } else {
            add_subrule(r, sv);
        }
    }
    std::copy(line + 1, input.end(), std::back_inserter(messages));
    return std::make_tuple(std::move(rules), std::move(messages));
}
} // namespace day19

template <>
auto advent2020::day19() -> result
{
    auto input = aoc::util::readlines("./source/2020/19/input.txt");
    auto [ rules, messages ] = day19::parse(input);
    auto const& match = rules[0];
    auto p1 = count_if(messages, match);

    rules[8].subrules.push_back({ &rules[42], &rules[8] }); // NOLINT
    rules[11].subrules.push_back({ &rules[42], &rules[11], &rules[31] }); // NOLINT
    auto p2 = count_if(messages, match);
    return aoc::result(p1, p2);
}
