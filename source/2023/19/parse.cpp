template<typename T = i64>
auto parse(auto const& input) {
    using workflow_t = workflow<T>;
    using rule_t     = rule<T>;
    using part_t     = part<T>;

    aoc::util::hash hash;
    std::vector<std::unique_ptr<workflow_t>> ws;
    aoc::dense::map<u64, u64> wi;

    auto get_workflow = [&](std::string_view v) -> workflow_t* {
        auto h = hash(v);
        auto [it, ok] = wi.insert({h, ws.size()});
        if (ok) {
            ws.push_back(std::move(std::make_unique<workflow_t>(h)));
        }
        return ws[it->second].get();
    };

    std::vector<part_t> parts;

    bool rules_done{false};
    for (auto const& s : input) {
        if (!rules_done) { rules_done = s.empty(); }
        if (!rules_done) {
            auto i = s.find('{');
            auto j = s.find('}');

            std::string_view workflow_name{ s.begin(), s.begin()+i };
            auto* w = get_workflow(workflow_name);

            std::string_view rule_part{s.begin()+i+1, s.begin()+j};

            for (auto const r : rule_part | std::views::split(',')) {
                rule_t rl{};
                if (r[1] == '>' || r[1] == '<') {
                    rl.op = r[1];
                    rl.field = r[0];
                    auto k = 0; while(r[k] != ':') { ++k; }
                    (void)scn::scan(std::string_view{r.begin()+2, r.begin()+k}, "{}", rl.value);
                    rl.work = get_workflow(std::string_view{r.begin()+k+1, r.end()});
                } else {
                    rl.op = '!';
                    rl.work = get_workflow(std::string{r.begin(), r.end()});
                }
                w->rules.push_back(rl);
            }
        } else {
            if constexpr (std::is_same_v<T, i64>) {
                if (s.empty()) { continue; }
                part_t p{};
                std::array<i64, 4> v{};
                (void)scn::scan(s, "{{x={},m={},a={},s={}}}", p[0], p[1], p[2], p[3]);
                parts.push_back(p);
            }
        }
    }
    return std::tuple{std::move(ws), std::move(parts)};
}