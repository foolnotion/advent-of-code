#include <algorithm>  /// Used for std::copy
#include <array>      /// Used for std::array
#include <cassert>    /// Used for assert
#include <cstdint>
#include <cstring>    /// Used for std::memcopy
#include <iostream>   /// Used for IO operations
#include <span>
#include <string>     /// Used for strings
#include <vector>     /// Used for std::vector

/**
 * @namespace hashing
 * @brief Hashing algorithms
 */

/**
 * @namespace MD5
 * @brief Functions for the [MD5](https://en.wikipedia.org/wiki/MD5) algorithm
 * implementation
 */
namespace hashing::md5 {
/**
 * @brief Rotates the bits of a 32-bit unsigned integer
 * @param n Integer to rotate
 * @param rotate How many bits for the rotation
 * @return uint32_t The rotated integer
 */
auto left_rotate32bits(uint32_t n, std::size_t rotate) -> uint32_t {
    return (n << rotate) | (n >> (32 - rotate));
}
/**
 * @brief Checks whether integers are stored as big endian or not
 * @note Taken from [this](https://stackoverflow.com/a/1001373) StackOverflow
 * post
 * @return true IF integers are detected to work as big-endian
 * @return false IF integers are detected to work as little-endian
 */
auto is_big_endian() -> bool {
    union {
        uint32_t i;
        std::array<char, 4> c;
    } bint = {0x01020304};

    return bint.c[0] == 1;
}
/**
 * @brief Sets 32-bit integer to little-endian if needed
 * @param n Number to set to little-endian (uint32_t)
 * @return uint32_t param n with binary representation as little-endian
 */
auto to_little_endian32(uint32_t n) -> uint32_t {
    if (!is_big_endian()) {
        return ((n << 24) & 0xFF000000) | ((n << 8) & 0x00FF0000) |
               ((n >> 8) & 0x0000FF00) | ((n >> 24) & 0x000000FF);
    }
    // Machine works on little endian, no need to change anything
    return n;
}
/**
 * @brief Sets 64-bit integer to little-endian if needed
 * @param n Number to set to little-endian (uint64_t)
 * @return uint64_t param n with binary representation as little-endian
 */
auto to_little_endian64(uint64_t n) -> uint64_t {
    if (!is_big_endian()) {
        return ((n << 56) & 0xFF00000000000000) |
               ((n << 40) & 0x00FF000000000000) |
               ((n << 24) & 0x0000FF0000000000) |
               ((n << 8) & 0x000000FF00000000) |
               ((n >> 8) & 0x00000000FF000000) |
               ((n >> 24) & 0x0000000000FF0000) |
               ((n >> 40) & 0x000000000000FF00) |
               ((n >> 56) & 0x00000000000000FF);
        ;
    }
    // Machine works on little endian, no need to change anything
    return n;
}
/**
 * @brief Transforms the 128-bit MD5 signature into a 32 char hex string
 * @param sig The MD5 signature (Expected 16 bytes)
 * @return std::string The hex signature
 */
auto sig2hex(std::span<uint8_t const> sig, std::string& hex) -> void {
    char const* hexChars = "0123456789abcdef";
    for (uint8_t i = 0; i < 16; i++) {
        hex.push_back(hexChars[(sig[i] >> 4) & 0xF]);
        hex.push_back(hexChars[(sig[i]) & 0xF]);
    }
}

auto sig2hex(std::span<uint8_t const> sig) -> std::string {
    std::string hex;
    sig2hex(sig, hex);
    return hex;
}

auto sig2hex(uint8_t const* sig) -> std::string {
    return sig2hex(std::span{sig, 16UL});
}


/**
 * @brief The MD5 algorithm itself, taking in a bytestring
 * @param input_bs The bytestring to hash
 * @param input_size The size (in BYTES) of the input
 * @return void* Pointer to the 128-bit signature
 */
auto hash_bs(std::span<uint8_t const> input_bs, uint8_t* sig) -> void {
    const auto* input = input_bs.data();
    auto input_size = input_bs.size();

    // Step 0: Initial Data (Those are decided in the MD5 protocol)
    // s is the shift used in the leftrotate each round
    std::array<uint32_t, 64> s = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
    // K is pseudo-random values used each round
    // The values can be obtained by the following python code:

    /**
     * @brief Values of K are pseudo-random and used to "salt" each round
     * The values can be obtained by the following python code
     * @code{.py}
     * from math import floor, sin
     *
     * for i in range(64):
     *     print(floor(2**32 * abs(sin(i+1))))
     * @endcode
     */
    static constexpr std::array<uint32_t, 64> K = {
        3614090360, 3905402710, 606105819,  3250441966, 4118548399, 1200080426,
        2821735955, 4249261313, 1770035416, 2336552879, 4294925233, 2304563134,
        1804603682, 4254626195, 2792965006, 1236535329, 4129170786, 3225465664,
        643717713,  3921069994, 3593408605, 38016083,   3634488961, 3889429448,
        568446438,  3275163606, 4107603335, 1163531501, 2850285829, 4243563512,
        1735328473, 2368359562, 4294588738, 2272392833, 1839030562, 4259657740,
        2763975236, 1272893353, 4139469664, 3200236656, 681279174,  3936430074,
        3572445317, 76029189,   3654602809, 3873151461, 530742520,  3299628645,
        4096336452, 1126891415, 2878612391, 4237533241, 1700485571, 2399980690,
        4293915773, 2240044497, 1873313359, 4264355552, 2734768916, 1309151649,
        4149444226, 3174756917, 718787259,  3951481745};

    // The initial 128-bit state
    uint32_t a0 = 0x67452301, A = 0;
    uint32_t b0 = 0xefcdab89, B = 0;
    uint32_t c0 = 0x98badcfe, C = 0;
    uint32_t d0 = 0x10325476, D = 0;

    // Step 1: Processing the bytestring

    // First compute the size the padded message will have
    // so it is possible to allocate the right amount of memory
    uint64_t padded_message_size = input_size + (input_size % 64 < 56 ? 64 : 128) - (input_size % 64);

    std::vector<uint8_t> padded_message(padded_message_size);

    // Beginning of the padded message is the original message
    std::copy(input, input + input_size, padded_message.begin());

    // Afterwards comes a single 1 bit and then only zeroes
    padded_message[input_size] = 1 << 7;  // 10000000
    for (uint64_t i = input_size; i % 64 != 56; i++) {
        if (i == input_size) {
            continue;  // pass first iteration
        }
        padded_message[i] = 0;
    }

    // We then have to add the 64-bit size of the message at the end
    // When there is a conversion from int to bytestring or vice-versa
    // We always need to make sure it is little endian
    uint64_t input_bitsize_le = to_little_endian64(input_size * 8);
    for (uint8_t i = 0; i < 8; i++) {
        padded_message[padded_message_size - 8 + i] =
            (input_bitsize_le >> (56 - 8 * i)) & 0xFF;
    }

    // Already allocate memory for blocks
    std::array<uint32_t, 16> blocks{};

    // Rounds
    for (uint64_t chunk = 0; chunk * 64 < padded_message_size; chunk++) {
        // First, build the 16 32-bits blocks from the chunk
        for (uint8_t bid = 0; bid < 16; bid++) {
            blocks[bid] = 0;

            // Having to build a 32-bit word from 4-bit words
            // Add each and shift them to the left
            for (uint8_t cid = 0; cid < 4; cid++) {
                blocks[bid] = (blocks[bid] << 8) +
                              padded_message[chunk * 64 + bid * 4 + cid];
            }
        }

        A = a0;
        B = b0;
        C = c0;
        D = d0;

        // Main "hashing" loop
        for (uint8_t i = 0; i < 64; i++) {
            uint32_t F = 0, g = 0;
            if (i < 16) {
                F = (B & C) | ((~B) & D);
                g = i;
            } else if (i < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                F = C ^ (B | (~D));
                g = (7 * i) % 16;
            }

            // Update the accumulators
            F += A + K[i] + to_little_endian32(blocks[g]);

            A = D;
            D = C;
            C = B;
            B += left_rotate32bits(F, s[i]);
        }
        // Update the state with this chunk's hash
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }

    // Build signature from state
    // Note, any type could be used for the signature
    // uint8_t was used to make the 16 bytes obvious
    // The sig needs to be little endian
    // auto* sig = new uint8_t[16];
    for (uint8_t i = 0; i < 4; i++) {
        sig[i] = (a0 >> (8 * i)) & 0xFF;
        sig[i + 4] = (b0 >> (8 * i)) & 0xFF;
        sig[i + 8] = (c0 >> (8 * i)) & 0xFF;
        sig[i + 12] = (d0 >> (8 * i)) & 0xFF;
    }
    // return sig;
}

auto hash_bs(std::string const& input_bs, uint8_t* sig) -> void {
    hash_bs({std::bit_cast<uint8_t const*>(input_bs.data()), input_bs.size()}, sig);
}

/**
 * @brief Converts the string to bytestring and calls the main algorithm
 * @param message Plain character message to hash
 * @return void* Pointer to the MD5 signature
 */
auto hash(const std::string& message) -> std::string {
    std::array<uint8_t, 16> sig;
    hash_bs({reinterpret_cast<uint8_t const*>(message.data()), message.size()}, sig.data());
    return sig2hex(sig.data());
}

auto hash(std::string const& message, uint8_t* sig) -> std::string {
    hash_bs({reinterpret_cast<uint8_t const*>(message.data()), message.size()}, sig);
    return sig2hex(sig);
}

auto hash(std::string const& message, uint8_t* sig, std::string& hex) -> void {
    hash_bs({reinterpret_cast<uint8_t const*>(message.data()), message.size()}, sig);
    sig2hex({sig, 16UL}, hex);
}
} // namespace hashing::md5
