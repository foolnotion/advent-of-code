#include <aoc.hpp>
#include <fplus/fplus.hpp>

namespace fp = fplus;

namespace {
    auto hash(std::string_view s) {
        auto f = [](u64 h, char c) { return (h + c) * 17 % 256; }; // NOLINT
        return fp::fold_left(f, 0UL, s);
    };

    auto organize_boxes(auto const& seq) {
        std::array<std::vector<std::pair<std::string_view, u32>>, 256> boxes;

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
            auto pred = [&](auto t) { return t.first == v; };
            auto& box = boxes[i];
            if (op == '=') {
                if (auto it = std::ranges::find_if(box, pred); it != box.end()) {
                    it->second = focal_length;
                } else {
                    box.emplace_back(v, focal_length);
                }
            } else {
                std::erase_if(boxes[i], pred);
            }
        }

        return boxes;
    }
} // namespace

template <>
auto advent2023::day15() -> result
{
    auto input = aoc::util::readlines("./source/2023/15/input.txt");
    auto seq = fp::split(',', true, input.front());

    auto boxes = organize_boxes(seq);
    auto box_focus = [&](auto i) {
        if (boxes[i].empty()) { return 0UL; }
        auto lens_focus = [&](auto j) { return (j+1) * boxes[i][j].second; };
        return (i+1) * fp::transform_reduce(lens_focus, std::plus{}, 0L, fp::numbers(0UL, boxes[i].size()));
    };

    auto const p1 = fp::transform_reduce(hash, std::plus{}, 0L, seq);
    auto const p2 = fp::transform_reduce(box_focus, std::plus{}, 0UL, fp::numbers(0UL, boxes.size()));
    return aoc::result(p1, p2);
}