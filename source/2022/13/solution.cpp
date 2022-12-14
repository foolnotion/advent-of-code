#include <aoc.hpp>
#include <variant>

namespace detail {
    // see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2162r0.html
    struct packet : std::variant<i32, std::vector<packet>> {
        using val = i32;
        using seq = std::vector<packet>;

        using variant::variant;
        explicit packet(val v) : variant(v) { }
        explicit packet(seq const& v) : variant(v) { }

        friend auto operator<(packet const& a, packet const& b) -> bool;
    };

    struct comparison_visitor {
        auto operator()(packet::val a, packet::val b) const -> bool {
            return a < b;
        }

        auto operator()(packet::seq const& a, packet::seq const& b) const -> bool {
            return std::ranges::lexicographical_compare(a, b);
        }

        auto operator()(packet::val a, std::vector<packet> const& b) const -> bool {
            return (*this)(packet::seq{a}, b);
        }

        auto operator()(std::vector<packet> const& a, packet::val b) const -> bool {
            return (*this)(a, packet::seq{b});
        }
    };

    auto operator<(packet const& a, packet const& b) -> bool {
        return std::visit(comparison_visitor{}, a, b);
    }

    auto parse(std::string_view& s) -> packet {
        if (s[0] == '[' && s[1] == ']') {
            s.remove_prefix(2);
            return packet{ packet::seq{} }; // empty sequence
        }
        if (s[0] == '[') {
            packet::seq seq;
            while(s[0] != ']') {
                s.remove_prefix(1);
                seq.push_back(parse(s));
            }
            s.remove_prefix(1);
            return seq;
        }
        auto const* it = std::ranges::find_if(s, aoc::equals<',', ']'>);
        std::string_view v{s.begin(), it};
        auto res = scn::scan_value<i32>(v);
        s.remove_prefix(v.size());
        return packet{res.value()};
    }

    auto parse(std::string const& s) -> packet {
        std::string_view v{s.data(), s.size()};
        return parse(v);
    }
} // namespace detail

template <>
auto advent2022::day13() -> result
{
    auto input = aoc::util::readlines("./source/2022/13/input.txt");
    using detail::packet;
    using detail::parse;
    auto part1{0L};
    auto p1{parse("[[2]]")};
    auto p2{parse("[[6]]")};
    auto i1{0}, i2{0}; // NOLINT
    for (auto i = 0, j = 0; i < std::ssize(input); i += 3, ++j) {
        auto a = parse(input[i]);
        auto b = parse(input[i+1L]);
        part1 += (j + 1L) * (a < b);
        i1 += (a < p1) + (b < p1);
        i2 += (a < p2) + (b < p2);
    }
    return aoc::result(part1, (i1+1) * (i2+2));
}
