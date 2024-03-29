#include <aoc.hpp>

template<>
auto advent2015::day10() -> result {
    std::string const input{"1113222113"};

    auto compute_length = [](std::string const& input, int times) {
        std::string s = input;
        std::string q;

        for (auto k = 0; k < times; ++k) {
            auto sz = std::ssize(s);
            q.reserve(sz);
            for (auto i = 0; i < sz; ) {
                auto j = i;
                while (j < sz && s[i] == s[j]) { ++j; }
                for (auto d = (j-i) % 10; d > 0; d /= 10) { // NOLINT
                    q.push_back(static_cast<char>(d + '0'));
                }
                q.push_back(s[i]);
                i = j;
            }
            s.swap(q);
            q.clear();
        }

        return s.size();
    };
    auto p1 = compute_length(input, 40); // NOLINT
    auto p2 = compute_length(input, 50); // NOLINT
    return aoc::result(p1, p2);
}
