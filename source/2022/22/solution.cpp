#include <aoc.hpp>
#include <complex>
#include <cpp-sort/sorters/merge_sorter.h>

namespace {
    using point  = std::complex<i32>;
    using matrix = Eigen::Matrix<char, -1, -1>;
    using view   = Eigen::Map<matrix>;

    struct orientation {
        static constexpr auto right = point{0, +1};
        static constexpr auto down  = point{+1, 0};
        static constexpr auto left  = point{0, -1};
        static constexpr auto up    = point{-1, 0};
    };

    struct turn {
        static constexpr auto left  = point{0, +1};
        static constexpr auto right = point{0, -1};
    };

    auto unpack(point p) {
        return std::tuple{p.real(), p.imag()};
    }

    // rotated coordinate matrices
    auto get_rotation(i32 dim, point ori) -> Eigen::Matrix<i32, -1, -1> {
        Eigen::Matrix<i32, -1, -1, Eigen::RowMajor> m(dim, dim);
        std::iota(m.data(), m.data()+m.size(), 0);
        if (ori == orientation::up)    { return m; }
        if (ori == orientation::right) { return m.transpose().colwise().reverse(); }
        if (ori == orientation::left)  { return m.transpose().rowwise().reverse(); }
        if (ori == orientation::down)  { return m.reverse(); }
        throw std::runtime_error("unknown orientation");
    }

    struct face {
        view  map;
        point pos;
        i32   dim;

        // used to navigate the cube
        std::pair<face*, point> l{}; // left
        std::pair<face*, point> r{}; // right
        std::pair<face*, point> u{}; // up
        std::pair<face*, point> d{}; // down

        [[nodiscard]] auto contains(point p) const {
            auto [i, j] = unpack(p);
            auto [x, y] = unpack(pos);
            return i >= x && i < x+dim && j >= y && j < y+dim;
        }

        [[nodiscard]] auto valid(point p) const {
            auto [x, y] = unpack(p);
            return x >= 0 && x < map.rows() && y >= 0 && y < map.cols() && map(x, y) != '#' && map(x, y) != ' ';
        }

        [[nodiscard]] auto step(point pos, point step) const -> std::tuple<face const*, point, point> {
            auto [a, b] = unpack(pos);
            auto p = pos + step;
            if (contains(p)) {
                return {map(p.real(), p.imag()) != '#' ? this : nullptr , p, orientation::up};
            }
            auto [x, y] = unpack(p);
            std::pair<face*, point> next;
            if      (x == a) { next = y < b ? l : r; } // moved along the x-axis
            else if (y == b) { next = x < a ? u : d; } // moved along the y-axis
            if (x < 0) { x += dim; }
            if (y < 0) { y += dim; }
            // map coordinates and return new point
            auto [q, o] = next;
            auto v = get_rotation(q->dim, o)(x % dim, y % dim);
            p = {q->pos.real() + v/dim, q->pos.imag() + v%dim};
            return {valid(p) ? next.first : nullptr, p, o};
        }
    };

    using move = std::pair<i32, char>;

    auto parse_path(auto const& path) {
        std::vector<move> moves; moves.reserve(path.size());
        for (auto i = 0; i < path.size();) {
            if (std::isalpha(path[i])) {
                moves.emplace_back(0, path[i++]);
                continue;
            }
            auto j = i;
            while (j < path.size() && std::isdigit(path[j])) {
                ++j;
            }
            std::string_view sv{path.begin()+i, path.begin()+j};
            i32 v{};
            (void)scn::scan(sv, "{}", v);
            moves.emplace_back(v, '.');
            i = j;
        }
        return moves;
    }

    auto prepare_p1(auto& faces_p1) {
        namespace rs = std::ranges;

        auto map = faces_p1.front().map;
        auto dim = faces_p1.front().dim;

        std::vector<face*> sptr;
        sptr.reserve(faces_p1.size());

        auto sortx = [](auto const* a, auto const* b) {
            auto [ar, ai] = unpack(a->pos);
            auto [br, bi] = unpack(b->pos);
            return ar == br ? ai < bi : ar < br;
        };

        auto sorty = [](auto const* a, auto const* b) {
            auto [ar, ai] = unpack(a->pos);
            auto [br, bi] = unpack(b->pos);
            return ai == bi ? ar < br : ai < bi;
        };

        // prepare for part 1
        rs::transform(faces_p1, std::back_inserter(sptr), [](auto& f) { return &f; });
        cppsort::quick_merge_sorter sorter;
        sorter(sptr, sortx);
        for (auto i = 0L; i < map.rows(); i += dim) { // iterate rows
            std::vector<face*> row;
            rs::copy_if(sptr, std::back_inserter(row), [&](auto const* s) { return s->pos.real() == i; });
            for (auto [j, s] : lz::enumerate(row)) {
                auto l = (j-1) >= 0 ? j-1 : row.size()-1;
                s->l = {row[l], orientation::up};
                auto r = (j+1) < row.size() ? j+1 : 0;
                s->r = {row[r], orientation::up};
            }
        }
        sorter(sptr, sorty);
        for (auto i = 0L; i < map.cols(); i += dim) { // iterate rows
            std::vector<face*> row;
            rs::copy_if(sptr, std::back_inserter(row), [&](auto const* s) { return s->pos.imag() == i; });
            for (auto [j, s] : lz::enumerate(row)) {
                auto u = j-1 >= 0 ? j-1 : row.size()-1;
                s->u = {row[u], orientation::up};
                auto d = j+1 < row.size() ? j+1 : 0;
                s->d = {row[d], orientation::up};
            }
        }
    }

