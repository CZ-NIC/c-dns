#pragma once

#include <cbor.h>

#include "format_specification.h"
#include "block.h"
#include "file_preamble.h"
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

        static constexpr std::size_t BUFFER_SIZE = UINT16_MAX;
        /**
         * @brief Construct a new CdnsEncoder object
         * @param file_preamble Filled out C-DNS file preamble
         * @param output_name Name of the output C-DNS file
         * @param compression Type of compression for the output C-DNS file
         * @throw CborEncoderException when constructor fails
         */
        CdnsEncoder(FilePreamble& file_preamble, std::string& output_name, CborOutputCompression compression);

        /**
         * @brief Destroy the CdnsEncoder object and properly close the output C-DNS file
         */
        ~CdnsEncoder() { if(m_cos) delete m_cos; }

        /**
         * @brief Buffer new DNS record to C-DNS block
         * @param qr New DNS record to buffer
         * @return `true` if DNS record was successfully buffered, `false` otherwise
         */
        bool buffer_qr(const GenericQueryResponse& qr) {
            return m_block.add_question_response_record(qr, m_file_preamble.m_block_parameters[m_block.get_block_parameters_index()]);
        }

        /**
         * @todo
         * bool buffer_aec(generic_aec& aec);
         * bool buffer_mm(generic_mm& mm);
         */

        /**
         * @brief Write start of CBOR array
         * @param size Number of items in the array
         */
        void write_array_start(std::size_t size);

        /**
         * @brief Write start of CBOR array of indefinite length
         */
        void write_indef_array_start();

        /**
         * @brief Write start of CBOR map
         * @param size Number of key:value pairs in the map
         */
        void write_map_start(std::size_t size);

        /**
         * @brief Write start of CBOR map of indefinite length
         */
        void write_indef_map_start();

        /**
         * @brief Write CBOR Byte string
         * @param char Pointer to start of the byte string
         * @param size Size of the byte string in bytes
         */
        void write_bytestring(const unsigned char* str, std::size_t size);

        /**
         * @brief Write CBOR Byte string
         * @param str Byte string to write
         */
        void write_bytestring(const std::string& str) {
            write_bytestring(reinterpret_cast<const unsigned char*>(str.c_str()), str.size());
        }

        /**
         * @brief Write CBOR Text string
         * @param char Pointer to start of the text string
         * @param size Size of the text string in bytes
         */
        void write_textstring(const unsigned char* str, std::size_t size);

        /**
         * @brief Write CBOR Text string
         * @param str Text string to write
         */
        void write_textstring(const std::string& str) {
            write_textstring(reinterpret_cast<const unsigned char*>(str.c_str()), str.size());
        }

        /**
         * @brief Write CBOR Break stop code to close indefinite length arrays and maps
         */
        void write_break();

        /**
         * @brief Write bool value to CBOR
         * @param value Bool value to write
         */
        void write(bool value);

        /**
         * @brief Write uint8_t value to CBOR
         * @param value Uint8_t value to write
         */
        void write(uint8_t value);

        /**
         * @brief Write uint16_t value to CBOR
         * @param value Uint16_t value to write
         */
        void write(uint16_t value);

        /**
         * @brief Write uint32_t value to CBOR
         * @param value Uint32_t value to write
         */
        void write(uint32_t value);

        /**
         * @brief Write uint64_t value to CBOR
         * @param value Uint64_t value to write
         */
        void write(uint64_t value);

        /**
         * @brief Write int8_t value to CBOR
         * @param value Int8_t value to write
         */
        void write(int8_t value);

        /**
         * @brief Write int16_t value to CBOR
         * @param value Int16_t value to write
         */
        void write(int16_t value);

        /**
         * @brief Write int32_t value to CBOR
         * @param value Int32_t value to write
         */
        void write(int32_t value);

        /**
         * @brief Write int64_t value to CBOR
         * @param value In64_t value to write
         */
        void write(int64_t value);

        /**
         * @todo
         * void write_file_start();
         * void write_file_preamble();
         * void write_block();
         * void open_file();
         * void close_file();
         * void rotate_file(const std::string& new_output_name);
         */

        private:
        /**
         * @brief Write contents of internal buffer to ouptut C-DNS file
         */
        void flush_buffer();

        /**
         * @brief Update internal buffer parameters
         * @param bytes Number of bytes written to internal buffer
         */
        void update_buffer(std::size_t bytes) {
            m_p += bytes;
            m_avail -= bytes;
            m_bytes_written += bytes;
        }

        FilePreamble m_file_preamble;
        CdnsBlock m_block;
        BaseCborOutputWriter* m_cos;
        unsigned char m_buffer[BUFFER_SIZE];
        unsigned char *m_p;
        std::size_t m_avail;
        std::size_t m_bytes_written;
        //Timestamp m_start_time;
    };
}