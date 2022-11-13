#include <aoc.hpp>
#include <unsupported/Eigen/CXX11/Tensor>

namespace detail {
template<std::size_t N>
auto iterate(auto a, auto b) {
    return [&]<auto... Idx>(std::index_sequence<Idx...>) {
        auto range = [&](auto) { return lz::range(a, b); };
        return lz::cartesian(range(Idx)...);
    }(std::make_index_sequence<N>{});
};

// unpack a tuple into an array (all elements must have the same type)
constexpr auto unpack(auto const& tup) {
    constexpr auto sz = std::tuple_size_v<std::remove_cvref_t<decltype(tup)>>;
    return [&]<auto... Idx>(std::index_sequence<Idx...>) {
        return std::array { std::get<Idx>(tup)... };
    }(std::make_index_sequence<sz>{});
};

// create a tuple containing value v repeated N times
template<std::size_t N>
constexpr auto repeat(auto const& v) {
    auto f = [&](auto) { return v; };
    return [&]<auto... Idx>(std::index_sequence<Idx...>) {
        return std::tuple{f(Idx)...};
    }(std::make_index_sequence<N>{});
};
} // namespace detail

template<typename T, std::size_t N = 3>
requires std::is_integral_v<T>
struct pocket_universe {
    Eigen::Tensor<T, N> tensor;

    [[nodiscard]] auto sum() const {
        Eigen::Tensor<u32, 0> res = tensor.template cast<u32>().sum();
        return res(0);
    }

    [[nodiscard]] auto sum(auto offsets, auto extents) const {
        Eigen::Tensor<u32, 0> res = tensor.slice(offsets, extents).template cast<u32>().sum();
        return res(0);
    }

    auto simulate(auto const& input, int steps) {
        auto const nrow = std::ssize(input);
        auto const ncol = std::ssize(input.front());

        auto const radius = 2 * (steps+1) + nrow;
        auto const mid = radius / 2;
        std::apply([&](auto... a){ tensor.resize(a...); }, detail::repeat<N>(radius));
        tensor.setConstant(0);

        for (auto&& [i, s] : lz::enumerate(input)) {
            for (auto [j, c] : lz::enumerate(s)) {
                auto state = (c == '#');
                auto x = i+mid-nrow/2;
                auto y = j+mid-ncol/2;
                std::apply([&](auto... a) { tensor(a...) = state; },
                        std::tuple_cat(detail::repeat<N-2>(mid), std::tuple{x,y}));
            }
        }

        auto off{mid-nrow/2-1}; // offset
        auto ext{nrow+2};       // extent
        decltype(tensor) aux = tensor;
        for (auto s = 0; s < steps; ++s, off -= 1, ext += 2) {
            for (auto const& tup : detail::iterate<N>(off, off+ext)) {
                auto n = neighbours(tup);
                std::apply([&](auto... args) {
                    aux(args...) = tensor(args...) ? (n == 3 || n == 4) : (n == 3);
                }, tup);
            }
            std::swap(aux, tensor);
        }
        return sum();
    };

    private:
    auto neighbours(auto const& t) {
        auto off = std::apply([](auto... a) { return std::array{(a-1)...}; }, t);
        auto ext = detail::unpack(detail::repeat<N>(3));
        return sum(off, ext);
    };
};

template<>
auto advent2020::day17() -> void {
    auto input = aoc::util::readlines("./source/2020/17/input.txt");
    constexpr auto steps{6};
    fmt::print("part 1: {}\n", pocket_universe<u8, 3>{}.simulate(input, steps));
    fmt::print("part 2: {}\n", pocket_universe<u8, 4>{}.simulate(input, steps));
}
