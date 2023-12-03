#include <aoc.hpp>

namespace {
    auto rearrange(auto const& input, auto&& mover) {
        std::vector<std::vector<char>> stacks;
        for(auto const& line : input) {
            if (line.empty()) { continue; }
            if (std::isupper(line.front())) {
                stacks.emplace_back(line.rbegin(), line.rend());
                continue;
            }
            i32 count, from, to; // NOLINT
            (void)scn::scan(line, "move {} from {} to {}", count, from, to);
            mover(stacks[from-1], stacks[to-1], count);
        }
        return lz::map(stacks, [](auto const& s){ return s.back(); }).toString();
    }
} // namespace

template<>
auto advent2022::day05() -> result {
    auto input = aoc::util::readlines("./source/2022/05/input.txt");
    auto f1 = [](auto& src, auto& dst, auto count) {
        std::copy(src.rbegin(), src.rbegin()+count, std::back_inserter(dst));
        src.erase(src.end()-count, src.end());
    };
    auto f2 = [](auto& src, auto& dst, auto count) {
        std::copy(src.end()-count, src.end(), std::back_inserter(dst));
        src.erase(src.end()-count, src.end());
    };
    return aoc::result(rearrange(input, f1), rearrange(input, f2));
}
