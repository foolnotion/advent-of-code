#include <aoc.hpp>
#include <Eigen/Dense>
#include <z3++.h>

using vec3 = Eigen::Array<i64, 3, 1>;

namespace {
    struct hailstone {
        vec3 position;
        vec3 velocity;

        [[nodiscard]] auto line() const {
            auto x0 = position(0);
            auto x1 = velocity(0) + x0;

            auto y0 = position(1);
            auto y1 = velocity(1) + y0;

            auto m = static_cast<double>(y1-y0) / (x1-x0);
            auto b = y0-m*x0;
            return std::pair{m, b};
        }

        [[nodiscard]] auto crossed_in_the_future(Eigen::Vector2d v) const {
            return std::signbit(v(0) - position(0)) == std::signbit(velocity(0)) &&
                   std::signbit(v(1) - position(1)) == std::signbit(velocity(1));
        }


        [[nodiscard]] auto cross(hailstone const& h) const {
            auto [m1, b1] = line();
            auto [m2, b2] = h.line();

            Eigen::Matrix2d m;
            m << -m1, 1, -m2, 1;
            Eigen::Vector2d b{b1, b2};

            Eigen::Vector2d r = m.lu().solve(b).array();
            return r;
        }
    };

    struct limits {
        static constexpr auto min{200000000000000L};
        static constexpr auto max{400000000000000L};
    };

    auto parse(auto const& input) {
        std::vector<hailstone> hs;
        for (auto const& s : input) {
            vec3 p;
            vec3 v;
            (void)scn::scan(s, "{}, {}, {} @ {}, {}, {}", p(0), p(1), p(2), v(0), v(1), v(2));
            hs.push_back({p, v});
        }
        return hs;
    }
} // namespace

template<>
auto advent2023::day24() -> result {
    auto input = aoc::util::readlines("./source/2023/24/input.txt");
    auto hs = parse(input);

    auto count{0L};
    for (auto i = 0; i < std::ssize(hs)-1; ++i) {
        auto const& a = hs[i];
        for (auto j = i+1; j < std::ssize(hs); ++j) {
            auto const& b = hs[j];
            auto c = a.cross(b);
            auto inside = (c.array() >= limits::min).all() && (c.array() <= limits::max).all();
            count += inside && a.crossed_in_the_future(c) && b.crossed_in_the_future(c);
        }
    }
    auto const p1 = count;

    // part 2 - use a SAT solver
    z3::context ctx;
    z3::expr x = ctx.int_const("x");
    z3::expr y = ctx.int_const("y");
    z3::expr z = ctx.int_const("z");
    z3::expr dx = ctx.int_const("dx");
    z3::expr dy = ctx.int_const("dy");
    z3::expr dz = ctx.int_const("dz");

    z3::solver solver(ctx);
    for (auto i = 0; i < hs.size(); ++i) {
        std::string name = fmt::format("t{}", i);
        auto const& h = hs[i];
        z3::expr t = ctx.int_const(name.c_str());
        solver.add(t >= 0);
        solver.add(ctx.int_val(h.position(0)) + t * ctx.int_val(h.velocity(0)) == x + t * dx);
        solver.add(ctx.int_val(h.position(1)) + t * ctx.int_val(h.velocity(1)) == y + t * dy);
        solver.add(ctx.int_val(h.position(2)) + t * ctx.int_val(h.velocity(2)) == z + t * dz);
    }
    ENSURE(solver.check() == z3::sat);
    auto const p2 = solver.get_model().eval(x + y + z).as_int64();

    return aoc::result(p1, p2);
}