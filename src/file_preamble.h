#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include "format_specification.h"
#include "dns.h"

namespace CDNS {
    class CdnsEncoder;

    static constexpr uint8_t VERSION_MAJOR = 1;
    static constexpr uint8_t VERSION_MINOR = 0;
    static constexpr uint8_t VERSION_PRIVATE = 0;

    static constexpr uint64_t DEFAULT_TICKS_PER_SECOND = 1000000;
    static constexpr uint64_t DEFAULT_MAX_BLOCK_ITEMS = 10000;

    /**
     * @brief Storage Hints structure
     */
    struct StorageHints {
        StorageHints() : query_response_hints(0), query_response_signature_hints(0),
                         rr_hints(0), other_data_hints(0) {}
        /**
         * @brief Serialize the Storage hints to C-DNS CBOR representation
         * @param enc C-DNS encoder
         */
        void write(CdnsEncoder& enc);

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
         */
        void write(CdnsEncoder& enc);

        uint64_t ticks_per_second;
        uint64_t max_block_items;
        StorageHints storage_hints;
        std::vector<OpCodes> opcodes;
        std::vector<RrTypes> rr_types;
        std::optional<StorageFlagsMask> storage_flags;
        std::optional<uint8_t> client_address_prefix_ipv4;
        std::optional<uint8_t> client_address_prefix_ipv6;
        std::optional<uint8_t> server_address_prefix_ipv4;
        std::optional<uint8_t> server_address_prefix_ipv6;
        std::optional<std::string> sampling_method;
        std::optional<std::string> anonymization_method;
    };

    /**
     * @brief Collection Parameters structure
     */
    struct CollectionParameters {
        CollectionParameters() : interfaces(), server_address(), vlan_ids() {}

        /**
         * @brief Serialize the Collection parameters to C-DNS CBOR representation
         * @param enc C-DNS encoder
         */
        void write(CdnsEncoder& enc);

        std::optional<uint64_t> query_timeout;
        std::optional<uint64_t> skew_timeout;
        std::optional<uint64_t> snaplen;
        std::optional<bool> promisc;
        std::vector<std::string> interfaces;
        std::vector<std::string> server_address;
        std::vector<uint16_t> vlan_ids;
        std::optional<std::string> filter;
        std::optional<std::string> generator_id;
        std::optional<std::string> host_id;
    };

    /**
     * @brief Block Parameters structure
     */
    struct BlockParameters {
        BlockParameters() : storage_parameters() {}

        /**
         * @brief Serialize the Block parameters to C-DNS CBOR representation
         * @param enc C-DNS encoder
         */
        void write(CdnsEncoder& enc);

        StorageParameters storage_parameters;
        std::optional<CollectionParameters> collection_parameters;
    };

    /**
     * @brief Class representing C-DNS file preamble
     */
    class FilePreamble {
        public:

        /**
         * @brief Default constructor
         */
        FilePreamble() : m_block_parameters() {}

        /**
         * @brief Construct a new FilePreamble object with already filled Block parameters
         * @param bps Vector of Block parameters to be used in the C-DNS file
         * @param private_version Optional private version of C-DNS file standard
         */
        FilePreamble(std::vector<BlockParameters>& bps, std::optional<uint8_t> private_version = std::nullopt)
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
         */
        void write(CdnsEncoder& enc);

        uint8_t m_major_format_version = VERSION_MAJOR;
        uint8_t m_minor_format_version = VERSION_MINOR;
        std::optional<uint8_t> m_private_version;
        std::vector<BlockParameters> m_block_parameters;
    };
}