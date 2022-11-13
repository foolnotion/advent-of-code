#include <aoc.hpp>

namespace detail {
struct food {
    std::vector<u64> ingredients;
    std::vector<u64> allergens;

    [[nodiscard]] auto contains_ingredient(u64 const& ingredient) const -> bool
    {
        return std::binary_search(ingredients.begin(), ingredients.end(), ingredient);
    }

    [[nodiscard]] auto contains_allergen(u64 const& allergen) const -> bool
    {
        return std::binary_search(allergens.begin(), allergens.end(), allergen);
    }

    void remove_ingredient(u64 const& ingredient)
    {
        ingredients.erase(std::remove(ingredients.begin(), ingredients.end(), ingredient), ingredients.end());
    }

    void remove_allergen(u64 const& allergen)
    {
        allergens.erase(std::remove(allergens.begin(), allergens.end(), allergen), allergens.end());
    }
};
} // namespace detail

template<>
auto advent2020::day21() -> void {
    auto input = aoc::util::readlines("./source/2020/21/input.txt");

    using detail::food;

    std::vector<food> recipes;
    aoc::dense::set<u64> ingredients;
    aoc::dense::set<u64> allergens;
    aoc::dense::map<u64, std::string> names;

    aoc::util::hash hash;

    for (auto const& line : input) {
        bool a = false;
        food f;
        auto tokens = lz::split(line, ' ').toVector();
        for (auto&& token : tokens) {
            if (token.find("(contains") != std::string::npos) {
                a = true;
                continue;
            }
            if (a) {
                auto allergen = token.substr(0, token.size() - 1);
                auto h = hash(allergen);
                f.allergens.push_back(h);
                allergens.insert(f.allergens.back());
                names[h] = allergen;
            } else {
                auto h =  hash(token);
                f.ingredients.push_back(h);
                ingredients.insert(f.ingredients.back());
                names[h] = token;
            }
        }
        std::sort(f.ingredients.begin(), f.ingredients.end());
        std::sort(f.allergens.begin(), f.allergens.end());
        recipes.push_back(f);
    }

    auto n = ingredients.size();
    std::vector<std::pair<u64, u64>> list;

    while (ingredients.size() != n - allergens.size()) {
        for (auto const& allergen : allergens) {
            std::vector<u64> v;

            for (auto const& recipe : recipes) {
                if (!recipe.contains_allergen(allergen)) {
                    continue;
                }
                if (v.empty()) {
                    v = recipe.ingredients;
                } else {
                    v.erase(std::remove_if(v.begin(), v.end(), [&](auto const& x) { return !recipe.contains_ingredient(x); }), v.end());
                }
            }

            if (v.size() == 1) {
                auto const& ingredient = v.front();
                ingredients.erase(ingredient);

                list.emplace_back( ingredient, allergen );

                for (auto& recipe : recipes) {
                    recipe.remove_ingredient(ingredient);
                    recipe.remove_allergen(allergen);
                }
            }
        }
    }

    size_t count = 0;
    for (auto const& ingredient : ingredients) {
        count += std::count_if(recipes.begin(), recipes.end(), [&](auto const& recipe) { return recipe.contains_ingredient(ingredient); });
    }
    fmt::print("part 1: {}\n", count);

    std::sort(list.begin(), list.end(), [&](auto const& x, auto const& y) { return names[x.second] < names[y.second]; });
    for (auto const& [ingredient, allergen] : list) {
        fmt::print("{},", names[ingredient]);
    }
}
