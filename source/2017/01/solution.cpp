#include <aoc.hpp>

template<>
auto advent2017::day01() -> result {
    std::fstream f("./source/2017/01/input.txt");
    if (!f.is_open()) {
        throw std::runtime_error("failed to open filename");
    }
    std::vector<u8> u;
    {
        std::string line;
        std::getline(f, line);
        for (auto it = line.begin(); it < line.end(); ++it) {
            std::string_view sv{it, it+1};
            u.push_back(scn::scan<u8>(sv, "{}")->value());
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
    return aoc::result(sum1, sum2);
}
