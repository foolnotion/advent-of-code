#include <aoc.hpp>

namespace rs = std::ranges;

namespace {
    using point = Eigen::Array<i64, 3, 1>;
    using interval = aoc::math::interval<i64>;

    struct node {
        i64 id;
        point p;
        point d;

        auto x() -> i64& { return p(0); }
        [[nodiscard]] auto x() const -> i64 { return p(0); }

        auto y() -> i64& { return p(1); }
        [[nodiscard]] auto y() const -> i64 { return p(1); }

        auto z() -> i64& { return p(2); }
        [[nodiscard]] auto z() const -> i64 { return p(2); }

        [[nodiscard]] auto ix() const { return interval{p(0), p(0)+d(0)-1}; }
        [[nodiscard]] auto iy() const { return interval{p(1), p(1)+d(1)-1}; }
        [[nodiscard]] auto iz() const { return interval{p(2), p(2)+d(2)-1}; }

        [[nodiscard]] auto intersects(node const& n) const {
            return ix().intersects(n.ix()) && iy().intersects(n.iy()) && iz().intersects(n.iz());
        }

        [[nodiscard]] auto can_disintegrate() const {
            return children.empty() || rs::all_of(children, [](auto const* c) { return c->parents.size() > 1; });
        }

        bool disintegrated{false};
        std::vector<node*> children;
        std::vector<node*> parents;
    };

    struct hash {
        auto operator()(auto const& brick) {
            aoc::util::hash h;
            return h( h(brick.a), h(brick.b) );
        }
    };

    auto parse(auto const& input) {
        std::vector<node> bricks;
        bricks.reserve(input.size());

        point pmin = point::Constant(std::numeric_limits<point::Scalar>::max());
        point pmax = point::Constant(0);

        for (auto i{0UL}; i < input.size(); ++i) {
            auto [ax, ay, az, bx, by, bz] = scn::scan<i64, i64, i64, i64, i64, i64>(input[i], "{},{},{}~{},{},{}")->values();
            point a(ax, ay, az);
            point b(bz, by, bz);
            pmin = a.min(pmin);
            pmax = a.max(pmax);
            bricks.emplace_back(i, a, b-a+1);
        }
        return std::tuple{ std::move(bricks), pmin, pmax };
    }
} // namespace

template<>
auto advent2023::day22() -> result {
    auto input = aoc::util::readlines("./source/2023/22/input.txt");
    auto [nodes, pmin, pmax] = parse(input);

    cppsort::merge_sorter sorter;
    sorter(nodes, [](auto const& n) { return n.z(); });

    auto drop = [&](auto& n, auto& nn, bool add = true) {
        auto f = [&](auto const& m) { return !m.disintegrated && m.id != n.id && m.intersects(n); };
        while(n.z() > 0) {
            n.z() -= 1;
            bool collided{false};
            for (auto& m : nn | std::views::filter(f)) {
                collided = true;
                if (add) {
                    m.children.push_back(&n);
                    n.parents.push_back(&m);
                }
            }
            if (collided) {
                n.z() += 1;
                break;
            }
        }
    };
    rs::for_each(nodes, [&](auto& n){ drop(n, nodes); });
    auto const p1 = rs::count_if(nodes, &node::can_disintegrate);

    // part 2
    auto total{0UL};
    sorter(nodes, [](auto const& n) { return n.z(); });

    for (auto& n : nodes) {
        aoc::dense::set<i64> fallen;
        std::vector<node> tmp;
        rs::copy_if(nodes, std::back_inserter(tmp), [&](auto const& m) { return n.id != m.id; });
        for (auto& m : tmp) {
            auto z = m.z();
            drop(m, tmp, false);
            if (m.z() < z) { fallen.insert(m.id); }
        }
        total += fallen.size();
    }

    auto const p2 = total;
    return aoc::result(p1, p2);
}