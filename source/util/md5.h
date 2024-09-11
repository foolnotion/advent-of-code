#include <cstdint>
#include <span>
#include <string>

namespace hashing::md5 {
    auto hash(std::string const& message) -> std::string;
    auto hash(std::string const& message, uint8_t* sig) -> std::string;
    auto hash(std::string const& message, uint8_t* sig, std::string& hex) -> void;

    auto hash_bs(std::string const& message, uint8_t* sig) -> void;
    auto hash_bs(std::span<uint8_t const> input_bs, uint8_t* sig) -> void;
    auto sig2hex(uint8_t const* sig) -> std::string;
} // namespace hashing::md5