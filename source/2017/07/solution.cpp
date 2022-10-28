#include <aoc.hpp>

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

template <>
auto advent2017::day07() const -> void
{
    std::fstream f("./source/2017/07/input.txt");

    // parse the input
    std::vector<std::string> input;
    for (std::string line; std::getline(f, line);) {
        input.push_back(line);
    }

    std::vector<program> programs;
    robin_hood::unordered_map<std::string, u64> map;

    // first, get all the programs
    for (auto const& line : input) {
        std::string name;
        u64 weight = 0;
        (void)scn::scan(line, "{} ({})", name, weight);
        program p { weight, weight, name };
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
    fmt::print("part 1: {}\n", root->name);

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
    fmt::print("part 2: {}\n", q->weight);
}
