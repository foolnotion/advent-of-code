#include <iterator>
#include <aoc.hpp>
#include <scn/detail/file.h>

template<>
auto advent<2017>::day01() const -> void {
    fmt::print("2017 day 1\n");

    std::fstream f("./source/2017/01/input.txt");
    if (!f.is_open()) {
        fmt::print(stderr, "failed to open filename\n");
    }
    std::vector<u8> u;

    {
        std::string line;
        std::getline(f, line);
        fmt::print("{}\n", line);
        for (auto it = line.begin(); it < line.end(); ++it) {
            std::string_view sv{it, it+1};
            int d;
            auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + 1, d);
            u.push_back(d);
        }
    }

    auto const sz = std::ssize(u);
    u64 sum1{0};
    u64 sum2{0};
    for (auto i = 0L, j = i+1, k = sz/2; i < sz; ++i, j = (++j % sz), k = (++k % sz)) {
        sum1 += u[i] * (u[i] == u[j]);
        sum2 += u[i] * (u[i] == u[k]); 
    }
    fmt::print("part 1: {}\n", sum1);
    fmt::print("part 2: {}\n", sum2);
}
