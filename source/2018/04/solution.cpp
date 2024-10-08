#include <aoc.hpp>
#include <chrono>
#include <fmt/chrono.h>
#include <date/date.h>

namespace {
    enum class event_type : int { shift_begin, asleep, wakeup };
    using duration = std::chrono::duration<i64, std::ratio<60>>;
    using time_point = std::chrono::time_point<date::local_t, duration>;

    struct event {
        i32 id{};
        event_type type{};
        time_point time;

        [[nodiscard]] auto minute() const {
            auto hm = std::chrono::hh_mm_ss{time - std::chrono::floor<std::chrono::days>(time)};
            return hm.minutes().count();
        }
    };

    auto make_time_point(auto t) {
        auto [year, month, day, hour, minute] = t;
        auto dur = std::chrono::duration_cast<std::chrono::days>(date::years{year-1970} + date::months{month-1} + date::days{day-1});
        auto ymd = date::local_days(std::chrono::floor<date::days>(dur));
        auto tp = ymd + std::chrono::hours(hour) + std::chrono::minutes{minute};
        return tp;
    };
} // namespace

template<>
auto advent2018::day04() -> result {
    auto lines = aoc::util::readlines("source/2018/04/input.txt");
    std::vector<event> events;
    constexpr auto minutes{60};
    aoc::dense::map<i32, Eigen::Array<i32, minutes, 1>> map;

    for (auto const& line : lines) {
        auto i = line.find(']');
        std::string_view u{line.begin(), line.begin()+i+1};
        std::string_view v{line.begin()+i+2, line.end()};
        auto t = scn::scan<i32, u32, u32, i32, i32>(u, "[{}-{}-{} {}:{}]")->values();
        i32 id{};
        event_type type{};
        if (v.front() == 'G') {
            type = event_type::shift_begin;
            id   = scn::scan<i32>(v, "Guard #{} begins shift")->value();
            map.insert({id, Eigen::Array<i32, minutes, 1>::Zero() });
        } else {
            type = v.front() == 'f' ? event_type::asleep : event_type::wakeup;
            id   = -1;
        }
        events.push_back(event{id, type, make_time_point(t)});
    }
    std::stable_sort(events.begin(), events.end(), [](auto a, auto b) { return a.time < b.time; });

    auto last_id{0};
    std::deque<i32> awake;
    for (auto it = events.begin(); it != events.end(); ++it) {
        auto& e = *it;
        if (e.type == event_type::shift_begin) {
            awake.push_back(e.id);
        } else if (e.type == event_type::asleep) {
            e.id = awake.back();
            awake.pop_back();
            last_id = e.id;
        } else if (e.type == event_type::wakeup) {
            e.id = last_id;
            awake.push_back(e.id);
            map[it->id].segment((it-1)->minute(), it->minute() - (it-1)->minute()) += 1;
        }
    }

    auto find_guard_and_minute = [&](auto&& f) {
        auto it = std::ranges::max_element(map.values(), std::less{}, f);
        return it->first * std::ranges::distance(it->second.begin(), std::ranges::max_element(it->second));
    };
    auto sum = [](auto const& t){ return std::reduce(t.second.begin(), t.second.end()); };
    auto max = [](auto const& t){ return std::ranges::max(t.second); };

    auto const p1 = find_guard_and_minute(sum);
    auto const p2 = find_guard_and_minute(max);
    return aoc::result(p1, p2);
}