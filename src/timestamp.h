/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <stdexcept>

#include "cdns_encoder.h"

namespace CDNS {
    
    static constexpr uint64_t MILIS_PER_SEC = 1000UL;
    static constexpr uint64_t MICROS_PER_SEC = 1000000UL;
    static constexpr uint64_t NANOS_PER_SEC = 1000000000UL;

    /**
     * @brief Simple timestamp representation
     */
    struct Timestamp {
        /**
         * @brief Default constructor
         */
        Timestamp() : m_secs(0), m_ticks(0) {}

        /**
         * @brief Construct a new Timestamp object
         * @param secs Seconds since the start of UNIX epoch
         * @param ticks Subsecond number of ticks in any resolution
         */
        Timestamp(uint64_t secs, uint64_t ticks) : m_secs(secs), m_ticks(ticks) {}

        /**
         * @brief Operator `smaller than`
         * @param rhs Timestamp to compare with
         * @return `true` if the Timestamp denotes earlier time than the `rhs` one
         */
        bool operator<(const Timestamp& rhs) const {
            if (m_secs < rhs.m_secs)
                return true;

            if ((m_secs == rhs.m_secs) && (m_ticks < rhs.m_ticks))
                return true;

            return false;
        }

        /**
         * @brief Operator `smaller or equal than`
         * @param rhs Timestamp to compare with
         * @return `true` if the Timestamp denotes earlier or identical time as the `rhs` one
         */
        bool operator<=(const Timestamp& rhs) const {
            if (m_secs < rhs.m_secs)
                return true;

            if ((m_secs == rhs.m_secs) && (m_ticks <= rhs.m_ticks))
                return true;

            return false;
        }

        /**
         * @brief Calculate time difference againt given Timestamp
         * @param reference Timestamp to calculate the difference against
         * @param ticks_per_second Subsecond resolution
         * @throw std::runtime_error if ticks_per_second is 0 (prevents division by 0)
         * @return Difference between the two timestamps in ticks per second
         */
        int64_t get_time_offset(const Timestamp& reference, uint64_t ticks_per_second) {
            if (ticks_per_second == 0)
                throw std::runtime_error("Ticks per second resolution is zero!");

            int64_t secs, ticks, overflow;

            // Substract seconds part of timestamp
            if (reference.m_secs <= m_secs) {
                secs = m_secs - reference.m_secs;
            }
            else {
                secs = -1 * (reference.m_secs - m_secs);
            }

            // Substract subseconds part of timestamp
            if (reference.m_ticks <= m_ticks) {
                ticks = m_ticks - reference.m_ticks;
                overflow = ticks / ticks_per_second;
            }
            else {
                ticks = -1 * (reference.m_ticks - m_ticks);
                overflow = -1 * (std::abs(ticks) / ticks_per_second);
            }

            // Calculate timestamp offset
            int64_t diff_secs = (secs + overflow) * ticks_per_second;
            int64_t diff_ticks = std::abs(ticks) % ticks_per_second;
            if (ticks < 0)
                diff_ticks *= -1;

            return diff_secs + diff_ticks;
        }

        /**
         * @brief Serialize the Timestamp to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc) {
            std::size_t written = 0;

            // Start Timestamp array
            written += enc.write_array_start(2);

            // Write Seconds
            written += enc.write(m_secs);

            // Write Ticks
            written += enc.write(m_ticks);

            return written;
        }

        uint64_t m_secs;
        uint64_t m_ticks;
    };
}