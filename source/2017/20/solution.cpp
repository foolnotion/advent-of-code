#include <aoc.hpp>

namespace {
    using point = Eigen::Array<i64, 3, 1>;

    struct particle {
        i32 id;
        point p;
        point v;
        point a;

        auto tick() {
            v += a;
            p += v;
        }

        auto operator<(particle const& other) const {
            return std::ranges::lexicographical_compare(p, other.p);
        }

        auto operator==(particle const& other) const {
            return (p == other.p).all();
        }
    };
} // namespace

template<>
auto advent2017::day20() -> result {
    auto input = aoc::util::readlines("./source/2017/20/input.txt");
    std::vector<particle> particles;
    particles.reserve(input.size());
    for (auto i = 0; i < std::ssize(input); ++i) {
        particle t{i};
        (void)scn::scan(input[i], "p=<{},{},{}>, v=<{},{},{}>, a=<{},{},{}>", t.p(0), t.p(1), t.p(2), t.v(0), t.v(1), t.v(2), t.a(0), t.a(1), t.a(2));
        particles.push_back(t);
    }

    // physics tells us that in the long term, particles with non-zero acceleration
    // will increase their velocity to infinity and move infinitely far from the origin
    // therefore the closest particle will be the one with zero acceleration
    auto p1 = std::ranges::min_element(particles, std::less{}, [](auto const& p){ return p.a.matrix().template lpNorm<2>(); })->id;

    auto compute_collisions = [](auto& particles) {
        cppsort::merge_sorter{}(particles);
        for (auto i = 0; i < std::ssize(particles)-1; ++i) {
            auto const& p = particles[i];
            auto j = i+1;
            while (p == particles[j]) {
                particles[j++].id = -1;
            }
            if (i+1 < j) { particles[i].id = -1; }
        }
        std::erase_if(particles, [](auto const& p) { return p.id < 0; });
    };

    auto const n = std::ssize(particles);
    auto p2 = n+1;
    auto const h = 10; // time window to detect convergence (good enough here)
    for (auto c = 0; c < h; ++c) {
        std::ranges::for_each(particles, &particle::tick);
        compute_collisions(particles);
        auto const s = std::ssize(particles);
        if (p2 > s) {
            p2 = s;
            c  = 0;
        }
    }
    return aoc::result(p1, p2);
}