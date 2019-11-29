#pragma once

#define _DEFAULT_SOURCE

#include <cbor.h>
#include <time.h>
#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "format_specification.h"
#include "dns.h"
#include "block_table.h"
#include "file_preamble.h"
#include "block.h"
#include "hash.h"
#include "interface.h"
#include "timestamp.h"
#include "writer.h"
#include "cdns_encoder.h"

namespace CDNS {
    /* Other */

    typedef enum
    {
        E_SUCCESS = 0,
        E_ERROR   = -1
    } cdns_ret_t;

    /* Functions */

    int cdns_push(CdnsBlock& ctx, const QueryResponse& qr);

    int cdns_serialize_file_preamble(const FilePreamble& ctx, unsigned char** buff, size_t* buff_size);

    int cdns_serialize_block(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size);

    int cdns_serialize_end(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size);

    class CdnsExporter {
        public:
        /**
         * @brief Construct a new CdnsExporter object to output C-DNS data
         * @param fp Filled C-DNS File preamble with file parameters
         * @param out C-DNS output to open (file name[std::string] or file descriptor[int])
         * @param compression Type of compression for the output C-DNS data
         */
        template<typename T>
        CdnsExporter(FilePreamble& fp, const T& out, CborOutputCompression compression)
            : m_file_preamble(fp), m_block(fp.get_block_parameters(0), 0), m_encoder(out, compression),
              m_blocks_written(0) {}

        /**
         * @brief Buffer new DNS record to C-DNS block
         * @param qr New DNS record to buffer
         * @throw std::exception if inserting DNS record to the Block fails
         * @return `true` if full Block was written to output, `false` otherwise
         */
        bool buffer_qr(const GenericQueryResponse& qr) {
            if (m_block.add_question_response_record(qr)) {
                write_block();
                return true;
            }

            return false;
        }

         /**
         * @todo
         * bool buffer_aec(generic_aec& aec);
         * bool buffer_mm(generic_mm& mm);
         */

        /**
         * @todo
         * Change Block parameters for different Blocks
         * Only when current internal Block is empty -> immediately after writing internal Block to output
         */

        /**
         * @brief Write the given C-DNS block to output
         * @param block C-DNS block to output
         */
        void write_block(CdnsBlock& block);

        /**
         * @brief Write the internally buffered C-DNS block to output
         */
        void write_block() {
            write_block(m_block);
            m_block.clear();
        }

        /**
         * @brief Close the current output and open a new one with given file name or file descriptor
         * @param out New output to open (file name[std::string] or file descriptor[int])
         * @param export_current_block If `true` currently internally buffered Block will be exported
         * before current output is closed
         * @throw CborOutputException if output rotation fails
         */
        template<typename T>
        void rotate_output(const T& out, bool export_current_block) {
            if (export_current_block)
                write_block();

            if (m_blocks_written > 0)
                m_encoder.write_break();

            m_encoder.rotate_output(out);
            m_blocks_written = 0;
        }

        private:
        /**
         * @brief Writes beginning of C-DNS file (File type ID, File preamble and start of File blocks array)
         */
        void write_file_header();

        FilePreamble m_file_preamble;
        CdnsBlock m_block;
        CdnsEncoder m_encoder;

        /**
         * @brief Number of Blocks written to the currently open output (gets reset on output rotation)
         */
        std::size_t m_blocks_written;
    };
}
