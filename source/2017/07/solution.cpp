#include <aoc.hpp>

namespace {
struct program {
    u64 weight;
    u64 wtotal; // total weight (including children)
    std::string name;
    program* parent;
    std::vector<program*> children;

    // update total weight
    auto update() -> void {
        for (auto* c : children) {
            c->update();
            wtotal += c->wtotal;
        }
    }
};
} // namespace

template <>
auto advent2017::day07() -> result {
    std::fstream f("./source/2017/07/input.txt");

    // parse the input
    std::vector<std::string> input;
    for (std::string line; std::getline(f, line);) {
        input.push_back(line);
    }

    std::vector<program> programs;
    ankerl::unordered_dense::map<std::string, u64> map;

    // first, get all the programs
    for (auto const& line : input) {
        auto [name, weight] = scn::scan<std::string, u64>(line, "{} ({})")->values();
        program p{ weight, weight, name };
        map.insert({name, programs.size()});
        programs.push_back(p);
    }

    // next, get all the children
    for (auto i = 0UL; i < input.size(); ++i) {
        auto const& line = input[i];
        if (line.back() == ')') { continue; }
        std::string_view sv{line.begin() + static_cast<int>(line.find('>')) + 2, line.end()};
        for (auto r : lz::split(sv, ", ")) {
            auto idx = map[std::string(r.begin(), r.end())];
            programs[idx].parent = &programs[i];
            programs[i].children.push_back(&programs[idx]);
        }
    }
    auto* root = programs.front().parent;
    while(root->parent) { root = root->parent; }
    auto p1 = root->name;

    root->update();
    program* q{nullptr};
    for (auto& p : programs) {
        if (p.children.empty()) { continue; }
        std::ranges::sort(p.children, std::less{}, &program::wtotal);
        auto a = p.children.front()->wtotal;
        auto b = p.children.back()->wtotal;
        auto c = p.children[1]->wtotal;
        if (a == b) { continue; }
        q = a == c ? p.children.back() : p.children.front();
        q->weight -= std::max(a,b)-c;
        break;
    }
    auto p2 = q->weight;
    return aoc::result(p1, p2);
}
