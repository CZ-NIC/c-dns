#pragma once

#include <cstdint>
#include <cstddef>
#include <smmintrin.h>

namespace CDNS {

    /**
     * CRC32 calculation for hashes on std::unordered_map non-primitive keys
     */
    template <class T> struct hash {
        std::size_t operator()(T const& val) const {
            const char *start = reinterpret_cast<const char*>(&val);
            const char *end = start + sizeof(val);
            uint32_t ret = ~0U;

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
    };
}