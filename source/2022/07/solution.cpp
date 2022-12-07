#include <aoc.hpp>

template<>
auto advent2022::day07() -> result {
    aoc::util::hash hash;
    std::deque<u64> path;
    aoc::dense::map<u64, u64> nodes;

    constexpr auto root{ hash("/") };
    nodes.insert({ root, 0 });

    for (auto&& line : aoc::util::readlines("./source/2022/07/input.txt")) {
        switch(line.front()) {
        case 'd': {
            std::string dir;
            std::ignore = scn::scan(line, "dir {}", dir);
            nodes.insert({ path.back() + hash(dir), 0 });
            break;
        }
        case '$': {
            if (line[2] == 'c') {
                std::string dir;
                (void)scn::scan(line, "$ cd {}", dir);
                if (dir == "..") {
                    path.pop_back();
                } else {
                    auto h = path.empty() ? 0 : path.back();
                    path.push_back(h + hash(dir));
                }
            }
            break;
        }
        default: {
            u64 size{};
            std::string_view v{line.data(), line.find(' ')};
            (void)scn::scan(v, "{}", size);
            for (auto p : path) { nodes[p] += size; }
            break;
        }
        }
    }
    auto pred1 = [](auto t) { return t.second > 100'000 ? 0 : t.second; }; // find dirs with a total size at most 100'000 NOLINT
    auto const root_size{nodes[root]};
    auto pred2 = [root_size](auto t) { return t.second + 40'000'000 - root_size; }; // find dirs that would free up enough space NOLINT
    auto part1 = std::transform_reduce(nodes.begin(), nodes.end(), 0UL, std::plus{}, pred1);
    auto part2 = std::ranges::min_element(nodes, std::less{}, pred2)->second;
    return aoc::result(part1, part2);
}
