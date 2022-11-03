#include <aoc.hpp>

struct item {
    i32 cost;
    i32 damage;
    i32 armor;

    friend auto operator==(item a, item b) {
        return std::tie(a.cost, a.damage, a.armor) == std::tie(b.cost, b.damage, b.armor);
    }
};

struct hero {
    i32 health;
    item weapon;
    item armor;
    item leftring;
    item rightring;

    [[nodiscard]] auto cost() const { return weapon.cost + armor.cost + leftring.cost + rightring.cost; }
    [[nodiscard]] auto damage() const { return weapon.damage + armor.damage + leftring.damage + rightring.damage; }
    [[nodiscard]] auto armour() const { return weapon.armor + armor.armor + leftring.armor + rightring.armor; }
};

struct boss {
    i32 health;
    i32 damage;
    i32 armor;
};

auto battle(hero h, boss b) {
    while(h.health > 0 && b.health > 0) {
        b.health -= std::max(1, h.damage() - b.armor);
        if (b.health > 0) {
            h.health -= std::max(1, b.damage - h.armour());
        }
    }
    return h.health > 0;
}

template<>
auto advent2015::day21() -> void {
    constexpr std::array weapons {
        item{ 8, 4, 0}, // NOLINT
        item{10, 5, 0}, // NOLINT
        item{25, 6, 0}, // NOLINT
        item{40, 7, 0}, // NOLINT
        item{74, 8, 0}  // NOLINT
    };

    constexpr std::array armors {
        item{  0, 0, 0}, // NOLINT - no armor
        item{ 13, 0, 1}, // NOLINT
        item{ 31, 0, 2}, // NOLINT
        item{ 53, 0, 3}, // NOLINT
        item{ 75, 0, 4}, // NOLINT
        item{102, 0, 5}  // NOLINT
    };

    constexpr std::array rings {
        item{  0, 0, 0}, // NOLINT - no ring
        item{ 25, 1, 0}, // NOLINT
        item{ 50, 2, 0}, // NOLINT
        item{100, 3, 0}, // NOLINT
        item{ 20, 0, 1}, // NOLINT
        item{ 40, 0, 2}, // NOLINT
        item{ 80, 0, 3}  // NOLINT
    };

    constexpr boss bigboss{109, 8, 2};
    constexpr i32 hp{100};

    auto mincost{std::numeric_limits<i32>::max()};
    auto maxcost{std::numeric_limits<i32>::min()};
    for (auto&& [we, ar, lr, rr] : lz::cartesian(weapons, armors, rings, rings)) {
        if (lr == rr) { continue; }
        hero h{hp, we, ar, lr, rr};
        if (battle(h, bigboss)) {
            mincost = std::min(mincost, h.cost());
        } else {
            maxcost = std::max(maxcost, h.cost());
        }
    }
    fmt::print("part 1: {}\n", mincost);
    fmt::print("part 2: {}\n", maxcost);
}
