#include <aoc.hpp>
#include <iostream>

namespace {
struct ingredient {
    i32 cap{}; // capacity
    i32 dur{}; // durability
    i32 fla{}; // flavor
    i32 tex{}; // texture
    i32 cal{}; // calories

    [[nodiscard]] auto score() const -> u32 {
        if (std::min({ cap, dur, fla, tex }) < 0) { return 0; }
        return cap * dur * fla * tex;
    }

    static constexpr auto from_tuple(std::tuple<std::string, i32, i32, i32, i32, i32> const& t) -> ingredient {
        return [&]<auto... Idx>(std::index_sequence<Idx...>) {
            return ingredient{std::get<Idx+1>(t)...};
        }(std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<decltype(t)>>-1>{});
    };

    friend auto operator+(ingredient a, ingredient b) -> ingredient {
        return {.cap=a.cap + b.cap, .dur=a.dur + b.dur, .fla=a.fla + b.fla, .tex=a.tex + b.tex, .cal=a.cal + b.cal };
    }

    friend auto operator*(ingredient a, i32 q) -> ingredient {
        return { .cap = a.cap * q, .dur = a.dur * q, .fla = a.fla * q, .tex = a.tex * q, .cal = a.cal * q };
    }
};
} // namespace

template <>
auto advent2015::day15() -> result {
    std::vector<std::string> const input {
        "Sprinkles: capacity 5, durability -1, flavor 0, texture 0, calories 5",
        "PeanutButter: capacity -1, durability 3, flavor 0, texture 0, calories 1",
        "Frosting: capacity 0, durability -1, flavor 4, texture 0, calories 6",
        "Sugar: capacity -1, durability 0, flavor 0, texture 2, calories 8",
    };

    auto ingredients = lz::map(input, [](auto const& s) {
        std::string name;
        auto result = scn::scan<std::string, i32, i32, i32, i32 ,i32>(s, "{} capacity {}, durability {}, flavor {}, texture {}, calories {}")->values();
        //name, x.cap, x.dur, x.fla, x.tex, x.cal);
        name = std::get<0>(result);
        return ingredient::from_tuple(result);
    }).toVector();

    std::vector<u32> amounts(ingredients.size(), 0);

    u32 bestscore = std::numeric_limits<u32>::min();
    u32 bestscore_with_calories = std::numeric_limits<u32>::min();

    constexpr int calorie_goal{500};
    constexpr int total{100};

    struct state {
        i32 index;
        i32 running_sum;
        i32 total_teaspoons;
        i32 calorie_goal;
    };

    auto permute = [&](auto& amounts, state state, auto&& permute) {
        auto [i, s, t, g] = state;
        if (s == t) {
            auto cookie = transform_reduce(ingredients.begin(), ingredients.end(), amounts.begin(), ingredient{}, std::plus{}, std::multiplies{});
            auto score = cookie.score();
            if (score > bestscore) {
                bestscore = score;
            }
            if (cookie.cal == g && score > bestscore_with_calories) {
                bestscore_with_calories = score;
            }
            return;
        }
        auto sz = std::ssize(amounts);
        for (auto q = 1; q <= t-s; ++q) {
            amounts[i] = q;
            if (i == sz-2) {
                amounts[i+1UL] = t-s-q;
                permute(amounts, {i+1, t, t, g}, permute);
            } else {
                permute(amounts, {i+1, s+q, t, g}, permute);
            }
        }
    };

    permute(amounts, {.index=0, .running_sum=0, .total_teaspoons=total, .calorie_goal=calorie_goal}, permute);
    return aoc::result(bestscore, bestscore_with_calories);
}
