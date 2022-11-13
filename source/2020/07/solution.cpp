#include <aoc.hpp>

template<>
auto advent2020::day07() -> void {
    auto input = aoc::util::readlines("./source/2020/07/input.txt");
    using item = std::tuple<int, std::string>;
    aoc::dense::map<std::string, std::vector<item>> contains;
    aoc::dense::map<std::string, std::vector<std::string>> contained;

    for (auto const& line : input) {
        if (line.empty()) { continue; }
        auto tokens = lz::map(lz::split(line, ' '), [](auto v) { return std::string(v); }).toVector();
        auto key = tokens[0] + tokens[1];
        if (tokens[4] == "no") {
            continue;
        }
        for (size_t i = 4; i < tokens.size(); i += 4) {
            auto qty = scn::scan_value<int>(tokens[i]);
            if (!qty.has_value()) {
                throw std::runtime_error(fmt::format("unable to parse qty from string {}\n", tokens[i]));
            }

            auto s = tokens[i+1] + tokens[i+2];
            contains[key].emplace_back(qty.value(), s);
            contained[s].push_back(key);
        }
    }

    aoc::dense::set<std::string> counted;
    auto get_count = [&](std::string const& color, auto&& rec) {
        if (auto [it, ok] = counted.insert(color); !ok) { return; }
        if (auto it = contained.find(color); it != contained.end()) {
            for (auto const& s : it->second) {
                rec(s, rec);
            }
        }
    };
    get_count("shinygold", get_count);
    fmt::print("part 1: {}\n", counted.size()-1);

    auto get_content_count = [&](std::string const& color, auto&& rec) {
        int c = 1;
        auto it = contains.find(color);
        if (it == contains.end()) { return c; }
        c += std::transform_reduce(begin(it->second), end(it->second), 0, std::plus{}, [&](auto const& a) {
                auto const& [q1, s1] = a;
                return q1 * rec(s1, rec);
            });
        return c;
    };
    fmt::print("part 2: {}\n", get_content_count("shinygold", get_content_count) - 1);
}
