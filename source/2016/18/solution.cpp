#include <aoc.hpp>

template<>
auto advent2016::day18() -> result {
    std::string input{".^^^^^.^^.^^^.^...^..^^.^.^..^^^^^^^^^^..^...^^.^..^^^^..^^^^...^.^.^^^^^^^^....^..^^^^^^.^^^.^^^.^^"};
    constexpr auto rows_p1{40};
    constexpr auto rows_p2{400'000};

    auto count = [&](auto rows) {
        std::vector<u8> v{1};
        std::transform(input.begin(), input.end(), std::back_inserter(v), aoc::equals<'.'>);
        v.push_back(1);
        auto u = v;

        auto sum = std::reduce(v.begin(), v.end(), 0UL);
        for (auto i = 0; i < rows-1; ++i) {
            for (auto j = 1; j < std::ssize(v)-1; ++j) {
                u[j] = v[j-1] == v[j+1];
            }
            std::swap(u, v);
            sum += std::reduce(v.begin(), v.end());
        }
        return sum - 2 * rows;
    };

    return aoc::result(count(rows_p1), count(rows_p2));
}
