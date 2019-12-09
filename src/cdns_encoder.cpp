#include <cstring>
#include <iostream>

#include "cdns_encoder.h"

std::size_t CDNS::CdnsEncoder::write_array_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = cbor_encode_array_start(size, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_indef_array_start()
{
    if (m_avail < 1)
        flush_buffer();
    std::size_t written = cbor_encode_indef_array_start(m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_map_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = cbor_encode_map_start(size, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_indef_map_start()
{
    if (m_avail < 1)
        flush_buffer();
    std::size_t written = cbor_encode_indef_map_start(m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write_bytestring(const unsigned char* str, std::size_t size)
{
    if (!str)
        return 0;
    
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = cbor_encode_bytestring_start(size, m_p, m_avail);
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
    std::size_t written = cbor_encode_string_start(size, m_p, m_avail);
    update_buffer(written);

    write_string(str, size);
    return written + size;
}

std::size_t CDNS::CdnsEncoder::write_break()
{
    if (m_avail < 1)
        flush_buffer();
    std::size_t written = cbor_encode_break(m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(bool value)
{
    if (m_avail < 1)
        flush_buffer();
    std::size_t written = cbor_encode_bool(value, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint8_t value)
{
    if (m_avail < 2)
        flush_buffer();
    std::size_t written = cbor_encode_uint8(value, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint16_t value)
{
    if (m_avail < 3)
        flush_buffer();
    std::size_t written = cbor_encode_uint16(value, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint32_t value)
{
    if (m_avail < 5)
        flush_buffer();
    std::size_t written = cbor_encode_uint32(value, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(uint64_t value)
{
    if (m_avail < 9)
        flush_buffer();
    std::size_t written = cbor_encode_uint64(value, m_p, m_avail);
    update_buffer(written);
    return written;
}

std::size_t CDNS::CdnsEncoder::write(int8_t value)
{
    std::size_t written;

    if (m_avail < 2)
        flush_buffer();
    if (value < 0) {
        written = cbor_encode_negint8(~value, m_p, m_avail);
        update_buffer(written);
    }
    else {
        written = cbor_encode_uint8(value, m_p, m_avail);
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
        written = cbor_encode_negint16(~value, m_p, m_avail);
        update_buffer(written);
    }
    else {
        written = cbor_encode_uint16(value, m_p, m_avail);
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
        written = cbor_encode_negint32(~value, m_p, m_avail);
        update_buffer(written);
    }
    else {
        written = cbor_encode_uint32(value, m_p, m_avail);
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
        written = cbor_encode_negint64(~value, m_p, m_avail);
        update_buffer(written);
    }
    else {
        written = cbor_encode_uint64(value, m_p, m_avail);
        update_buffer(written);
    }

    return written;
}

void CDNS::CdnsEncoder::flush_buffer()
{
    if (m_p != m_buffer) {
        try {
            m_cos->write(reinterpret_cast<const char*>(m_buffer), m_p - m_buffer);
            m_p = m_buffer;
            m_avail = BUFFER_SIZE;
        }
        catch (CborOutputException& e) {
            std::cerr << e.what() << std::endl;
        }
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