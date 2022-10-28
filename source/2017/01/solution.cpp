#include <aoc.hpp>

template<>
auto advent2017::day01() const -> void {
    std::fstream f("./source/2017/01/input.txt");
    if (!f.is_open()) {
        fmt::print(stderr, "failed to open filename\n");
        return;
    }
    std::vector<u8> u;

    {
        std::string line;
        std::getline(f, line);
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

    i64 j{1};
    i64 k{sz/2};

    for (auto i = 0; i < sz; ++i) {
        sum1 += u[i] * (u[i] == u[j]);
        sum2 += u[i] * (u[i] == u[k]);

        j = (j+1) % sz;
        k = (k+1) % sz;
    }

    fmt::print("part 1: {}\n", sum1);
    fmt::print("part 2: {}\n", sum2);
}
