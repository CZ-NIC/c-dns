/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <time.h>
#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <istream>
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
#include "cdns_decoder.h"

namespace CDNS {

    /**
     * @brief Class serving as C-DNS library's main interface for writing C-DNS to output
     *
     * CdnsExporter is initialized with filled File preamble structure, output destination
     * and output compression option. User only needs to call buffer_*() methods, which
     * buffer records, events and malformed messages to C-DNS block and when the block is full,
     * it automatically gets written to output.
     *
     * To change the output file or file descriptor user can call rotate_output() method.
     *
     * To enforce writing of not fully buffered block to output write_block() method is provided.
     * This method can also write to output an externally created C-DNS block. (WARNING: External
     * blocks aren't checked against CdnsExporter's Block parameters. This is up to the user!!!)
     */
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
              m_active_block_parameters(0), m_blocks_written(0) {}

        /**
         * @brief Destroy the CdnsExporter object and write the end of C-DNS output
         * if any output is currently open
         */
        ~CdnsExporter() {
            if (m_blocks_written > 0)
                m_encoder.write_break();
        }

        /** Delete [move] copy constructors and assignment operators */
        CdnsExporter(CdnsExporter& copy) = delete;
        CdnsExporter(CdnsExporter&& copy) = delete;
        CdnsExporter& operator=(CdnsExporter& rhs) = delete;
        CdnsExporter& operator=(CdnsExporter&& rhs) = delete;

        /**
         * @brief Buffer new DNS record to C-DNS block
         * @param qr New DNS record to buffer
         * @param stats Current Block statistics (It's user's responsibility to count statistics and update them
         * in the Block. User also has to start counting statistics from 0 again if new Block is started -> method
         * returns non-0 value)
         * @throw std::exception if inserting DNS record to the Block fails
         * @return Number of uncompressed bytes written if full Block was written to output, 0 otherwise
         */
        std::size_t buffer_qr(const GenericQueryResponse& qr, const boost::optional<BlockStatistics>& stats = boost::none) {
            std::size_t written = 0;
            if (m_block.add_question_response_record(qr, stats))
                written = write_block();

            return written;
        }

        /**
         * @brief Buffer new Address Event to C-DNS block
         * @param aec New Address Event to buffer
         * @param stats Current Block statistics (It's user's responsibility to count statistics and update them
         * in the Block. User also has to start counting statistics from 0 again if new Block is started -> method
         * returns non-0 value)
         * @throw std::exception if inserting Address Event to the Block fails
         * @return Number of uncompressed bytes written if full Block was written to output, 'false' otherwise
         */
        std::size_t buffer_aec(const GenericAddressEventCount& aec, const boost::optional<BlockStatistics>& stats = boost::none) {
            std::size_t written = 0;
            if (m_block.add_address_event_count(aec, stats))
                written = write_block();

            return written;
        }

        /**
         * @brief Buffer new Malformed message to C-DNS block
         * @param mm New Malformed message to buffer
         * @param stats Current Block statistics (It's user's responsibility to count statistics and update them
         * in the Block. User also has to start counting statistics from 0 again if new Block is started -> method
         * returns non-0 value)
         * @throw std::exception if inserting Malformed message to the Block fails
         * @return Number of uncompressed bytes written if full Block was written to output, 0 otherwise
         */
        std::size_t buffer_mm(const GenericMalformedMessage& mm, const boost::optional<BlockStatistics>& stats = boost::none) {
            std::size_t written = 0;
            if (m_block.add_malformed_message(mm, stats))
                written = write_block();

            return written;
        }

        /**
         * @brief Write the given C-DNS block to output
         * @param block C-DNS block to output
         * @return Number of uncompressed bytes written
         */
        std::size_t write_block(CdnsBlock& block);

        /**
         * @brief Write the internally buffered C-DNS block to output
         * @return Number of uncompressed bytes written
         */
        std::size_t write_block() {
            std::size_t written = write_block(m_block);
            m_block.clear();
            m_block.set_block_parameters(m_file_preamble.get_block_parameters(m_active_block_parameters),
                                         m_active_block_parameters);
            return written;
        }

        /**
         * @brief Close the current output and open a new one with given file name or file descriptor
         * @param out New output to open (file name[std::string] or file descriptor[int])
         * @param export_current_block If `true` currently internally buffered Block will be exported
         * before current output is closed
         * @throw CborOutputException if output rotation fails
         * @return Number of uncompressed bytes written to close current output, 0 if closing empty output
         */
        template<typename T>
        std::size_t rotate_output(const T& out, bool export_current_block) {
            std::size_t written = 0;
            if (export_current_block)
                written += write_block();

            if (m_blocks_written > 0)
                written += m_encoder.write_break();

            m_encoder.rotate_output(out);
            m_blocks_written = 0;
            return written;
        }

        /**
         * @brief Get the number of items in currently buffered Block
         *
         * Can be up to 3 x <max_block_items>, because <max_block_items> affects each array in Block
         * individually.
         *
         * @return Number of items in currently buffered Block
         */
        std::size_t get_block_item_count() const {
            return m_block.get_item_count();
        }

