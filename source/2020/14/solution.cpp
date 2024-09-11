#include <aoc.hpp>
#include <bitset>

template<>
auto advent2020::day14() -> result {
    auto input = aoc::util::readlines("./source/2020/14/input.txt");
    aoc::dense::map<u64, u64> mem;
    std::string mask;

    constexpr auto nbits{36};
    // part 1
    for (auto const& line : input) {
        auto tokens = lz::split(line, ' ').toVector();

        if (tokens[0] == "mask") {
            mask = tokens[2];
        } else {
            // parse memory address
            std::string_view sv{tokens[0].data()+4, tokens[0].size()-5}; // NOLINT
            auto addr = aoc::util::read<u64>(sv);

            // parse value and apply mask
            auto val = aoc::util::read<u64>(tokens[2]);
            auto bits = std::bitset<nbits>(val);
            for(auto i = 0; i < nbits; ++i) {
                auto m = mask[nbits-i-1];
                if (m == '0') { bits[i] = false; }
                else if (m == '1') { bits[i] = true; }
            }
            mem[addr] = bits.to_ulong();
        }
    }

    auto get = [](auto p) { return p.second; };
    auto const map1 = lz::map(mem, get);
    auto p1 = std::reduce(map1.begin(), map1.end());

    // part 2
    std::bitset<nbits> bits;
    std::vector<int> floating;
    mem.clear();

    // write_mem function to handle the floating bits
    auto write_mem = [&](size_t v, size_t i, auto &&rec) {
        if (i == floating.size()) {
            mem[bits.to_ulong()] = v;
            return;
        }
        auto j = floating[i];
        for (auto b : { true, false }) {
            bits[j] = b;
            rec(v, i+1, rec);
        }
    };

    for (auto const& line : input) {
        auto tokens = lz::split(line, ' ').toVector();

        if (tokens[0] == "mask") {
            mask = tokens[2];
        } else {
            // parse memory address
            std::string_view sv{tokens[0].data()+4, tokens[0].size()-5}; // NOLINT
            auto addr = aoc::util::read<u64>(sv);

            // parse value and apply mask
            auto val = aoc::util::read<u64>(tokens[2]);
            bits = std::bitset<nbits>(addr);
            floating.clear();
            for(auto i = 0; i < nbits; ++i) {
                auto m = mask[nbits-i-1];
                if (m == '0') {
                    // unchanged
                } else if (m == '1') {
                    bits[i] = true;
                } else if (m == 'X') {
                    floating.push_back(i);
                }
            }
            write_mem(val, 0, write_mem);
        }
    }
    auto const map2 = lz::map(mem, get);
    auto p2 = std::reduce(map2.begin(), map2.end());
    return aoc::result(p1, p2);
}
