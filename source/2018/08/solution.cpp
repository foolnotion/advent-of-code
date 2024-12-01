#include <aoc.hpp>

template<>
auto advent2018::day08() -> result {
    auto input = aoc::util::readlines("source/2018/08/input.txt").front();
    auto svtostr = [](auto sv) { return std::string{sv.data(), sv.size()}; };
    auto split = std::views::split(input, ' ') | std::views::transform(svtostr) | std::views::transform(aoc::util::read<i32>);
    std::vector<i32> tree{split.begin(), split.end()};

    auto len = [&]() {
        std::vector<i32> len(tree.size(), 0);
        std::function<i32(i32)> compute_length = [&](auto i) {
            auto c = tree[i];
            auto m = tree[i+1UL];
            auto l = 0;
            if (c > 0) {
                for (auto k = 0; k < c; ++k) {
                    l += compute_length(i+l+2);;
                }
            }
            len[i] = l+m+2;
            return len[i];
        };
        compute_length(0);
        return len;
    }();

    auto metadata_sum = [&](auto i) {
        auto end = tree.begin() + i + len[i];
        auto beg = end - tree[i+1UL];
        return std::reduce(beg, end);
    };

    auto compute_value = [&](auto i, auto&& self) {
        auto c = tree[i];
        if (c == 0) { return metadata_sum(i); }
        auto m = tree[i+1UL];
        std::vector<i32> children;
        children.reserve(c);
        for (auto j = 0, k = i+2; j < c; ++j) {
            children.push_back(k);
            k += len[k];
        }
        auto l = len[i];
        auto child_value = [&](auto j) {
            return j > 0 && j <= std::ssize(children) ? self(children[j-1], self) : 0;
        };
        return std::transform_reduce(tree.begin()+i+l-m, tree.begin()+i+l, 0, std::plus{}, child_value);
    };

    auto nodes = std::views::iota(0L, std::ssize(tree)) | std::views::filter([&](auto i){ return len[i] > 0; });
    auto const p1 = std::transform_reduce(nodes.begin(), nodes.end(), 0UL, std::plus{}, metadata_sum);
    auto const p2 = compute_value(0, compute_value);
    return aoc::result(p1, p2);
}
