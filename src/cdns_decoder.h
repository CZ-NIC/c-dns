/**
 * Copyright © 2020 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <istream>
#include <stdexcept>
#include <functional>

#include "format_specification.h"

namespace CDNS {

    /**
     * @brief Exception thrown if there's some issue with decoding C-DNS data
     */
    class CdnsDecoderException : public std::runtime_error {
        public:
        explicit CdnsDecoderException(const char* msg) : std::runtime_error(msg) {}
        explicit CdnsDecoderException(std::string& msg) : std::runtime_error(msg) {}
    };

    /**
     * @brief Exception thrown when the input stream reaches its end
     */
    class CdnsDecoderEnd : public std::runtime_error {
        public:
        explicit CdnsDecoderEnd(const char* msg) : std::runtime_error(msg) {}
        explicit CdnsDecoderEnd(std::string& msg) : std::runtime_error(msg) {}
    };

    /**
     * @brief Decodes input stream of CBOR data.
     */
    class CdnsDecoder {
        public:

        static constexpr std::size_t BUFFER_SIZE = 65535;

        /**
         * @brief Construct a new CdnsDecoder object
         * @param input Valid input stream to read C-DNS data from
         * @throw CdnsDecoderException if the input stream isn't valid
         */
        CdnsDecoder(std::istream& input) : m_input(input) {
            m_p = m_end = m_buffer;
            if (input.bad())
                throw CdnsDecoderException("Bad input stream");
        }

        /**
         * @brief Look up CBOR major type of the next item in input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @return CBOR major type of the next item in input stream
         */
        CborType peek_type();

        /**
         * @brief Read an unsigned integer item from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Unsigned integer read from input stream
         */
        uint64_t read_unsigned();

        /**
         * @brief Read a negative integer item from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Negative integer read from input stream
         */
        int64_t read_negative();

        /**
         * @brief Read unsigned or negative integer from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Integer read from input stream
         */
        int64_t read_integer();

        /**
         * @brief Read a bool item from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Bool value read from input stream
         */
        bool read_bool();

        /**
         * @brief Read a byte string item from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return A byte string read from input stream
         */
        std::string read_bytestring();

        /**
         * @brief Read a text string item from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return A text string read from input stream
         */
        std::string read_textstring();

        /**
         * @brief Read a start of an array from input stream
         * @param indef Set by this method to TRUE if the array start read from input stream is
         * of indefinite length, FALSE otherwise
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Number of items in the array read from input stream (0 if array is of
         * indefinite length, check indef parameter value)
         */
        uint64_t read_array_start(bool& indef);

        /**
         * @brief Read an array from input stream
         * @param cb Callback that has to contain the reading of one item from array. It takes one parameter
         * which is a reference to the CdnsDecoder itself.
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         */
        void read_array(std::function<void(CdnsDecoder&)> cb) {
            bool indef = false;
            uint64_t length = read_array_start(indef);

            while (length > 0 || indef) {
                if (indef && peek_type() == CborType::BREAK) {
                    read_break();
                    break;
                }

                cb(*this); // reading of one item of the array has to happen here
                length--;
            }
        }

        /**
         * @brief Read a start of a map from input stream
         * @param indef Set by this method to TRUE if the map start read from input stream is
         * of indefinite length, FALSE otherwise
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return Number of key/value pairs in the map read from input stream (0 if map is
         * of indefinite length, check indef parameter value)
         */
        uint64_t read_map_start(bool& indef);

        /**
         * @brief Read a "break" stop code from input stream
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         */
        void read_break();

        /**
         * @brief Skip over the next item in input stream no matter what it is (skips over entire
         * array or map if that is the next item in input stream)
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         */
        void skip_item();

        private:

        /**
         * @brief Read the first byte of the next item in input stream. This byte contains item's
         * CBOR major type (high-order 3 bits) and additional information (low-order 5 bits).
         * @param cbor_type Set by this method to item's CBOR major type
         * @param additional Set by this method to item's additional information
         * @throw CdnsDecoderEnd if the end of input stream is reached
         */
        void read_cbor_type(CborType& cbor_type, uint8_t& additional);

        /**
         * @brief Read an unsigned integer from input stream
         * @param item_length Length of the integer (low-order 5 bits of additional information from
         * item's first byte)
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @return Unsigned integer read from input stream
         */
        uint64_t read_int(uint8_t item_length);

        /**
         * @brief Read string from the input stream
         * @param cbor_type CborType::BYTE_STRING or CborType::TEXT_STRING
         * @param length Length of the string to read
         * @param indef TRUE if it's indefinite length string, FALSE otherwise
         * @throw CdnsDecoderEnd if the end of input stream is reached
         * @throw CdnsDecoderException if an error is encountered decoding CBOR data
         * @return String read from the input stream
         */
        std::string read_string(CborType cbor_type, uint64_t length, bool indef);

        /**
         * @brief Read more data from input stream to decoder's buffer
         * @throw CdnsDecoderEnd if the end of input stream is reached
         */
        void read_to_buffer();

        std::istream& m_input;
        unsigned char m_buffer[BUFFER_SIZE];
        unsigned char* m_p;
        unsigned char* m_end;
    };
}