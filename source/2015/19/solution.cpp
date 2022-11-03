#include <aoc.hpp>

template<>
auto advent2015::day19() -> void {

    auto input = aoc::util::readlines("./source/2015/19/input.txt");

    std::string molecule;
    aoc::dense::map<std::string, std::vector<std::string>> map;

    auto maxlen{0L};
    for (auto&& [i, s] : lz::enumerate(input)) {
        if (s.empty()) { continue; }
        if (i < std::ssize(input)-1) {
            std::string a;
            std::string b;
            (void)scn::scan(s, "{} => {}", a, b);
            if (auto [it, ok] = map.insert({a, {b}}); !ok) {
                it->second.push_back(b);
            }
            maxlen = std::max({maxlen, std::ssize(a), std::ssize(b)});
        } else {
            molecule = s;
        }
    }

    int total{0};
    aoc::dense::set<u64> uniq;
    aoc::util::hash hasher;
    auto const ms{std::ssize(molecule)};
    for (auto len = maxlen; len >= 1; --len) {
        for (auto i = 0L; i <= ms-len; ++i) {
            std::string key{&molecule[i], static_cast<std::size_t>(len)};
            if (auto it = map.find(key); it != map.end()) {
                for (const auto& val : it->second) {
                    auto ex = molecule;
                    ex.replace(i, len, val);
                    uniq.insert(hasher(ex));
                }
            }
        }
    }
    fmt::print("part 1: {}\n", uniq.size());

    // for part two, the rules can actually be worked out by hand, as follows:
    // 1) the productions of this grammar contain a number of symbols, some of which never appear on the left hand side
    //    Rn, Ar, Y, and C
    // 2) the symbols Rn, Ar, Y always appear in the same order: first Rn, then optionally Y in the middle, then Ar
    // 3) the symbol Y always shows between other symbols, so it could be seen as a comma
    // 4) overall, Rn, Y, Ar have the meaning of (,)
    // 5) taking the productions in reverse for part 2, we realize that:
    //    - every standard rule reduces the length of the expression by 1
    //    - every pair of  () reduces the length of the expression by 2
    //    - every additional comma also reduces the length of the expression by 2
    // 6) therefore, the total length reduction will be: #total = #symbols - #Rn - #Ar - 2 * #Y
    // 7) since we want to get to a length of 1 (the expression "e"), we will get there in #total - 1
    // 8) in practice we just have to calculate #total and subtract #Y
    auto steps{0};
    for (auto i = 0UL; i < molecule.size(); ++i) {
        if (molecule[i] == 'R' || (molecule[i] == 'A' && molecule[i+1] == 'r')) {
            continue; // no need to count them
        }
        if (molecule[i] == 'Y') { steps -= 1; continue; }
        if (map.contains({&molecule[i], 1})) { steps += 1; }
        else if (map.contains({&molecule[i], 2})) { steps += 1; ++i; }
    }
    fmt::print("part 2: {}\n", steps);

    // for completeness, here's a version that does a search
    aoc::dense::map<std::string, std::string> rmap;
    std::vector<std::string> keys;
    for (auto const& [k, v] : map) {
        for (auto const& u : v) {
            rmap[u] = k;
            keys.push_back(u);
        }
    }
    std::ranges::sort(keys, std::greater{}, &std::string::size);

    auto minsteps{std::numeric_limits<int>::max()};
    aoc::dense::map<u64, int> seen;
    bool found{false};
    auto search = [&](auto mol, auto depth, auto&& search) {
        if (found) { return; }
        if (mol == "e") {
            found = true;
            minsteps = std::min(minsteps, depth);
            return;
        }
        if (depth+1 >= minsteps) { return; }
        if (auto [it, ok] = seen.insert({hasher(mol), depth}); !ok) {
            return;
        }
        for (auto const& k : keys) {
            if (auto i = mol.find(k); i != std::string::npos) {
                auto tmp = mol;
                tmp.replace(i, k.size(), rmap[k]);
                search(std::move(tmp), depth+1, search);
            }
        }
    };
    search(molecule, 0, search);
    fmt::print("part 2: {}\n", minsteps);
}