    auto prepare_p2(auto& faces_p2) {
        // prepare for part 2 (manually configure the cube net)
        // face 1
        faces_p2[0].r = {&faces_p2[1], orientation::up};
        faces_p2[0].l = {&faces_p2[3], orientation::down};
        faces_p2[0].u = {&faces_p2[5], orientation::right};
        faces_p2[0].d = {&faces_p2[2], orientation::up};
        // face 2
        faces_p2[1].r = {&faces_p2[4], orientation::down};
        faces_p2[1].l = {&faces_p2[0], orientation::up};
        faces_p2[1].u = {&faces_p2[5], orientation::up};
        faces_p2[1].d = {&faces_p2[2], orientation::right};
        // face 3
        faces_p2[2].r = {&faces_p2[1], orientation::left};
        faces_p2[2].l = {&faces_p2[3], orientation::left};
        faces_p2[2].u = {&faces_p2[0], orientation::up};
        faces_p2[2].d = {&faces_p2[4], orientation::up};
        // face 4
        faces_p2[3].r = {&faces_p2[4], orientation::up};
        faces_p2[3].l = {&faces_p2[0], orientation::down};
        faces_p2[3].u = {&faces_p2[2], orientation::right};
        faces_p2[3].d = {&faces_p2[5], orientation::up};
        // face 5
        faces_p2[4].r = {&faces_p2[1], orientation::down};
        faces_p2[4].l = {&faces_p2[3], orientation::up};
        faces_p2[4].u = {&faces_p2[2], orientation::up};
        faces_p2[4].d = {&faces_p2[5], orientation::right};
        // face 6
        faces_p2[5].r = {&faces_p2[4], orientation::left};
        faces_p2[5].l = {&faces_p2[0], orientation::left};
        faces_p2[5].u = {&faces_p2[3], orientation::up};
        faces_p2[5].d = {&faces_p2[1], orientation::up};  
    }
} // namespace

template <>
auto advent2022::day22() -> result
{
    namespace rs = std::ranges;

    auto input = aoc::util::readlines("./source/2022/22/input.txt");
    auto const rows = std::ssize(input)-2; // last 2 lines represent an empty line and the list of instructions
    auto const cols = std::ssize(*std::ranges::max_element(input.rbegin()+2, input.rend(), std::less{}, [](auto const& s) { return s.size(); }));
    auto const dim  = std::max(rows, cols) / 4; // not necessarily correct as there is one 5x2 net, but it will do

    matrix map(rows, cols);
    map.setConstant(' ');
    for (auto i = 0; i < rows; ++i) {
        rs::copy(input[i], map.row(i).begin());
    }
    auto constexpr nsides = 6;
    std::vector<face> faces_p1;
    faces_p1.reserve(nsides);
    for (auto i = 0L; i < map.rows(); i += dim) {
        for (auto j = 0L; j < map.cols(); j += dim) {
            if (map(i, j) != ' ') {
                faces_p1.push_back(face{view{map.data(), rows, cols}, {i, j}, (i32)dim});
            }
        }
    }
    prepare_p1(faces_p1);

    auto faces_p2 = faces_p1;
    prepare_p2(faces_p2);

    auto const path = input.back();
    auto moves = parse_path(path);

    auto hash_point = [](point const& p) {
        return aoc::util::hash{}(p.real(), p.imag());
    };

    auto hash_pair = [](std::pair<point, point> const& p) {
        auto [a, b] = p;
        return aoc::util::hash{}(a.real(), a.imag(), b.real(), b.imag());
    };

    aoc::dense::map<point, i32, decltype(hash_point)> fmap{
        { orientation::left, 2 },
        { orientation::right, 0 },
        { orientation::up, 3 },
        { orientation::down, 1 }  
    };

    aoc::dense::map<std::pair<point, point>, point, decltype(hash_pair)> omap {
        { {orientation::up,    orientation::up},    orientation::up },
        { {orientation::up,    orientation::right}, orientation::right },
        { {orientation::up,    orientation::down},  orientation::down },
        { {orientation::up,    orientation::left},  orientation::left },
        { {orientation::right, orientation::up},    orientation::right },
        { {orientation::right, orientation::right}, orientation::down },
        { {orientation::right, orientation::down},  orientation::left },
        { {orientation::right, orientation::left},  orientation::up },
        { {orientation::down,  orientation::up},    orientation::down },
        { {orientation::down,  orientation::right}, orientation::left },
        { {orientation::down,  orientation::down},  orientation::up },
        { {orientation::down,  orientation::left},  orientation::right },
        { {orientation::left,  orientation::up},    orientation::left },
        { {orientation::left,  orientation::right}, orientation::up },
        { {orientation::left,  orientation::down},  orientation::right },
        { {orientation::left,  orientation::left},  orientation::down }
    };

    auto get_password = [&](face const* side) {
        auto ori = orientation::right;
        auto pos = side->pos;
        for (auto [n, t] : moves) {
            if (t != '.') {
                ori *= t == 'R' ? turn::right : turn::left;
                continue;
            }
            // take steps
            auto step = ori;
            for (auto k = 0; k < n; ++k) {
                auto [s, p, o] = side->step(pos, step);
                if (s == nullptr) { break; }
                side = s;
                pos  = p;
                ori  = omap[{ori, o}];
                step = ori;
            }
        }
        auto [x, y] = unpack(pos);
        return 1000 * (x+1) + 4 * (y+1) + fmap[ori];
    };

    auto part1 = get_password(faces_p1.data());
    auto part2 = get_password(faces_p2.data());
    return aoc::result(part1, part2);
}