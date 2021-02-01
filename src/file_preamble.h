/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <boost/optional.hpp>

#include "format_specification.h"
#include "dns.h"

namespace CDNS {
    class CdnsEncoder;
    class CdnsDecoder;

    static constexpr uint8_t VERSION_MAJOR = 1;
    static constexpr uint8_t VERSION_MINOR = 0;
    static constexpr uint8_t VERSION_PRIVATE = 1;

    static constexpr uint64_t DEFAULT_TICKS_PER_SECOND = 1000000;
    static constexpr uint64_t DEFAULT_MAX_BLOCK_ITEMS = 10000;

    static constexpr uint32_t DEFAULT_QR_HINTS = QueryResponseHintsMask::time_offset |
                                                 QueryResponseHintsMask::client_address_index |
                                                 QueryResponseHintsMask::client_port |
                                                 QueryResponseHintsMask::transaction_id |
                                                 QueryResponseHintsMask::qr_signature_index |
                                                 QueryResponseHintsMask::client_hoplimit |
                                                 QueryResponseHintsMask::response_delay |
                                                 QueryResponseHintsMask::query_name_index |
                                                 QueryResponseHintsMask::query_size |
                                                 QueryResponseHintsMask::response_size |
                                                 QueryResponseHintsMask::response_processing_data |
                                                 QueryResponseHintsMask::query_question_sections |
                                                 QueryResponseHintsMask::query_answer_sections |
                                                 QueryResponseHintsMask::query_authority_sections |
                                                 QueryResponseHintsMask::query_additional_sections |
                                                 QueryResponseHintsMask::response_answer_sections |
                                                 QueryResponseHintsMask::response_authority_sections |
                                                 QueryResponseHintsMask::response_additional_sections;

    static constexpr uint32_t DEFAULT_QR_SIG_HINTS = QueryResponseSignatureHintsMask::server_address_index |
                                                     QueryResponseSignatureHintsMask::server_port |
                                                     QueryResponseSignatureHintsMask::qr_transport_flags |
                                                     QueryResponseSignatureHintsMask::qr_type |
                                                     QueryResponseSignatureHintsMask::qr_sig_flags |
                                                     QueryResponseSignatureHintsMask::query_opcode |
                                                     QueryResponseSignatureHintsMask::qr_dns_flags |
                                                     QueryResponseSignatureHintsMask::query_rcode |
                                                     QueryResponseSignatureHintsMask::query_classtype_index |
                                                     QueryResponseSignatureHintsMask::query_qdcount |
                                                     QueryResponseSignatureHintsMask::query_ancount |
                                                     QueryResponseSignatureHintsMask::query_nscount |
                                                     QueryResponseSignatureHintsMask::query_arcount |
                                                     QueryResponseSignatureHintsMask::query_edns_version |
                                                     QueryResponseSignatureHintsMask::query_udp_size |
                                                     QueryResponseSignatureHintsMask::query_opt_rdata_index |
                                                     QueryResponseSignatureHintsMask::response_rcode;

    static constexpr uint8_t DEFAULT_RR_HINTS = RrHintsMask::ttl |
                                                RrHintsMask::rdata_index;

    static constexpr uint8_t DEFAULT_OTHER_DATA_HINTS = OtherDataHintsMask::malformed_messages |
                                                        OtherDataHintsMask::address_event_counts;

    /**
     * @brief Storage Hints structure
     */
    struct StorageHints {
        StorageHints() : query_response_hints(DEFAULT_QR_HINTS),
                         query_response_signature_hints(DEFAULT_QR_SIG_HINTS),
                         rr_hints(DEFAULT_RR_HINTS),
                         other_data_hints(DEFAULT_OTHER_DATA_HINTS) {}
        /**
         * @brief Serialize the Storage hints to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Read the StorageHints from C-DNS CBOR input stream
         * @param dec C-DNS decoder
         */
        void read(CdnsDecoder& dec);

        /**
         * @brief Reset StorageHints to default values.
         * Is applied in every call of read() method.
         */
        void reset();

        uint32_t query_response_hints;
        uint32_t query_response_signature_hints;
        uint8_t rr_hints;
        uint8_t other_data_hints;
    };

    /**
     * @brief Storage Parameters structure
     */
    struct StorageParameters {
        StorageParameters() : ticks_per_second(DEFAULT_TICKS_PER_SECOND),
                              max_block_items(DEFAULT_MAX_BLOCK_ITEMS), storage_hints(),
                              opcodes(OpCodesDefault), rr_types(RrTypesDefault) {}
        /**
         * @brief Serialize the Storage parameters to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Read the StorageParameters from C-DNS CBOR input stream
         * @param dec C-DNS decoder
         */
        void read(CdnsDecoder& dec);

