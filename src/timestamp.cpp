/**
 * Copyright © 2020 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include "timestamp.h"

int64_t CDNS::Timestamp::get_time_offset(const Timestamp& reference, uint64_t ticks_per_second)
{
    if (ticks_per_second == 0)
        throw std::runtime_error("Ticks per second resolution is zero!");

    int64_t ticks = (m_secs * ticks_per_second) + m_ticks;
    int64_t ref_ticks = (reference.m_secs * ticks_per_second) + reference.m_ticks;

    return ticks - ref_ticks;
}

void CDNS::Timestamp::add_time_offset(int64_t offset, uint64_t ticks_per_second)
{
    if (ticks_per_second == 0)
        throw std::runtime_error("Ticks per second resolution is zero!");

    int64_t ticks = (m_secs * ticks_per_second) + m_ticks;

    if (-1 * offset > ticks)
        throw std::runtime_error("Adding offset to Timestamp would create invalid Timestamp!");

    ticks += offset;
    m_secs = ticks / ticks_per_second;
    m_ticks = ticks % ticks_per_second;
}

std::string CDNS::Timestamp::string()
{
    std::stringstream ss;

    ss << "Timestamp:" << std::endl;
    ss << "\tSeconds: " << std::to_string(m_secs) << std::endl;
    ss << "\tTicks: " << std::to_string(m_ticks) << std::endl;

    return ss.str();
}

std::size_t CDNS::Timestamp::write(CdnsEncoder& enc)
{
    std::size_t written = 0;

    // Start Timestamp array
    written += enc.write_array_start(2);

    // Write Seconds
    written += enc.write(m_secs);

    // Write Ticks
    written += enc.write(m_ticks);

    return written;
}

void CDNS::Timestamp::read(CdnsDecoder& dec)
{
    reset();
    bool is_m_secs = false;
    bool is_m_ticks = false;

    bool indef = false;
    uint64_t length = dec.read_array_start(indef);

    for (unsigned i = 0; (i < length) || indef; i++) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (i) {
            case 0:
                m_secs = dec.read_unsigned();
                is_m_secs = true;
                break;
            case 1:
                m_ticks = dec.read_unsigned();
                is_m_ticks = true;
                break;
            default:
                throw CdnsDecoderException("Timestamp array has more than 2 items");
                break;
        }
    }

    if (!is_m_secs || !is_m_ticks)
        throw CdnsDecoderException("Wrong format of the Timestamp");
}

void CDNS::Timestamp::reset()
{
    m_secs = 0;
    m_ticks = 0;
}