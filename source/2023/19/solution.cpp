#include <aoc.hpp>

namespace rs = std::ranges;

namespace {
    using interval = aoc::math::interval<i64>;

    auto xmas(char c) {
        constexpr std::array<char, 4> x{'x', 'm', 'a', 's'};
        return rs::distance(x.begin(), rs::find(x, c));
    }

    template<typename T = i64>
    auto sum(std::array<T, 4> v) { return std::reduce(v.begin(), v.end()); }

    template<>
    auto sum<interval>(std::array<interval, 4> v) {
        return std::transform_reduce(v.begin(), v.end(), 1L, std::multiplies{}, std::mem_fn(&interval::width));
    }

    template<typename T>
    struct part {
        std::array<T, 4> v;
        auto operator[](i64 i) -> T& { return v[i]; }
        auto operator[](i64 i) const -> T { return v[i]; }
        [[nodiscard]] auto sum() const { return ::sum<T>(v); }
    };

    template<typename T> struct workflow;
    template<typename T> struct rule;

    template<typename T = i64>
    auto rule_impl(rule<T> const& rule, part<T> p) -> std::pair<part<T>, bool>;

    template<typename T = i64>
    auto work_impl(workflow<T> const& work, part<T> p) -> void;

    template<typename T>
    struct rule {
        char op{'<'};       // operation
        char field{' '};    // field name
        i64 value{};        // comparison value
        workflow<T>* work;  // target workflow

        auto operator()(part<T> p) const { return rule_impl<T>(*this, p); }
    };

    template<typename T>
    struct workflow {
        u64 id{};
        std::vector<rule<T>> rules;
        std::deque<part<T>> parts;

        auto operator()(part<T> p) const { return work_impl<T>(*this, p); }
    };

    template<typename T>
    [[nodiscard]] auto rule_impl(rule<T> const& rule, part<T> p) -> std::pair<part<T>, bool> {
        auto [op, field, value] = std::tuple{rule.op, rule.field, rule.value};
        auto ok = (op == '!') || (op == '<' && p[xmas(field)] < value) || (op == '>' && p[xmas(field)] > value);
        if(ok) { rule.work->parts.push_front(p); }
        return {p, ok};
    }

    template<>
    [[nodiscard]] auto rule_impl<interval>(rule<interval> const& rule, part<interval> p) -> std::pair<part<interval>, bool> {
        auto [work, op, field, value] = std::tuple{rule.work, rule.op, rule.field, rule.value};
        if (op == '!') {
            work->parts.push_front(p);
            return {p, true};
        }
        auto i = xmas(field);
        auto iv = p[i];
        if (!iv.contains(value)) {
            p[i] = interval::empty();
            return {p, false};
        }
        p[i] = op == '<' ? interval{iv.a, value-1} : interval{value+1, iv.b};
        work->parts.push_front(p);
        return {p, true};
    }

    template<typename T>
    auto work_impl(workflow<T> const& work, part<T> p) -> void {
        auto check = [p](auto const& r) { return r(p).second; };
        if (auto it = rs::find_if(work.rules, check); it != work.rules.end()) {
            (*it->work)(p);
        }
    }

    template<>
    auto work_impl<interval>(workflow<interval> const& work, part<interval> p) -> void {
        for (auto const& r : work.rules) {
            auto [uv, ok] = r(p);
            if (!ok) { break; }

            if (r.op != '!') {
                auto i = xmas(r.field);
                auto x = r.op == '<' ? interval{uv[i].b+1, p[i].b} : interval{p[i].a, uv[i].a-1};
                p[i] = x;
            }
            (*r.work)(uv);
        }
    }

    #include "parse.cpp" // NOLINT

    template<typename T>
    auto solve(auto const& input) {
        aoc::util::hash hash;
        auto [workflows, parts] = parse<T>(input);

        auto* in = rs::find_if(workflows, [&](auto const& w){ return w->id == hash("in"); })->get();
        if constexpr (std::is_same_v<T, i64>) {
            rs::for_each(parts, *in);
        } else {
            constexpr interval range{1, 4000};
            (*in)({ range, range, range, range });
        }
        auto* out = rs::find_if(workflows, [&](auto const& w){ return w->id == hash("A"); })->get();
        auto const& acc = out->parts;
        return std::transform_reduce(acc.begin(), acc.end(), 0L, std::plus{}, std::mem_fn(&part<T>::sum));
    }
} // namespace

template<>
auto advent2023::day19() -> result {
    auto input = aoc::util::readlines("./source/2023/19/input.txt");

    auto const p1 = solve<i64>(input);
    auto const p2 = solve<interval>(input);
    return aoc::result(p1, p2);
}