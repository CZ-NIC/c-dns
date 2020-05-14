/**
 * Copyright © 2020 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "cdns_decoder.h"

CDNS::CborType CDNS::CdnsDecoder::peek_type()
{
    read_to_buffer();
    if (static_cast<CborType>(m_p[0]) == CborType::BREAK)
        return CborType::BREAK;
    else
        return static_cast<CborType>(m_p[0] & 0xE0);
}

uint64_t CDNS::CdnsDecoder::read_unsigned()
{
    CborType cbor_type;
    uint8_t item_length;
    read_cbor_type(cbor_type, item_length);
    if (cbor_type != CborType::UNSIGNED) {
        throw CdnsDecoderException(("read_unsigned() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (item_length >= 28) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(item_length)).c_str());
    }

    return read_int(item_length);
}

int64_t CDNS::CdnsDecoder::read_negative()
{
    CborType cbor_type;
    uint8_t item_length;
    read_cbor_type(cbor_type, item_length);
    if (cbor_type != CborType::NEGATIVE) {
        throw CdnsDecoderException(("read_negative() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (item_length >= 28) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(item_length)).c_str());
    }

    return -1 - read_int(item_length);
}

int64_t CDNS::CdnsDecoder::read_integer()
{
    CborType peek = peek_type();
    switch (peek) {
        case CborType::UNSIGNED:
            return read_unsigned();
            break;
        case CborType::NEGATIVE:
            return read_negative();
            break;
        default:
            throw CdnsDecoderException(("read_integer() called on wrong major type " +
                                        std::to_string(static_cast<uint8_t>(peek) >> 5)).c_str());
            break;
    }
}

bool CDNS::CdnsDecoder::read_bool()
{
    CborType cbor_type;
    uint8_t bool_value;
    read_cbor_type(cbor_type, bool_value);
    if (cbor_type != CborType::SIMPLE && cbor_type != CborType::UNSIGNED) {
        throw CdnsDecoderException(("read_bool() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (cbor_type == CborType::SIMPLE && (bool_value != 20 && bool_value != 21)) {
        throw CdnsDecoderException("CBOR additional information value isn't bool");
    }
    else if (cbor_type == CborType::UNSIGNED && bool_value >= 28) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(bool_value)).c_str());
    }

    if (cbor_type == CborType::SIMPLE) {
        return bool_value == 21;
    }
    else {
        return read_int(bool_value) != 0;
    }
}

std::string CDNS::CdnsDecoder::read_bytestring()
{
    CborType cbor_type;
    uint8_t item_length;
    read_cbor_type(cbor_type, item_length);
    if (cbor_type != CborType::BYTE_STRING) {
        throw CdnsDecoderException(("read_bytestring() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (item_length >= 28 && item_length <= 30) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(item_length)).c_str());
    }

    return read_string(CborType::BYTE_STRING, read_int(item_length), item_length == 31 ? true : false);

}

std::string CDNS::CdnsDecoder::read_textstring()
{
    CborType cbor_type;
    uint8_t item_length;
    read_cbor_type(cbor_type, item_length);
    if (cbor_type != CborType::TEXT_STRING) {
        throw CdnsDecoderException(("read_textstring() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (item_length >= 28 && item_length <= 30) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(item_length)).c_str());
    }

    return read_string(CborType::TEXT_STRING, read_int(item_length), item_length == 31 ? true : false);
}

uint64_t CDNS::CdnsDecoder::read_array_start(bool& indef)
{
    CborType cbor_type;
    uint8_t array_length_value;
    read_cbor_type(cbor_type, array_length_value);
    if (cbor_type != CborType::ARRAY) {
        throw CdnsDecoderException(("read_array_start() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (array_length_value >= 28 && array_length_value <= 30) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(array_length_value)).c_str());
    }

    if (array_length_value == 31) {
        indef = true;
        return 0;
    }

    indef = false;
    return read_int(array_length_value);
}

uint64_t CDNS::CdnsDecoder::read_map_start(bool& indef)
{
    CborType cbor_type;
    uint8_t map_length_value;
    read_cbor_type(cbor_type, map_length_value);
    if (cbor_type != CborType::MAP) {
        throw CdnsDecoderException(("read_map_start() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
    else if (map_length_value >= 28 && map_length_value <= 30) {
        throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                    std::to_string(map_length_value)).c_str());
    }

    if (map_length_value == 31) {
        indef = true;
        return 0;
    }

    indef = false;
    return read_int(map_length_value);
}

void CDNS::CdnsDecoder::read_break()
{
    CborType cbor_type;
    uint8_t break_code;
    read_cbor_type(cbor_type, break_code);
    if (cbor_type != CborType::SIMPLE || break_code != 31) {
        throw CdnsDecoderException(("read_break() called on wrong major type " +
                                    std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
    }
}

void CDNS::CdnsDecoder::skip_item()
{
    CborType cbor_type;
    uint8_t item_length;
    read_cbor_type(cbor_type, item_length);

    switch (cbor_type) {
        case CborType::UNSIGNED:
        case CborType::NEGATIVE:
        case CborType::TAG:
            if (item_length >= 28) {
                throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                            std::to_string(item_length)).c_str());
            }
            read_int(item_length);
            break;

        case CborType::SIMPLE:
            if (item_length >= 28 && item_length <= 30) {
                throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                            std::to_string(item_length)).c_str());
            }
            read_int(item_length);
            break;

        case CborType::BYTE_STRING:
        case CborType::TEXT_STRING:
            if (item_length >= 28 && item_length <= 30) {
                throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                            std::to_string(item_length)).c_str());
            }
            read_string(cbor_type, read_int(item_length), item_length == 31 ? true : false);
            break;

        case CborType::ARRAY:
        case CborType::MAP:
            if (item_length >= 28 && item_length <= 30) {
                throw CdnsDecoderException(("Unsupported CBOR additional information value: " +
                                            std::to_string(item_length)).c_str());
            }
            if (item_length == 31) {
                while(true) {
                    if (peek_type() == CborType::SIMPLE && (m_p[0] & 0x1F) == 31) {
                        m_p++;
                        break;
                    }
                    skip_item();
                    if (cbor_type == CborType::MAP)
                        skip_item();
                }
            }
            else {
                uint64_t item_count = read_int(item_length);
                for (int i = 0; i < item_count; i++) {
                    skip_item();
                    if (cbor_type == CborType::MAP)
                        skip_item();
                }
            }
            break;

        default:
            throw CdnsDecoderException(("Unknown CBOR major type " +
                                        std::to_string(static_cast<uint8_t>(cbor_type) >> 5)).c_str());
            break;
    }
}

void CDNS::CdnsDecoder::read_cbor_type(CborType& cbor_type, uint8_t& additional)
{
    read_to_buffer();
    cbor_type = static_cast<CborType>(m_p[0] & 0xE0);
    additional = m_p[0] & 0x1F;
    m_p++;
}

uint64_t CDNS::CdnsDecoder::read_int(uint8_t item_length)
{
    uint64_t value = 0;

    if (item_length <= 23) {
        return item_length;
    }
    else if (item_length >= 24 && item_length <= 27) {
        for (int i = 1 << (item_length - 24); i > 0; i--) {
            read_to_buffer();
            value += (m_p[0] << ((i - 1) * 8));
            m_p++;
        }
    }

    return value;
}

std::string CDNS::CdnsDecoder::read_string(CborType cbor_type, uint64_t length, bool indef)
{
    std::string ret;

    if (!indef) {
        ret.reserve(length);
        for (int i = 0; i < length; i++) {
            read_to_buffer();
            ret.push_back(m_p[0]);
            m_p++;
        }
    }
    else {
        while (peek_type() != CborType::SIMPLE) {
            CborType chunk_type;
            uint8_t chunk_length_value;
            read_cbor_type(chunk_type, chunk_length_value);
            if (chunk_type != cbor_type) {
                throw CdnsDecoderException(("Different chunk major type inside indefinite length string: " +
                                            std::to_string(static_cast<uint8_t>(chunk_type) >> 5)).c_str());
            }
            else if (chunk_length_value == 31) {
                throw CdnsDecoderException("Indefinite length chunk inside indefinite length string");
            }

            uint64_t chunk_length = read_int(chunk_length_value);
            ret.reserve(ret.size() + chunk_length);
            for (int i = 0; i < chunk_length; i++) {
                read_to_buffer();
                ret.push_back(m_p[0]);
                m_p++;
            }
        }

        read_break();
    }

    return ret;
}

void CDNS::CdnsDecoder::read_to_buffer()
{
    if (m_p == m_end) {
        if (m_input.eof())
            throw CdnsDecoderEnd("End of input stream");

        m_input.read(reinterpret_cast<char*>(m_buffer), BUFFER_SIZE);
        m_p = m_buffer;
        m_end = m_buffer + m_input.gcount();
    }
}