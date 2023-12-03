#include <aoc.hpp>
#include <pratt-parser/parser.hpp>
#include <pratt-parser/token.hpp>

namespace {
enum op { add, sub, mul, noop };

struct nud {
    using token_t = pratt::token<i64>;
    using value_t = typename token_t::value_t;
    using token_kind = pratt::token_kind;

    template <typename Parser>
    auto operator()(Parser& parser, token_t const& tok, token_t const& left) -> value_t
    {
        auto bp = tok.precedence(); // binding power
        switch (tok.kind()) {
        case pratt::token_kind::constant: { return left.value(); }
        case token_kind::dynamic: {
            switch (tok.opcode()) {
            case op::sub: { return -parser.parse_bp(bp, token_kind::eof).value(); }
            default: {
                throw std::runtime_error(fmt::format("nud: unknown dynamic node opcode: {}\n", tok.opcode()));
            }
            }
        }
        case token_kind::lparen: { return parser.parse_bp(bp, token_kind::rparen).value(); }
        default: {
            throw std::runtime_error(fmt::format("nud: unsupported token {} {}\n", tok.name(), tok.to_string()));
        };
        }
    };
};

struct led {
    using token_t = pratt::token<i64>;
    using value_t = typename token_t::value_t;
    using token_kind = pratt::token_kind;

    template <typename Parser>
    auto operator()(Parser& /*unused*/, token_t const& tok, token_t const& left, token_t const& right) -> value_t
    {
        auto lhs = left.value();
        auto rhs = right.value();

        switch (tok.kind()) {
        case token_kind::dynamic:
            switch (tok.opcode()) {
            case op::add: { return lhs + rhs; }
            case op::sub: { return lhs - rhs; }
            case op::mul: { return lhs * rhs; }
            default: {
                throw std::runtime_error("led: unknown dynamic node opcode " + std::to_string(tok.opcode()));
            }
            }
            break;

        default:
            throw std::runtime_error("led: unsupported token " + std::string(tok.name()));
        };
    }
};

} // namespace

template <>
auto advent2020::day18() -> result
{
    using token = nud::token_t;
    using op = op;
    using as = pratt::associativity;
    using ki = pratt::token_kind;

    aoc::dense::map<std::string_view, token> tokens {
        { "+", token(ki::dynamic, "+", op::add, 1, as::left) },
        { "-", token(ki::dynamic, "-", op::sub, 1, as::left) },
        { "*", token(ki::dynamic, "*", op::mul, 1, as::left) },
        { "(", token(ki::lparen, "(", op::noop, 0, as::none) },
        { ")", token(ki::rparen, "(", op::noop, 0, as::none) },
        { "eof", token(ki::eof, "eof", op::noop, 0, as::none) }
    };

    using parser = pratt::parser<nud, led, std::identity, decltype(tokens)>;
    auto input = aoc::util::readlines("./source/2020/18/input.txt");
    auto evaluate = [&tokens](auto const& expr) { return parser(expr, tokens, {}).parse(); };
    auto p1 = std::transform_reduce(input.begin(), input.end(), 0UL, std::plus{}, evaluate);

    tokens["+"] = token(ki::dynamic, "+", op::add, 2, as::left);
    auto p2 = std::transform_reduce(input.begin(), input.end(), 0UL, std::plus{}, evaluate);
    return aoc::result(p1, p2);
}
