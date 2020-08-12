/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <iostream>

#include "cdns_encoder.h"

std::size_t CDNS::CdnsEncoder::write_int(uint64_t value, CborType major)
{
    if (value <= 23) {
        if (m_avail >= 1) {
            m_p[0] = static_cast<uint8_t>(major) | value;
            return 1;
        }
    }
    else if (value <= UINT8_MAX) {
        if (m_avail >= 2) {
            m_p[0] = static_cast<uint8_t>(major) | 24;
            m_p[1] = value;
            return 2;
        }
    }
    else if (value <= UINT16_MAX) {
        if (m_avail >= 3) {
            m_p[0] = static_cast<uint8_t>(major) | 25;
            m_p[1] = value >> 8;
            m_p[2] = value;
            return 3;
        }
    }
    else if (value <= UINT32_MAX) {
        if (m_avail >= 5) {
            m_p[0] = static_cast<uint8_t>(major) | 26;
            m_p[1] = value >> 24;
            m_p[2] = value >> 16;
            m_p[3] = value >> 8;
            m_p[4] = value;
            return 5;
        }
    }
    else {
        if (m_avail >= 9) {
            m_p[0] = static_cast<uint8_t>(major) | 27;
            m_p[1] = value >> 56;
            m_p[2] = value >> 48;
            m_p[3] = value >> 40;
            m_p[4] = value >> 32;
            m_p[5] = value >> 24;
            m_p[6] = value >> 16;
            m_p[7] = value >> 8;
            m_p[8] = value;
            return 9;
        }
    }

    return 0;
}

std::size_t CDNS::CdnsEncoder::write_array_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = write_int(size, CborType::ARRAY);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_indef_array_start()
{
    if (m_avail < 1)
        flush_buffer();

    if (m_avail < 1)
        return 0;

    m_p[0] = static_cast<uint8_t>(CborType::ARRAY) | 31;
    update_buffer(1);
    return 1;
}

std::size_t CDNS::CdnsEncoder::write_map_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = write_int(size, CborType::MAP);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_indef_map_start()
{
    if (m_avail < 1)
        flush_buffer();

    if (m_avail < 1)
        return 0;

    m_p[0] = static_cast<uint8_t>(CborType::MAP) | 31;
    update_buffer(1);
    return 1;
}

std::size_t CDNS::CdnsEncoder::write_bytestring(const unsigned char* str, std::size_t size)
{
    if (!str)
        return 0;
    
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = write_int(size, CborType::BYTE_STRING);
    update_buffer(written);

    write_string(str, size);
    return written + size;
}

std::size_t CDNS::CdnsEncoder::write_textstring(const unsigned char* str, std::size_t size)
{
    if (!str)
        return 0;

    if (m_avail < 9)
        flush_buffer();
    std::size_t written = write_int(size, CborType::TEXT_STRING);
    update_buffer(written);

    write_string(str, size);
    return written + size;
}

std::size_t CDNS::CdnsEncoder::write_break()
{
    if (m_avail < 1)
        flush_buffer();

    if (m_avail < 1)
        return 0;

    m_p[0] = static_cast<uint8_t>(CborType::SIMPLE) | 31;
    update_buffer(1);
    return 1;
}

std::size_t CDNS::CdnsEncoder::write(bool value)
{
    std::size_t written;
    if (m_avail < 1)
        flush_buffer();

    if (value)
        written = write_int(21, CborType::SIMPLE);
    else
        written = write_int(20, CborType::SIMPLE);

    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint8_t value)
{
    if (m_avail < 2)
        flush_buffer();
    std::size_t written = write_int(value, CborType::UNSIGNED);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint16_t value)
{
    if (m_avail < 3)
        flush_buffer();
    std::size_t written = write_int(value, CborType::UNSIGNED);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint32_t value)
{
    if (m_avail < 5)
        flush_buffer();
    std::size_t written = write_int(value, CborType::UNSIGNED);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint64_t value)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = write_int(value, CborType::UNSIGNED);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(int8_t value)
{
    std::size_t written;

    if (m_avail < 2)
        flush_buffer();
    if (value < 0) {
        written = write_int(~value, CborType::NEGATIVE);
        update_buffer(written);
    }
    else {
        written = write_int(value, CborType::UNSIGNED);
        update_buffer(written);
    }

    return written;
}

std::size_t CDNS::CdnsEncoder::write(int16_t value)
{
    std::size_t written;

    if (m_avail < 3)
        flush_buffer();
    if (value < 0) {
        written = write_int(~value, CborType::NEGATIVE);
        update_buffer(written);
    }
    else {
        written = write_int(value, CborType::UNSIGNED);
        update_buffer(written);
    }

    return written;
}

std::size_t CDNS::CdnsEncoder::write(int32_t value)
{
    std::size_t written;

    if (m_avail < 5)
        flush_buffer();
    if (value < 0) {
        written = write_int(~value, CborType::NEGATIVE);
        update_buffer(written);
    }
    else {
        written = write_int(value, CborType::UNSIGNED);
        update_buffer(written);
    }

    return written;
}

std::size_t CDNS::CdnsEncoder::write(int64_t value)
{
    std::size_t written;

    if (m_avail < 9)
        flush_buffer();
    if (value < 0) {
        written = write_int(~value, CborType::NEGATIVE);
        update_buffer(written);
    }
    else {
        written = write_int(value, CborType::UNSIGNED);
        update_buffer(written);
    }

    return written;
}

void CDNS::CdnsEncoder::flush_buffer()
{
    if (m_p != m_buffer) {
        m_cos->write(reinterpret_cast<const char*>(m_buffer), m_p - m_buffer);
        m_p = m_buffer;
        m_avail = BUFFER_SIZE;
    }
}

void CDNS::CdnsEncoder::write_string(const unsigned char* str, std::size_t size)
{
    const unsigned char* str_left = str;
    std::size_t size_left = size;

    while(m_avail < size_left) {
        std::memcpy(m_p, str_left, m_avail);
        size_left -= m_avail;
        str_left += m_avail;
        update_buffer(m_avail);
        flush_buffer();
    }

    std::memcpy(m_p, str_left, size_left);
    update_buffer(size_left);
}