#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "format_specification.h"
#include "dns.h"

namespace CDNS {

    static constexpr uint8_t VERSION_MAJOR = 1;
    static constexpr uint8_t VERSION_MINOR = 0;
    static constexpr uint8_t VERSION_PRIVATE = 0;

    /**
     * @brief Storage Hints structure
     */
    struct StorageHints {
        /*QueryResponseHintsMask*/ uint32_t query_response_hints;
        /*QueryResponseSignatureHintsMask*/ uint32_t query_response_signature_hints;
        /*RrHintsMask*/ uint8_t rr_hints;
        /*OtherDataHintsMask*/ uint8_t other_data_hints;
    };

    /**
     * @brief Storage Parameters structure
     */
    struct StorageParameters {
        uint64_t ticks_per_second;
        uint64_t max_block_items;
        StorageHints storage_hints;
        std::vector<OpCodes> opcodes;
        std::vector<RrTypes> rr_types;
        StorageFlagsMask storage_flags;
        uint8_t client_address_prefix_ipv4;
        uint8_t client_address_prefix_ipv6;
        uint8_t server_address_prefix_ipv4;
        uint8_t server_address_prefix_ipv6;
        std::string sampling_method;
        std::string anonymization_method;
    };

    /**
     * @brief Collection Parameters structure
     */
    struct CollectionParameters {
        uint64_t query_timeout;
        uint64_t skew_timeout;
        uint64_t snaplen;
        bool promisc;
        std::vector<std::string> interfaces;
        std::vector<std::string> server_address;
        std::vector<uint16_t> vlan_ids;
        std::string filter;
        std::string generator_id;
        std::string host_id;
    };

    /**
     * @brief Block Parameters structure
     */
    struct BlockParameters {
        StorageParameters storage_parameters;
        CollectionParameters collection_parameters;
    };

    /**
     * @brief Class representing C-DNS file preamble
     */
    class FilePreamble {
        public:

        FilePreamble(){}

        FilePreamble(std::vector<BlockParameters> bps, uint8_t private_version = VERSION_PRIVATE)
            : m_private_version(private_version), m_block_parameters(bps) {}

        index_t add_block_parameters(BlockParameters &bp) {
            m_block_parameters.push_back(bp);
            return m_block_parameters.size() - 1;
        }

        std::size_t block_parameters_size() {
            return m_block_parameters.size();
        }

        uint8_t m_major_format_version = VERSION_MAJOR;
        uint8_t m_minor_format_version = VERSION_MINOR;
        uint8_t m_private_version;
        std::vector<BlockParameters> m_block_parameters;
    };
}