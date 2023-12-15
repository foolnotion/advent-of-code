#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

template <>
auto advent2023::day15() -> result
{
    auto input = aoc::util::readlines("./source/2023/15/input.txt");

    auto hash = [](std::string_view s) {
        auto f = [](u64 h, char c) { return (h + c) * 17 % 256; }; // NOLINT
        return fp::fold_left(f, 0UL, s);
    };

    auto seq = fp::split(',', true, input.front());
    auto const p1 = fp::transform_reduce(hash, std::plus{}, 0L, seq);

    // part 2
    std::array<aoc::dense::map<std::string_view, std::pair<u32, u32>>, 256> boxes; // NOLINT
    std::array<u64, boxes.size()> cnt {};

    for (std::string_view v : seq) {
        u8 focal_length{};
        char op{};
        if (std::isdigit(v.back())) {
            focal_length = v.back() - '0';
            v.remove_suffix(1);
        }
        op = v.back();
        v.remove_suffix(1);

        auto i = hash(v);
        if (op == '=') {
            if (auto [it, ok] = boxes[i].insert({ v, { focal_length, cnt[i]++ } }); !ok) {
                it->second.first = focal_length;
            }
        } else {
            boxes[i].erase(v);
        }
    }

    cppsort::merge_sorter sort{};
    auto box_focus = [&](auto i) -> u64 {
        if (boxes[i].empty()) { return 0; }
        auto values = boxes[i].values();
        sort(values, [](auto t) { return t.second.second; });
        auto lens_focus = [&](auto j) { return (j+1) * values[j].second.first; };
        return (i+1) * fp::transform_reduce(lens_focus, std::plus{}, 0L, fp::numbers(0UL, values.size()));
    };
    auto const p2 = fp::transform_reduce(box_focus, std::plus{}, 0UL, fp::numbers(0UL, boxes.size()));

    return aoc::result(p1, p2);
}