#include <aoc.hpp>

using fmt::print;
using std::optional;

struct record {
    optional<int> children;
    optional<int> cats;
    optional<int> samoyeds;
    optional<int> pomeranians;
    optional<int> akitas;
    optional<int> vizslas;
    optional<int> goldfish;
    optional<int> trees;
    optional<int> cars;
    optional<int> perfumes;
};

template <>
auto advent2015::day16() -> void
{
    auto input = aoc::util::readlines("./source/2015/16/input.txt");

    // this is horrible but it's a cheap way to deal with AoC "parsing hell"
    ankerl::unordered_dense::map<std::string_view, std::tuple<std::add_pointer_t<void(record&, int)>, std::add_pointer_t<optional<int>(record const&)>>> tab {
        { "children",    std::make_tuple([](record& r, int v) { r.children = v; },    [](record const& r) { return r.children; })     },
        { "cats",        std::make_tuple([](record& r, int v) { r.cats = v; },        [](record const& r) { return r.cats; })         },
        { "samoyeds",    std::make_tuple([](record& r, int v) { r.samoyeds = v; },    [](record const& r) { return r.samoyeds; })     },
        { "pomeranians", std::make_tuple([](record& r, int v) { r.pomeranians = v; }, [](record const& r) { return r.pomeranians; })  },
        { "akitas",      std::make_tuple([](record& r, int v) { r.akitas = v; },      [](record const& r) { return r.akitas; })       },
        { "vizslas",     std::make_tuple([](record& r, int v) { r.vizslas = v; },     [](record const& r) { return r.vizslas; })      },
        { "goldfish",    std::make_tuple([](record& r, int v) { r.goldfish = v; },    [](record const& r) { return r.goldfish; })     },
        { "trees",       std::make_tuple([](record& r, int v) { r.trees = v; },       [](record const& r) { return r.trees; })        },
        { "cars",        std::make_tuple([](record& r, int v) { r.cars = v; },        [](record const& r) { return r.cars; })         },
        { "perfumes",    std::make_tuple([](record& r, int v) { r.perfumes = v; },    [](record const& r) { return r.perfumes; })     },
    };

    record target { 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 }; // children, cats, etc. NOLINT
    std::array record_names = { "children", "cats", "samoyeds", "pomeranians", "akitas", "vizslas", "goldfish", "trees", "cars", "perfumes" };
    auto check_part1 = [&](record const& a, record const& b) {
        return std::ranges::all_of(record_names, [&](auto const& name) {
                auto const& [_, get] = tab[name];
                return !get(a) || (*get(a) == *get(b));
            });
    };

    auto check_part2 = [&](record const& a, record const& b) {
        return std::ranges::all_of(record_names, [&](auto const& name) {
                auto const& [_, get] = tab[name];
                if (!get(a)) { return true; }
                if (name == "cats" || name == "trees") { return *get(a) > *get(b); }
                if (name == "pomeranians" || name == "goldfish") { return *get(a) < *get(b); }
                return *get(a) == *get(b);
            });
    };

    int sue1{};
    int sue2{};

    for (auto&& [i, s] : lz::enumerate(input)) {
        aoc::util::remove_all(s, ":");
        aoc::util::remove_all(s, ",");
        auto words = lz::split(s, ' ').toVector();
        record rec;
        for (auto j = 2; j < std::ssize(words); j += 2) {
            auto const& [set, _] = tab[words[j]];
            set(rec, scn::scan_value<int>(words[j + 1UL]).value());
        }
        if (check_part1(rec, target)) { sue1 = i+1; }
        if (check_part2(rec, target)) { sue2 = i+1; }
    }
    print("part 1: {}\n", sue1);
    print("part 2: {}\n", sue2);
}
