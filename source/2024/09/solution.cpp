#include <algorithm>
#include <aoc.hpp>

namespace rs = std::ranges;
namespace vs = std::views;

namespace {
struct file {
    u64 index;
    u64 id;
    u64 size;

    constexpr auto operator==(file const&) const -> bool = default;

    [[nodiscard]] auto checksum() const -> u64 {
        auto nums = vs::iota(index, index+size);
        return std::reduce(nums.begin(), nums.end()) * id;
    }
};

constexpr auto operator<=>(file const& a, file const& b) -> std::weak_ordering {
    return a.index <=> b.index;
}
} // namespace

template<>
struct fmt::formatter<file> {
    template<typename Context>
    constexpr auto parse(Context& ctx) { return ctx.begin(); }

    template<typename Context>
    constexpr auto format(file const& f, Context& ctx) const {
        return fmt::format_to(ctx.out(), "[{}, {}, {}]", f.index, f.id, f.size);
    }
};

template<>
auto advent2024::day09() -> result {
    auto const input = aoc::util::readlines("./source/2024/09/input.txt").front();
    constexpr auto n = 10UL;

    using queue = aoc::priority_queue<u64, rs::greater>;
    std::array<queue, n> space;
    aoc::priority_queue<file> files;
    for (auto i = 0, s = 0; i < std::ssize(input); ++i) {
        auto const v = input[i]-'0';
        if (i % 2 == 0) { files.emplace(s, i/2, v); }
        else            { if (v > 0) { space[v].push(s); } }
        s += v;
    }

    auto top_or_default = [](queue const& q) {
        return q.empty() ? ~0UL : q.top();
    };

    auto defrag = [&](auto files, auto space, bool move_whole_files = false) {
        aoc::priority_queue<file> defragged;

        while(!files.empty()) {
            auto f = files.top();
            files.pop();

            auto const x = move_whole_files ? f.size : 1;
            auto it = rs::min_element(space.begin()+x, space.end(), rs::less{}, top_or_default);

            if (it == space.end() || it->empty() || it->top() > f.index) {
                defragged.emplace(f.index, f.id, f.size);
                continue;
            }
            auto const s = rs::distance(space.begin(), it);
            auto const i = it->top();
            it->pop();

            if (f.size <= s) {
                defragged.emplace(i, f.id, f.size);
                space[f.size].push(f.index);
                if (s > f.size) {
                    space[s-f.size].push(i+f.size);
                }
            } else if (f.size > s) {
                if (move_whole_files) { throw std::runtime_error("logical problem"); }
                defragged.emplace(i, f.id, s);
                files.emplace(f.index, f.id, f.size-s);
            }
        }
        return defragged;
    };

    auto checksum = [](auto files) {
        auto const& vals = files.values();
        return std::transform_reduce(vals.begin(), vals.end(), 0UL, std::plus{}, std::mem_fn(&file::checksum));
    };

    auto const p1 = checksum(defrag(files, space, false));
    auto const p2 = checksum(defrag(files, space, true));
    return aoc::result(p1, p2);
}