#ifndef AOC_UTIL_PRIORITY_QUEUE_HPP
#define AOC_UTIL_PRIORITY_QUEUE_HPP

#include <fmt/base.h>
#include <vector>
#include <ranges>

namespace aoc {
template<typename T, typename Compare = std::ranges::less, typename Container = std::vector<T>>
requires std::three_way_comparable<T>
class priority_queue {
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;

    Container vec_;
    Compare comp_;

public:
    priority_queue() = default;
    explicit priority_queue(std::vector<T> vec) : vec_{std::move(vec)}, comp_{Compare{}} {
        std::ranges::make_heap(vec_, comp_);
    }

    [[nodiscard]] auto size() const -> typename Container::size_type { return vec_.size(); }
    [[nodiscard]] auto empty() const -> bool { return vec_.empty(); }

    [[nodiscard]] auto values() const -> Container const& { return vec_; }
    auto values() -> Container& { return vec_; }

    [[nodiscard]] auto top() const -> std::conditional_t<std::is_trivial_v<T>, value_type, const_reference> {
        return vec_.front();
    }

    auto heapify() -> void { std::ranges::make_heap(vec_, comp_); }

    auto pop() -> void {
        std::ranges::pop_heap(vec_, comp_);
        vec_.pop_back();
    }

    auto push(std::conditional_t<std::is_trivial_v<T>, value_type, const_reference> elem) {
        vec_.push_back(elem);
        std::ranges::push_heap(vec_, comp_);
    }

    template<typename... Args>
    auto emplace(Args&&... args) {
        vec_.emplace_back(std::forward<Args>(args)...);
        std::ranges::push_heap(vec_, comp_);
    }
};

}  // namespace aoc

// formatters
template<typename T, typename Compare, typename Container>
struct fmt::formatter<aoc::priority_queue<T, Compare, Container>> {
    template<typename Context>
    constexpr auto parse(Context& ctx) { return ctx.begin(); }

    template<typename Context>
    auto format(aoc::priority_queue<T, Compare, Container> const& q, Context& ctx) const {
        return fmt::format_to(ctx.out(), "{}", q.values());
    }
};

#endif