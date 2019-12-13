#pragma once

#include <cstdint>
#include <cstddef>
#include <smmintrin.h>
#include <optional>

namespace CDNS {

    /**
     * @brief CRC32 calculation for hashes on std::unordered_map non-primitive keys
     * @param data Pointer to the start of data to calculate hash on
     * @param size Size of the data in bytes
     * @param seed Initial seed value for hash calculation
     * @return Hash value for "data"
     */
    template<class T>
    std::size_t hash_value(T const* data, std::size_t size, uint32_t seed = ~0U) {
        const char* start = reinterpret_cast<const char*>(data);
        const char* end = start + size;
        uint32_t ret = seed;

        for ( ; start < end; ) {
            // At least 8 bytes of data left
            if (start + 8 <= end) {
                ret = _mm_crc32_u64(ret, *reinterpret_cast<const uint64_t*>(start));
                start += 8;
            }
            // At least 4 bytes of data left
            else if (start + 4 <= end) {
                ret = _mm_crc32_u32(ret, *reinterpret_cast<const uint32_t*>(start));
                start += 4;
            }
            // At least 2 bytes of data left
            else if (start + 2 <= end) {
                ret = _mm_crc32_u16(ret, *reinterpret_cast<const uint16_t*>(start));
                start += 2;
            }
            // 1 byte of data left
            else {
                ret = _mm_crc32_u8(ret, *reinterpret_cast<const uint8_t*>(start));
                start += 1;
            }
        }

        return static_cast<std::size_t>(~ret);
    }

    /**
     * @brief CRC32 calculation for hashes on std::unordered_map non-primitive keys
     * @param data Data to calculate hash on
     * @param seed Initial seed value for hash calculation
     * @return Hash value for "data"
     */
    template<class T>
    std::size_t hash_value(T const& data, uint32_t seed = ~0U) {
        return hash_value(&data, sizeof(data), seed);
    }

    /**
     * @brief CRC32 calculation for hashes on std::unordered_map non-primitive keys
     * @param data Optional data to calculate hash on
     * @param seed Initial seed value for hash calculation
     * @return Hash value for "data"
     */
    template<class T>
    std::size_t hash_value(std::optional<T> const& data, uint32_t seed = ~0U) {
        if (data)
            return hash_value(&data.value(), sizeof(data.value()), seed);
        else
            return seed;
    }

    /**
     * @brief Hash structure to give to std::unordered_map as alternative hash function
     */
    template <class T> struct hash {
        std::size_t operator()(T const& data) const {
            return hash_value(data);
        }
    };
}
