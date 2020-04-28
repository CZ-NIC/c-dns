/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <cstring>
#include <cstdint>
#include <stdexcept>

#include "writer.h"

namespace CDNS {

    /**
     * @brief Exception thrown if there's some issue with encoding data to C-DNS
     */
    class CdnsEncoderException : public std::runtime_error {
        public:
        explicit CdnsEncoderException(const char* msg) : std::runtime_error(msg) {}
        explicit CdnsEncoderException(std::string& msg) : std::runtime_error(msg) {}
    };

    /**
     * @brief Main C-DNS library class. Buffers DNS records, Address events and Malformed messages
     * to internal C-DNS memory representation and then writes them to output file.
     */
    class CdnsEncoder {
        public:

        static constexpr std::size_t BUFFER_SIZE = 2048;

        /**
         * @enum CborTypes
         * @brief Cbor data types (highest 3 bits of type byte)
         */
        enum class CborTypes : uint8_t {
            UNSIGNED = 0x00,
            NEGATIVE = 0x20,
            BYTE_STRING = 0x40,
            TEXT_STRING = 0x60,
            ARRAY = 0x80,
            MAP = 0xA0,
            TAG = 0xC0,
            FLOAT = 0xE0,
            SIMPLE = 0xE0
        };

        /**
         * @brief Construct a new CdnsEncoder object
         * @param output File name or valid file descriptor to output C-DNS data
         * @param compression Type of compression for the output C-DNS data
         * @throw CborEncoderException if constructor fails
         * @throw CborOutputException if output initialization fails
         */
        template<typename T>
        CdnsEncoder(const T& output, CborOutputCompression compression) : m_p(m_buffer),
                                                                          m_avail(BUFFER_SIZE) {
            switch (compression) {
                case CborOutputCompression::NO_COMPRESSION:
                    m_cos = new CborOutputWriter(output);
                    break;
                case CborOutputCompression::GZIP:
                    m_cos = new GzipCborOutputWriter(output);
                    break;
                case CborOutputCompression::XZ:
                    m_cos = new XzCborOutputWriter(output);
                    break;
                default:
                    throw CdnsEncoderException("Unknown type of compression");
                    break;
            }

            std::memset(m_buffer, 0, sizeof(m_buffer));
        }

        /**
         * @brief Destroy the CdnsEncoder object and properly close the C-DNS output
         */
        ~CdnsEncoder() {
            flush_buffer();
            if(m_cos)
                delete m_cos;
        }

        /** Delete [move] copy constructors and assignment operators */
        CdnsEncoder(CdnsEncoder& copy) = delete;
        CdnsEncoder(CdnsEncoder&& copy) = delete;
        CdnsEncoder& operator=(CdnsEncoder& rhs) = delete;
        CdnsEncoder& operator=(CdnsEncoder&& rhs) = delete;

        /**
         * @brief Write start of CBOR array
         * @param size Number of items in the array
         * @return Number of uncompressed bytes written
         */
        std::size_t write_array_start(std::size_t size);

        /**
         * @brief Write start of CBOR array of indefinite length
         * @return Number of uncompressed bytes written
         */
        std::size_t write_indef_array_start();

        /**
         * @brief Write start of CBOR map
         * @param size Number of key:value pairs in the map
         * @return Number of uncompressed bytes written
         */
        std::size_t write_map_start(std::size_t size);

        /**
         * @brief Write start of CBOR map of indefinite length
         * @return Number of uncompressed bytes written
         */
        std::size_t write_indef_map_start();

        /**
         * @brief Write CBOR Byte string
         * @param char Pointer to start of the byte string
         * @param size Size of the byte string in bytes
         * @return Number of uncompressed bytes written
         */
        std::size_t write_bytestring(const unsigned char* str, std::size_t size);

        /**
         * @brief Write CBOR Byte string
         * @param str Byte string to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write_bytestring(const std::string& str) {
            return write_bytestring(reinterpret_cast<const unsigned char*>(str.data()), str.size());
        }

        /**
         * @brief Write CBOR Text string
         * @param char Pointer to start of the text string
         * @param size Size of the text string in bytes
         * @return Number of uncompressed bytes written
         */
        std::size_t write_textstring(const unsigned char* str, std::size_t size);

        /**
         * @brief Write CBOR Text string
         * @param str Text string to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write_textstring(const std::string& str) {
            return write_textstring(reinterpret_cast<const unsigned char*>(str.data()), str.size());
        }

        /**
         * @brief Write CBOR Break stop code to close indefinite length arrays and maps
         * @return Number of uncompressed bytes written
         */
        std::size_t write_break();

        /**
         * @brief Write bool value to CBOR
         * @param value Bool value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(bool value);

        /**
         * @brief Write uint8_t value to CBOR
         * @param value Uint8_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(uint8_t value);

        /**
         * @brief Write uint16_t value to CBOR
         * @param value Uint16_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(uint16_t value);

        /**
         * @brief Write uint32_t value to CBOR
         * @param value Uint32_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(uint32_t value);

        /**
         * @brief Write uint64_t value to CBOR
         * @param value Uint64_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(uint64_t value);

        /**
         * @brief Write int8_t value to CBOR
         * @param value Int8_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(int8_t value);

        /**
         * @brief Write int16_t value to CBOR
         * @param value Int16_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(int16_t value);

        /**
         * @brief Write int32_t value to CBOR
         * @param value Int32_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(int32_t value);

        /**
         * @brief Write int64_t value to CBOR
         * @param value In64_t value to write
         * @return Number of uncompressed bytes written
         */
        std::size_t write(int64_t value);

        /**
         * @brief Close the current output and open a new one with given file name or file descriptor
         * @param out New output to open (file name[std::string] or file descriptor[int])
         */
        template<typename T>
        void rotate_output(const T& out) {
            flush_buffer();
            m_cos->rotate_output(out);
        }

        private:
        /**
         * @brief Write contents of internal buffer to ouptut C-DNS file
         */
        void flush_buffer();

        /**
         * @brief Write integer in CBOR to output buffer
         * @param value Value to write to the buffer
         * @param major CBOR data type of the value. Doesn't have to be just unsigned or negative number.
         * It can also be a map, array, string. etc. and the value is its length.
         * @return Number of uncompressed bytes written
         */
        std::size_t write_int(uint64_t value, CborTypes major);

        /**
         * @brief Write string to CBOR
         * @param str Pointer to start of the string
         * @param size Size of the string in bytes
         */
        void write_string(const unsigned char* str, std::size_t size);

        /**
         * @brief Update internal buffer parameters
         * @param bytes Number of bytes written to internal buffer
         */
        void update_buffer(std::size_t bytes) {
            m_p += bytes;
            m_avail -= bytes;
        }

        BaseCborOutputWriter* m_cos;
        unsigned char m_buffer[BUFFER_SIZE];
        unsigned char *m_p;
        std::size_t m_avail;
    };
}