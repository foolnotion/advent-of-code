#include <aoc.hpp>
#include <flux.hpp>

namespace {
template <typename T, int S = 10> // NOLINT
struct tile {
    enum { dimension = S };
    using matrix = Eigen::Matrix<T, S, S>;
    u64 id;
    matrix m;

    friend auto operator==(tile const& a, tile const& b) -> bool { return a.id == b.id; }
    friend auto operator!=(tile const& a, tile const& b) -> bool { return a.id != b.id; }

    [[nodiscard]] auto dihedral_group() const -> std::vector<tile>
    {
        auto tmp = *this;
        return {
            tmp,
            tmp.transpose().flip_horizontal(),
            tmp.flip_horizontal().flip_vertical(),
            tmp.transpose().flip_vertical(),
            tmp.flip_vertical(),
            tmp.transpose(),
            tmp.flip_horizontal(),
            tmp.flip_horizontal().flip_vertical().transpose()
        };
    }

    [[nodiscard]] auto left() const { return m.col(0); }
    [[nodiscard]] auto right() const { return m.col(m.cols() - 1); }
    [[nodiscard]] auto top() const  { return m.row(0); }
    [[nodiscard]] auto bottom() const { return m.row(m.rows() - 1); }

    [[nodiscard]] auto match_right(tile const& other) const -> bool { return right() == other.left(); }
    [[nodiscard]] auto match_left(tile const& other) const -> bool { return left() == other.right(); }
    [[nodiscard]] auto match_top(tile const& other) const -> bool { return top() == other.bottom(); }
    [[nodiscard]] auto match_bottom(tile const& other) const -> bool { return bottom() == other.top(); }

    [[nodiscard]] auto match_horizontal(tile const& other) const -> bool {
        return match_left(other) || match_right(other);
    }

    [[nodiscard]] auto match_vertical(tile const& other) const -> bool {
        return match_top(other) || match_bottom(other);
    }

    [[nodiscard]] auto match_any_side(tile const& other) const -> bool {
        return match_horizontal(other) || match_vertical(other);
    }

    auto flip(bool axis) {
        Eigen::PermutationMatrix<-1, -1> p(m.cols());
        auto& idx = p.indices();
        std::iota(idx.begin(), idx.end(), 0);
        std::ranges::reverse(idx);
        if (axis) { m = p * m; } else { m = m * p; }
    }

    auto flip_horizontal_in_place() { flip(0); }
    auto flip_vertical_in_place() { flip(1); }
    void transpose_in_place() { m.transposeInPlace(); }

    [[nodiscard]] auto flip_horizontal() const -> tile
    {
        auto tmp = *this;
        tmp.flip_horizontal_in_place();
        return tmp;
    }

    [[nodiscard]] auto flip_vertical() const -> tile
    {
        auto tmp = *this;
        tmp.flip_vertical_in_place();
        return tmp;
    }

    [[nodiscard]] auto transpose() const -> tile
    {
        auto tmp = *this;
        tmp.transpose_in_place();
        return tmp;
    }
};
} // namespace

using tile_t = tile<char, 10>; // NOLINT

namespace day20 {
auto parse(std::vector<std::string> const& input) {
    std::vector<tile_t> tiles;
    for (auto i = 0; i < input.size(); i += tile_t::dimension + 2) {
        auto id{0U};
        tile_t::matrix m;
        (void)scn::scan(input[i], "Tile {}:", id);
        for (auto j = 0; j < tile_t::dimension; ++j) {
            auto const& line = input[i+j+1];
            m.row(j) = Eigen::Map<Eigen::Array<char, -1, 1> const>(line.data(), std::ssize(line));
        }
        tiles.push_back({id, m});
    }
    return tiles;
}
} // namespace day20