        /**
         * @brief Get the number of QueryResponse items in currently buffered Block
         * @return Number of QueryResponse items in currently buffered Block
         */
        std::size_t get_block_qr_count() const {
            return m_block.get_qr_count();
        }

        /**
         * @brief Get the number of AddressEventCount items in currently buffered Block
         * @return Number of AddressEventCount items in currently buffered Block
         */
        std::size_t get_block_aec_count() const {
            return m_block.get_aec_count();
        }

        /**
         * @brief Get the number of MalformedMessage items in currently buffered Block
         * @return Number of MalformedMessage items in currently buffered Block
         */
        std::size_t get_block_mm_count() const {
            return m_block.get_mm_count();
        }

        /**
         * @brief Get the number of Blocks written to the current output file or file descriptor
         * @return Number of Blocks written to the current output file or file descriptor
         */
        std::size_t get_blocks_written_count() const {
            return m_blocks_written;
        }

        /**
         * @brief Add another Block parameters to File preamble
         *
         * If some Blocks were already written to current output (call get_blocks_written_count()
         * to find out), the new Block parameters will be written only to the next rotated output.
         * To enforce the use of these new Block parameters call rotate_output() method.
         *
         * @param bp New Block parameters to add
         * @return Index of the new Block parameters in File preamble array of Block parameters
         */
        index_t add_block_parameters(BlockParameters& bp) {
            return m_file_preamble.add_block_parameters(bp);
        }

        /**
         * @brief Set Block parameters used for next internally buffered CDNS blocks (doesn't
         * affect currently buffered Block)
         *
         * User can enforce using these Block parameters immediately by calling write_block() right
         * after calling this method. This writes currently buffered Block to output and starts a new
         * Block that will use these new Block parameters.
         *
         * @param index Index to File preamble's array of Block parameters
         * @return 'true' if Block parameters are successfuly set, 'false' if given index is out of
         * bounds for File preamble's array of Block parameters
         */
        bool set_active_block_parameters(index_t index) {
            if (index >= m_file_preamble.block_parameters_size())
                return false;

            m_active_block_parameters = index;
            return true;
        }

        /**
         * @brief Get index of the active Block parameters in File Preamble's array of Block parameters
         * @return Index of active Block parameters
         */
        index_t get_active_block_parameters() const {
            return m_active_block_parameters;
        }

        /**
         * @brief Get the active Block parameters in File Preamble's array of Block parameters
         * @return Active Block parameters
         */
        BlockParameters& get_active_block_parameters_ref() {
            return m_file_preamble.get_block_parameters(m_active_block_parameters);
        }

        private:
        /**
         * @brief Writes beginning of C-DNS file (File type ID, File preamble and start of File blocks array)
         * @return Number of uncompressed bytes written
         */
        std::size_t write_file_header();

        FilePreamble m_file_preamble;
        CdnsBlock m_block;
        CdnsEncoder m_encoder;
        index_t m_active_block_parameters;

        /**
         * @brief Number of Blocks written to the currently open output (gets reset on output rotation)
         */
        std::size_t m_blocks_written;
    };

    /**
     * @brief Class serving as C-DNS library's main interface for reading C-DNS data from
     * input
     *
     * CdnsReader is initialized with valid input stream containing uncompressed C-DNS data.
     * CdnsReader constructor automatically reads the beginning of C-DNS file including its
     * file preamble. User then reads the input by Blocks with the read_block(bool& eof) method.
     * From these Blocks user can extract Query Response pairs and other data. When CdnsReader
     * reaches the end of C-DNS data it sets the "eof" parameter in read_block() method to TRUE.
     * CdnsBlockRead returned by this call is then empty.
     */
    class CdnsReader {
        public:
        /**
         * @brief Construct a new CdnsReader object to read uncompressed C-DNS data.
         * The constructor automatically reads the start of C-DNS file and filles
         * the m_file_preamble item.
         * @param input Valid input stream to read C-DNS data from
         */
        CdnsReader(std::istream& input) : m_file_preamble(),
                                          m_decoder(input),
                                          m_blocks_count(0),
                                          m_blocks_read(0),
                                          m_indef_blocks(false) { read_file_header(); }

        /**
         * @brief Read whole C-DNS Block from input stream
         * @param eof If set by this method to TRUE, then reader has reached the end
         * of C-DNS file and the returned C-DNS block is empty. Otherwise set to FALSE.
         * @return New C-DNS Block read from input stream
         */
        CdnsBlockRead read_block(bool& eof);

        FilePreamble m_file_preamble; //!< C-DNS file preamble

        private:
        /**
         * @brief Reads the start of C-DNS file including file preamble and
         * start of the block array from input stream
         */
        void read_file_header();

        CdnsDecoder m_decoder;
        uint64_t m_blocks_count;
        uint64_t m_blocks_read;
        bool m_indef_blocks;
    };
}