        /**
         * @brief Reset StorageParameters to default values.
         * Is applied in every call of read() method.
         */
        void reset();

        uint64_t ticks_per_second;
        uint64_t max_block_items;
        StorageHints storage_hints;
        std::vector<OpCodes> opcodes;
        std::vector<RrTypes> rr_types;
        boost::optional<StorageFlagsMask> storage_flags;
        boost::optional<uint8_t> client_address_prefix_ipv4;
        boost::optional<uint8_t> client_address_prefix_ipv6;
        boost::optional<uint8_t> server_address_prefix_ipv4;
        boost::optional<uint8_t> server_address_prefix_ipv6;
        boost::optional<std::string> sampling_method;
        boost::optional<std::string> anonymization_method;
    };

    /**
     * @brief Collection Parameters structure
     */
    struct CollectionParameters {
        CollectionParameters() : interfaces(), server_address(), vlan_ids() {}

        /**
         * @brief Serialize the Collection parameters to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Read the CollectionParameters from C-DNS CBOR input stream
         * @param dec C-DNS decoder
         */
        void read(CdnsDecoder& dec);

        /**
         * @brief Reset CollectionParameters to default values.
         * Is applied in every call of read() method.
         */
        void reset();

        boost::optional<uint64_t> query_timeout;
        boost::optional<uint64_t> skew_timeout;
        boost::optional<uint64_t> snaplen;
        boost::optional<bool> promisc;
        std::vector<std::string> interfaces;
        std::vector<std::string> server_address;
        std::vector<uint16_t> vlan_ids;
        boost::optional<std::string> filter;
        boost::optional<std::string> generator_id;
        boost::optional<std::string> host_id;
    };

    /**
     * @brief Block Parameters structure
     */
    struct BlockParameters {
        BlockParameters() : storage_parameters() {}

        /**
         * @brief Serialize the Block parameters to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Read the BlockParameters from C-DNS CBOR input stream
         * @param dec C-DNS decoder
         */
        void read(CdnsDecoder& dec);

        /**
         * @brief Reset BlockParameters to default values.
         * Is applied in every call of read() method.
         */
        void reset();

        StorageParameters storage_parameters;
        boost::optional<CollectionParameters> collection_parameters;
    };

    /**
     * @brief Class representing C-DNS file preamble
     */
    class FilePreamble {
        public:

        /**
         * @brief Default constructor
         */
        FilePreamble() : m_block_parameters({BlockParameters()}) {}

        /**
         * @brief Construct a new FilePreamble object with already filled Block parameters
         * @param bps Vector of Block parameters to be used in the C-DNS file
         * @param private_version Optional private version of C-DNS file standard
         */
        FilePreamble(std::vector<BlockParameters>& bps, boost::optional<uint8_t> private_version = VERSION_PRIVATE)
            : m_block_parameters(bps) {}

        /**
         * @brief Add new filled Block parameters to File preamble
         * @param bp New Block parameters to add
         * @return Index of the newly inserted Block parameters in the Block parameters vector in File preamble
         */
        index_t add_block_parameters(BlockParameters& bp) {
            m_block_parameters.push_back(bp);
            return m_block_parameters.size() - 1;
        }

        /**
         * @brief Get the number of items in Block parameters vector in File preamble
         * @return Number of items in Block parameters vector
         */
        std::size_t block_parameters_size() const {
            return m_block_parameters.size();
        }

        /**
         * @brief Get Block parameters from File preamble based on the index to Block parameters vector
         * @param index Index of the Block parameters to find
         * @throw std::runtime_error if given index is out of bounds of the Block parameter vector
         * @return Block parameters under the given index
         */
        BlockParameters& get_block_parameters(index_t index) {
            if (index < m_block_parameters.size())
                return m_block_parameters[index];

            throw std::runtime_error("Block parameters index out of range.");
        }

        /**
         * @brief Serialize the File preamble to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Read the FilePreamble from C-DNS CBOR input stream
         * @param dec C-DNS decoder
         */
        void read(CdnsDecoder& dec);

        /**
         * @brief Reset FilePreamble to default values.
         * Is applied in every call of read() method.
         */
        void reset();

        uint8_t m_major_format_version = VERSION_MAJOR;
        uint8_t m_minor_format_version = VERSION_MINOR;
        boost::optional<uint8_t> m_private_version = VERSION_PRIVATE;
        std::vector<BlockParameters> m_block_parameters;
    };
}