auto assemble_image(std::vector<tile_t> const& all, i64 dim) {
    aoc::dense::set<u64> visited;
    Eigen::Matrix<u64, -1, -1> image(dim, dim);
    auto check = [&](i64 count, auto&& check) -> bool {
        if (count == dim * dim) {
            return true;
        }
        auto [row, col] = std::div(count, dim);
        for (u64 i = 0; i < all.size(); ++i) {
            auto const& u = all[i];
            if (visited.contains(u.id)) { continue; }
            if (row > 0 && !u.match_left(all[image(row - 1, col)])) {
                continue;
            }
            if (col > 0 && !u.match_top(all[image(row, col - 1)])) {
                continue;
            }
            visited.insert(u.id);
            image(row, col) = i;
            auto res = check(count + 1, check);
            visited.erase(u.id);
            if (res) { return true; }
        }
        return false;
    };
    bool found = false;
    for (u64 i = 0; i < all.size(); ++i) {
        image(0,0) = i;
        auto const& u = all[i];
        visited.insert(u.id);
        auto res = check(1, check);
        visited.erase(u.id);
        if (res) {
            found = true;
            break;
        }
    }
    if (!found) { throw std::logic_error("unable to find arrangement."); }
    return image;
}

template<>
auto advent2020::day20() -> result {
    auto input = aoc::util::readlines("./source/2020/20/input.txt");
    auto tiles = day20::parse(input);

    // part 1
    auto dim = static_cast<i64>(std::sqrt(tiles.size()));
    std::vector<tile_t> all; all.reserve(tiles.size() * 8);
    for (auto&& g: tiles | std::views::transform(&tile_t::dihedral_group)) {
        for (auto&& t : g) {
            all.push_back(std::move(t));
        }
    }
    auto im = assemble_image(all, dim);
    auto p1 = all[im(0,0)].id * all[im(0,dim-1)].id * all[im(dim-1,0)].id * all[im(dim-1,dim-1)].id;

    auto const d = tile_t::dimension - 2;
    Eigen::Array<char, -1, -1> stitched(dim * d, dim * d);

    // assemble the tiles into the final image
    auto const prod = lz::cartesian(lz::range(dim), lz::range(dim));
    std::ranges::for_each(prod, [&](auto t) {
        auto [i, j] = t;
        stitched.block(i * d, j * d, d, d) = all[im(i, j)].m.block(1, 1, d, d).transpose();
    });
    stitched = (stitched == '#').select(stitched, ' ');
    tile<char, -1> img{0, stitched};

    std::string s0{"                  # "};
    std::string s1{"#    ##    ##    ###"};
    std::string s2{" #  #  #  #  #  #   "};
    Eigen::Matrix<char, 3, 20> mon;
    mon.row(0) = Eigen::Map<Eigen::Array<char, -1, 1>>(s0.data(), std::ssize(s0));
    mon.row(1) = Eigen::Map<Eigen::Array<char, -1, 1>>(s1.data(), std::ssize(s1));
    mon.row(2) = Eigen::Map<Eigen::Array<char, -1, 1>>(s2.data(), std::ssize(s2));

    auto const mon_scales{(mon.array() == '#').count()};
    auto const x{(img.m.array() == '#').count()};

    auto match_monster = [&](auto const& mat) {
        auto const prod = lz::cartesian(lz::range(mon.rows()), lz::range(mon.cols()));
        return std::ranges::none_of(prod, [&](auto t) {
            auto [i, j] = t;
            return mon(i, j) == '#' && mat(i, j) != mon(i, j);
        });
    };

    auto p2{0};
    for (auto&& g : img.dihedral_group()) {
        auto n = x;
        auto const prod = lz::cartesian(lz::range(g.m.rows()-mon.rows()), lz::range(g.m.cols()-mon.cols()));
        for (auto [i, j] : prod) {
            if (match_monster(g.m.block(i, j, mon.rows(), mon.cols()))) {
                n -= mon_scales;
                j += mon.cols();
            }
        }
        if (n < x) {
            p2 = n;
            break;
        }
    }
    return aoc::result(p1, p2);
}
