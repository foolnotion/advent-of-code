#include <aoc.hpp>

namespace rg = std::ranges;

// some aliases to make it more clear what's going on
using COST     = i32; // NOLINT
using HEALTH   = i32; // NOLINT
using DAMAGE   = i32; // NOLINT
using MANA     = i32; // NOLINT
using ARMOR    = i32; // NOLINT
using DURATION = i32; // NOLINT

struct spell {
    enum type { missile, drain, shield, poison, recharge };
    static constexpr std::array name { "magic missile", "drain", "shield", "poison", "recharge" };
    type typ;
    i32 cost;
    i32 heal;
    i32 damage;
    i32 mana;
    i32 armor;
    i32 duration;
};

struct boss {
    i32 health;
    i32 damage;

    [[nodiscard]] auto defeated() const { return !(health > 0); }
};

struct hero {
    static constexpr std::array grimoire {
        spell{ spell::type::recharge, COST{229}, HEALTH{0}, DAMAGE{0}, MANA{101}, ARMOR{0}, DURATION{5} }, // NOLINT
        spell{ spell::type::poison,   COST{173}, HEALTH{0}, DAMAGE{3}, MANA{0},   ARMOR{0}, DURATION{6} }, // NOLINT
        spell{ spell::type::shield,   COST{113}, HEALTH{0}, DAMAGE{0}, MANA{0},   ARMOR{7}, DURATION{6} }, // NOLINT
        spell{ spell::type::drain,    COST{73},  HEALTH{2}, DAMAGE{2}, MANA{0},   ARMOR{0}, DURATION{1} }, // NOLINT
        spell{ spell::type::missile,  COST{53},  HEALTH{0}, DAMAGE{4}, MANA{0},   ARMOR{0}, DURATION{1} }, // NOLINT
    };

    i32 health;
    i32 mana;
    i32 armor;

    std::vector<spell> active_spells;

    auto cast(spell s) {
        mana -= s.cost;
        active_spells.push_back(s);
    }

    // undo the last spell that was cast
    auto uncast() {
        mana += active_spells.back().cost;
        active_spells.erase(active_spells.end()-1, active_spells.end());
    }

    [[nodiscard]] auto defeated() const { return !(health > 0); }
};

struct game {
    hero const game_hero{};
    boss const game_boss{};

    auto apply_effects(hero& h, boss& b) const {
        auto& sp = h.active_spells;
        if (h.active_spells.empty()) { return; }
        // decrease duration
        for (auto& s : h.active_spells) { --s.duration; }
        // apply effects
        h.armor = 0; // armor does not stack
        for (auto s : sp) {
            h.health += s.heal;
            h.mana += s.mana;
            h.armor += s.armor;
            b.health -= s.damage;
        }
        // clear expired effects
        sp.erase(std::remove_if(sp.begin(), sp.end(), [](auto e) { return e.duration == 0; }), sp.end());
    };

    auto fight(hero h, boss b, bool turn, i32 spent, i32& mincost, bool part2) const {
        if (h.defeated()) { return; }
        apply_effects(h, b); // boss can be defeated by an existing effect
        if (b.defeated()) {
            mincost = std::min(spent, mincost);
            return;
        }

        if (turn) {
            h.health -= part2;
            if (h.defeated()) { return; }
            for (auto s : hero::grimoire) {
                if (s.cost > h.mana || s.cost + spent >= mincost) { continue; }
                if (rg::any_of(h.active_spells, [&](auto x) { return x.typ == s.typ; })) { continue; }
                h.cast(s);
                fight(h, b, !turn, spent + s.cost, mincost, part2);
                h.uncast();
            }
        } else {
            auto d = std::max(1, b.damage - h.armor);
            h.health -= d;
            fight(std::move(h), b, !turn, spent, mincost, part2);
        }
    };

    auto fight(bool part2) const {
        auto mincost{std::numeric_limits<i32>::max()};
        fight(game_hero, game_boss, true, 0, mincost, part2);
        return mincost;
    }
};

template<>
auto advent2015::day22() -> void {
    hero goodhero{HEALTH{50}, MANA{500}, ARMOR{0}}; // NOLINT
    boss evilboss{HEALTH{71}, DAMAGE{10}}; // NOLINT
    game game{goodhero, evilboss};

    fmt::print("part 1: {}\n", game.fight(/*part2=*/false));
    fmt::print("part 2: {}\n", game.fight(/*part2=*/true));
}
