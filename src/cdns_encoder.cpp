#include <cstring>
#include <iostream>

#include "cdns_encoder.h"

void CDNS::CdnsEncoder::write_array_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    update_buffer(cbor_encode_array_start(size, m_p, m_avail));
}

void CDNS::CdnsEncoder::write_indef_array_start()
{
    if (m_avail < 1)
        flush_buffer();
    update_buffer(cbor_encode_indef_array_start(m_p, m_avail));
}

void CDNS::CdnsEncoder::write_map_start(std::size_t size)
{
    if (m_avail < 9)
        flush_buffer();
    update_buffer(cbor_encode_map_start(size, m_p, m_avail));
}

void CDNS::CdnsEncoder::write_indef_map_start()
{
    if (m_avail < 1)
        flush_buffer();
    update_buffer(cbor_encode_indef_map_start(m_p, m_avail));
}

void CDNS::CdnsEncoder::write_bytestring(const unsigned char* str, std::size_t size)
{
    if (!str)
        return;
    
    if (m_avail < 9)
        flush_buffer();
    update_buffer(cbor_encode_bytestring_start(size, m_p, m_avail));

    write_string(str, size);
}

void CDNS::CdnsEncoder::write_textstring(const unsigned char* str, std::size_t size)
{
    if (!str)
        return;

    if (m_avail < 9)
        flush_buffer();
    update_buffer(cbor_encode_string_start(size, m_p, m_avail));

    write_string(str, size);
}

void CDNS::CdnsEncoder::write_break()
{
    if (m_avail < 1)
        flush_buffer();
    update_buffer(cbor_encode_break(m_p, m_avail));
}

void CDNS::CdnsEncoder::write(bool value)
{
    if (m_avail < 1)
        flush_buffer();
    update_buffer(cbor_encode_bool(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(uint8_t value)
{
    if (m_avail < 2)
        flush_buffer();
    update_buffer(cbor_encode_uint8(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(uint16_t value)
{
    if (m_avail < 3)
        flush_buffer();
    update_buffer(cbor_encode_uint16(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(uint32_t value)
{
    if (m_avail < 5)
        flush_buffer();
    update_buffer(cbor_encode_uint32(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(uint64_t value)
{
    if (m_avail < 9)
        flush_buffer();
    update_buffer(cbor_encode_uint64(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(int8_t value)
{
    if (m_avail < 2)
        flush_buffer();
    if (value < 0)
        update_buffer(cbor_encode_negint8(~value, m_p, m_avail));
    else
        update_buffer(cbor_encode_uint8(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(int16_t value)
{
    if (m_avail < 3)
        flush_buffer();
    if (value < 0)
        update_buffer(cbor_encode_negint16(~value, m_p, m_avail));
    else
        update_buffer(cbor_encode_uint16(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(int32_t value)
{
    if (m_avail < 5)
        flush_buffer();
    if (value < 0)
        update_buffer(cbor_encode_negint32(~value, m_p, m_avail));
    else
        update_buffer(cbor_encode_uint32(value, m_p, m_avail));
}

void CDNS::CdnsEncoder::write(int64_t value)
{
    if (m_avail < 9)
        flush_buffer();
    if (value < 0)
        update_buffer(cbor_encode_negint64(~value, m_p, m_avail));
    else
        update_buffer(cbor_encode_uint64(value, m_p, m_avail));
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