#include <aoc.hpp>
#include <chrono>
#include <semaphore>
#include <thread>
#include <utility>
#include <variant>

using namespace std::chrono_literals;

namespace {
    struct instruction {
        static constexpr auto snd = aoc::util::hash{}("snd");
        static constexpr auto set = aoc::util::hash{}("set");
        static constexpr auto add = aoc::util::hash{}("add");
        static constexpr auto mul = aoc::util::hash{}("mul");
        static constexpr auto mod = aoc::util::hash{}("mod");
        static constexpr auto rcv = aoc::util::hash{}("rcv");
        static constexpr auto jgz = aoc::util::hash{}("jgz");
        u64 opcode{};
        std::variant<char, i64> a;
        std::variant<char, i64> b;
    };

    template<typename T>
    requires std::is_arithmetic_v<T>
    struct concurrent_queue {
        std::mutex m;
        std::queue<T> q;

        auto push(T t) {
            std::lock_guard const lock(m);
            q.push(t);
        }

        auto pop() {
            std::lock_guard const lock(m);
            q.pop();
        }

        auto front() {
            std::lock_guard const lock(m);
            return q.front();
        }

        auto empty() {
            std::lock_guard const lock(m);
            return q.empty();
        }

        auto pop_front() -> std::optional<T> {
            std::lock_guard const lock(m);
            if (q.empty()) { return {}; }
            auto x = q.front();
            q.pop();
            return x;
        }
    };

    struct program {
        std::array<i64, aoc::constants::alphabet.size()> reg = {0};
        std::vector<instruction> code;
        i64 sendcount{0};
        bool waiting{false};

        explicit program(std::vector<instruction> c) : code{std::move(c)} { }

        auto operator()(char c) const -> i64 {
            return reg[c-'a'];
        }

        auto operator()(i64 v) const -> i64 {
            return v;
        }

        template<typename OP>
        auto apply_binary(instruction const& in) {
            auto c = std::get<char>(in.a);
            auto v = std::visit(*this, in.b);
            reg[c-'a'] = OP{}(reg[c-'a'], v);
        }

        auto operator()(auto& recv, auto& recv_sem, auto& send, auto& send_sem) -> i64 {
            i64 freq{};

            for (auto i = 0; i < std::ssize(code); ++i) {
                if (i < 0 || i >= std::ssize(code)) { break; }
                auto const& in = code[i];
                auto j = i;
                switch(in.opcode) {
                    case instruction::snd: {
                        auto x = std::visit(*this, in.a);
                        send.push(x);
                        send_sem.release();
                        ++sendcount;
                        break;
                    }
                    case instruction::set: {
                        auto c = std::get<char>(in.a);
                        auto v = std::visit(*this, in.b);
                        reg[c-'a'] = v;
                        break;
                    }
                    case instruction::add: {
                        apply_binary<std::plus<>>(in);
                        break;
                    }
                    case instruction::mul: {
                        apply_binary<std::multiplies<>>(in);
                        break;
                    }
                    case instruction::mod: {
                        apply_binary<std::modulus<>>(in);
                        break;
                    }
                    case instruction::rcv: {
                        // this is for part 1
                        // auto x = std::visit(*this, in.a);
                        // if (x != 0) {
                        //     frequency = freq;
                        //     return frequency;
                        // }
                        waiting = true;
                        recv_sem.acquire();
                        waiting = false;
                        auto x = recv.pop_front();
                        auto c = std::get<char>(in.a);
                        if (x) {
                            reg[c-'a'] = x.value();
                        } else {
                            return freq;
                        }
                        break;
                    }
                    case instruction::jgz: {
                        auto x = std::visit(*this, in.a);
                        auto y = std::visit(*this, in.b);
                        if (x > 0) { i += y-1; }
                        break;
                    }
                }
            }
            return freq;
        }
    };

    auto parse(auto const& input) {
        std::vector<instruction> instructions;
        instructions.reserve(input.size());

        for (auto const& s : input) {
            auto vec = lz::map(lz::split(s, ' '), [](auto x) { return std::string(x.begin(), x.end()); }).toVector();

            auto fill_value = [](std::variant<char, i64>& var, std::string const& str) {
                if (std::isalpha(str.front())) {
                    var = scn::scan_value<char>(str)->value();
                } else {
                    var = scn::scan_value<i64>(str)->value();
                }
            };

            instruction in;
            in.opcode = aoc::util::hash{}(vec.front());;
            switch(in.opcode) {
                case instruction::snd:
                case instruction::rcv:
                {
                    fill_value(in.a, vec[1]);
                    break;
                }
                default: {
                    fill_value(in.a, vec[1]);
                    fill_value(in.b, vec[2]);
                    break;
                }
            }
            instructions.push_back(in);
        }
        return instructions;
    }
} // namespace

template<>
auto advent2017::day18() -> result {
    auto input = aoc::util::readlines("./source/2017/18/input.txt");
    auto instructions = parse(input);
    auto const p1{2951};

    concurrent_queue<i64> q1;
    concurrent_queue<i64> q2;

    program a{instructions};
    program b{instructions};
    b.reg['p'-'a'] = 1;

    std::counting_semaphore<> s1(0);
    std::counting_semaphore<> s2(0);

    std::jthread t1{[&]() { a(q1, s1, q2, s2); }};
    std::jthread t2{[&]() { b(q2, s2, q1, s1); }};
    while(true) {
        if (a.waiting && b.waiting && q1.empty() && q2.empty()) {
            s1.release(decltype(s1)::max());
            s2.release(decltype(s2)::max());
            break;
        }
        std::this_thread::sleep_for(1ms);
    }
    auto const p2 = b.sendcount;
    return aoc::result(p1, p2);
}