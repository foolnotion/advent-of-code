#include <aoc.hpp>

using std::ranges::all_of;
using std::ranges::binary_search;

template<>
auto advent2020::day04() -> result {
    constexpr int nfield{7};
    std::array<std::string, nfield> required_fields = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };

    auto constexpr nan = std::numeric_limits<i32>::quiet_NaN();
    aoc::dense::map<std::string_view, bool(*)(std::string_view)> validation_rules {
        {
            "byr", [](auto s) {
                auto res = scn::scan_value<i32>(s);
                return !static_cast<bool>(!res.has_value() || res->value() < 1920 || res->value() > 2002);
            }
        },
        {
            "iyr", [](auto s) {
                auto res = scn::scan_value<int>(s);
                return !static_cast<bool>(!res.has_value() || res->value() < 2010 || res->value() > 2020);
            }
        },
        {
            "eyr", [](auto s) {
                auto res = scn::scan_value<int>(s);
                return !static_cast<bool>(!res.has_value() || res->value() < 2020 || res->value() > 2030);
            }
        },
        {
            "hgt", [](auto s) {
                std::string_view unit(s.data() + s.size() - 2, 2);
                std::string_view height(s.data(), s.size() - 2);
                auto res = scn::scan_value<int>(height);

                if (!res.has_value()) {
                    return false;
                }

                auto heightval = res->value();

                if (unit == "cm") {
                    return heightval >= 150 && heightval <= 193;
                }

                if (unit == "in") {
                    return heightval >= 59 && heightval <= 76;
                }

                return false;
            }
        },
        {
            "hcl", [](auto s) {
                if (s.size() != 7) { return false; }
                if (s[0] != '#') { return false; }
                return all_of(s.begin() + 1, s.end(), [](char c) {
                    return std::isdigit(c) || (std::islower(c) && std::isalpha(c));
                });
            }
        },
        {
            "ecl", [](auto s) {
                return
                    s == "amb" || s == "blu" || s == "brn" ||
                    s == "gry" || s == "grn" || s == "hzl" || s == "oth";
            }
        },
        {
            "pid", [](auto s) {
                auto f = [](char c) { return std::isdigit(c); };
                return s.size() == 9 && all_of(s, f);
            }
        },
        {
            "cid", [](auto  s) { return true; }
        }
    };

    std::vector<std::string_view> fields;
    auto validate_entry = [&](std::string_view entry) {
        // validate passport entry
        fields.clear();
        auto pos = 0L;
        while (pos < entry.size()) {
            if (pos = entry.find(':', pos+1); pos != -1) {
                auto field = std::string_view(entry.data() + pos - 3, 3);
                auto pos1 = entry.find(' ', pos+1);
                if (pos1 == -1) { pos1 = entry.size(); }
                auto value = std::string_view(entry.data() + pos + 1, pos1 - pos - 1);
                auto it = validation_rules.find(field);

                if (it != validation_rules.end() && it->second(value)) {
                    fields.push_back(field);
                }
            }
        }
        std::sort(begin(fields), end(fields));
        return all_of(required_fields, [&](auto const& f) {
            return binary_search(fields, f);
        });
    };

    std::string buf;
    bool space = false;
    int valid_entries = 0;
    auto input = aoc::util::readlines("./source/2020/04/input.txt");
    for(auto const& line : input) {
        if (line.empty() && !buf.empty()) {
            std::string_view entry(buf.data(), buf.size());
            valid_entries += validate_entry(entry);
            // encountered an empty line, reset the buffer
            buf.clear();
        }
        fmt::format_to(std::back_inserter(buf), "{}", buf.empty() ? "" : " ");
        for (auto c : line) {
            if (std::isspace(c)) {
                space = true;
                continue;
            }
            if (space) {
                space = false;
                fmt::format_to(std::back_inserter(buf), "{}", ' ');
            }
            fmt::format_to(std::back_inserter(buf), "{}", c);
        }
    }
    auto p2 = valid_entries;
    return aoc::result("242", p2); // for part 1 just change the required fields, all rules are there
}
