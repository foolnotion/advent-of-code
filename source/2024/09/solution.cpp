#include <aoc.hpp>

namespace {
struct block {
    i32 id{-1};
    i32 size{0};
    bool moved{false};
};
} // namespace

template<>
auto advent2024::day09() -> result {
    auto const input = aoc::util::readlines("./source/2024/09/input.txt").front();

    std::list<block> blocks;
    for (auto i = 0; i < std::ssize(input); ++i) {
        auto const id = i % 2 == 0 ? i/2 : -1;
        auto const size = input[i]-'0';
        blocks.insert(blocks.end(), {id, size});
    }

    auto checksum = [](auto const& blocks) {
        auto sum{0UL};
        auto i = 0;
        for (auto const& b : blocks) {
            if (b.id >= 0) {
                for (auto j = i; j < i + b.size; ++j) {
                    sum += b.id * j;
                }
            }
            i += b.size;
        }
        return sum;
    };

    auto fragment = [&](auto blocks, bool part2) {
        auto a = std::prev(blocks.end());

        while(true) {
            auto b = blocks.begin();
            for(; a != b && (a->id == -1 || a->moved); std::advance(a, -1)){}
            if (a == b) { break; }

            auto const size = part2 ? a->size : 1;
            for(; b != a && (b->id >=  0 || b->size < size); std::advance(b, +1)){}

            if (b == a) {
                if (!part2) { break; }
                a->moved = true;
                continue;
            }

            if (a->size == b->size) { // file fits exactly - do a swap
                a->moved = true;
                std::swap(*a, *b);
            } else if (a->size < b->size) { // file fits into space
                b->size -= a->size;
                blocks.insert(b, {a->id, a->size, true});
                a->id = -1;
            } else if (a->size > b->size) { // file doesn't fit
                ENSURE(!part2);
                b->id = a->id;
                a->size -= b->size;
            }
        }
        return checksum(blocks);
    };

    auto const p1 = fragment(blocks, false);
    auto const p2 = fragment(blocks, true);
    return aoc::result(p1, p2);
}