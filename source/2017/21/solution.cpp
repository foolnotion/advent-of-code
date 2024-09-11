#include <aoc.hpp>

namespace {
    using matrix = Eigen::Array<char, -1, -1, Eigen::RowMajor>;

    auto variations(auto const& m) {
        std::array<matrix, 8> v = {{
            m,                                 // original
            m.rowwise().reverse(),             // flip horizontal
            m.colwise().reverse(),             // flip vertical
            m.transpose(),                     // transpose
            m.reverse(),                       // reverse
            m.transpose().rowwise().reverse(), // transpose flip horizontal
            m.transpose().colwise().reverse(), // transpose flip vertical
            m.transpose().reverse()            // transpose reverse
        }};
        return v;
    }

    auto parse(auto const& input) {
        aoc::dense::map<u64, matrix> rules;
        std::vector<char> buf;

        auto parse_rule = [&](auto const& s) {
            buf.clear();
            i64 k{0};
            for (auto c : s) {
                if (c == '/') { ++k; continue; }
                buf.push_back(c);
            }
            return k;
        };

        i64 nrows{};
        i64 ncols{};
        i64 k{};
        for (auto const& s : input) {
            auto [s0, s1] = scn::scan<std::string, std::string>(s, "{} => {}")->values();
            // parse source rule
            k = parse_rule(s0);
            nrows = k+1;
            ncols = std::ssize(buf) / nrows;
            matrix src = Eigen::Map<matrix>(buf.data(), nrows, ncols);
            // parse destination rule
            k = parse_rule(s1);
            nrows = k+1;
            ncols = std::ssize(buf) / nrows;
            matrix dst = Eigen::Map<matrix>(buf.data(), nrows, ncols);
            for(auto&& m : variations(src)) {
                rules.insert({ aoc::util::hash{}(m.reshaped()), dst });
            }
        }
        return rules;
    }
} // namespace

template<>
auto advent2017::day21() -> result {
    auto input = aoc::util::readlines("./source/2017/21/input.txt");
    auto rules = parse(input);
    std::string initial{".#...####"};
    matrix initial_pattern = Eigen::Map<matrix>(initial.data(), 3, 3);

    auto count_on = [&](auto const initial_pattern, auto iterations) {
        matrix m = initial_pattern;
        matrix n;
        for (auto i = 0; i < iterations; ++i) {
            auto r = m.rows() % 2 == 0 ? (m.rows() / 2) : (m.rows() / 3);
            auto s = m.rows() % 2 == 0 ? 2 : 3;
            n.resize((s+1)*r, (s+1)*r);
            for (auto i = 0, x = 0; i < m.rows(); i += s, x += s+1) {
                for (auto j = 0, y = 0; j < m.cols(); j += s, y += s+1) {
                    auto h = aoc::util::hash{}(m.block(i, j, s, s).reshaped());
                    if (auto it = rules.find(h); it != rules.end()) {
                        auto const& q = it->second;
                        n.block(x, y, q.rows(), q.cols()) = q;
                    }
                }
            }
            std::swap(m, n);
        }
        return (m == '#').count();
    };
    auto const p1 = count_on(initial_pattern, 5);
    auto const p2 = count_on(initial_pattern, 18);
    return aoc::result(p1, p2);
}