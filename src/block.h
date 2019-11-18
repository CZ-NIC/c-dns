#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <deque>
#include <vector>

#include "format_specification.h"
#include "block_table.h"
#include "hash.h"

namespace CDNS {

    /**
     * @brief Simple timestamp representation
     */
    struct Timestamp {
        uint64_t timestamp_secs;
        uint64_t timestamp_ticks;
    };

    /**
     * @brief Block table's ClassType structure
     */
    struct ClassType {
        ClassType() : type(0), class_(0) {}

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const ClassType& key() const {
            return *this;
        }

        uint16_t type;
        uint16_t class_;
    };

    /**
     * @brief Block table's Query Response Signature structure
     */
    struct QueryResponseSignature {
        QueryResponseSignature() : server_address_index(0), server_port(0), qr_transport_flags(),
            qr_type(), qr_sig_flags(), query_opcode(0), qr_dns_flags(), query_rcode(0),
            query_classtype_index(0), query_qdcount(0), query_ancount(0), query_nscount(0),
            query_arcount(0), query_edns_version(0), query_udp_size(0), query_opt_rdata_index(0),
            response_rcode(0) {}

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const QueryResponseSignature& key() const {
            return *this;
        }

        index_t server_address_index;
        uint16_t server_port;
        QueryResponseTransportFlagsMask qr_transport_flags;
        QueryResponseTypeValues qr_type;
        QueryResponseFlagsMask qr_sig_flags;
        uint8_t query_opcode;
        DNSFlagsMask qr_dns_flags;
        uint16_t query_rcode;
        index_t query_classtype_index;
        uint16_t query_qdcount;
        uint32_t query_ancount;
        uint16_t query_nscount;
        uint16_t query_arcount;
        uint8_t query_edns_version;
        uint16_t query_udp_size;
        index_t query_opt_rdata_index;
        uint16_t response_rcode;
    };

    /**
     * @brief Block table's Question structure
     */
    struct Question {
        Question() : name_index(0), classtype_index(0) {}

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const Question& key() const {
            return *this;
        }

        index_t name_index;
        index_t classtype_index;
    };

    /**
     * @brief Block table's RR structure
     */
    struct RR {
        RR() : name_index(0), classtype_index(0), ttl(0), rdata_index(0) {}

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const RR& key() const {
            return *this;
        }

        index_t name_index;
        index_t classtype_index;
        uint8_t ttl;
        index_t rdata_index;
    };

    /**
     * @brief Block table's Malformed Message Data structure
     */
    struct MalformedMessageData {
        MalformedMessageData() : server_address_index(0), server_port(0), mm_transport_flags(),
            mm_payload() {}

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const MalformedMessageData& key() const {
            return *this;
        }

        index_t server_address_index;
        uint16_t server_port;
        QueryResponseTransportFlagsMask mm_transport_flags;
        std::string mm_payload;
    };

    /**
     * @brief Block table's Response Processing Data structure
     */
    struct ResponseProcessingData {
        index_t bailiwick_index;
        ResponseProcessingFlagsMask processing_flags;
    };

    /**
     * @brief Block table's Query Response Extended structure
     */
    struct QueryResponseExtended {
        index_t question_index;
        index_t answer_index;
        index_t authority_index;
        index_t additional_index;
    };

    /**
     * @brief Block preamble structure
     */
    struct BlockPreamble {
        uint64_t earliest_time; //TODO create timestamp class
        index_t block_parameters_index;
    };

    /**
     * @brief Block statistics structure
     */
    struct BlockStatistics {
        unsigned processed_messages;
        unsigned qr_data_items;
        unsigned unmatched_queries;
        unsigned unmatched_responses;
        unsigned discarded_opcode;
        unsigned malformed_items;
    };

    /**
     * @brief QueryResponse item structure
     */
    struct QueryResponse {
        uint64_t time_offset;
        index_t client_address_index;
        uint16_t client_port;
        uint16_t transaction_id;
        index_t qr_signature_index;
        uint8_t client_hoplimit;
        int64_t response_delay;
        index_t query_name_index;
        std::size_t query_size;
        std::size_t response_size;
        ResponseProcessingData response_processing_data;
        QueryResponseExtended query_extended;
        QueryResponseExtended response_extended;
    };

    /**
     * @brief Address Event Count item structure
     */
    struct AddressEventCount {
        AddressEventCount() : ae_type(), ae_code(0), ae_transport_flags(), ae_address_index(0) {}

        /**
         * @brief Return reference to itself as key for unordered_map
         */
        const AddressEventCount& key() const {
            return *this;
        }

        AddressEventTypeValues ae_type;
        uint8_t ae_code;
        QueryResponseTransportFlagsMask ae_transport_flags;
        index_t ae_address_index;
        //uint64_t ae_count;
    };

    /**
     * @brief Malformed Message item structure
     */
    struct MalformedMessage {
        uint64_t time_offset;
        index_t client_address_index;
        uint16_t client_port;
        index_t message_data_index;
    };

    /**
     * @brief Structure representing byte string item
     */
    struct StringItem {
        StringItem() : data() {}

        /**
         * @brief Return reference to the byte string data as key for unordered_map
         */
        const std::string& key() const {
            return data;
        }

        std::string data;
    };

    /**
     * @brief Structure representing list of indexes to question or resource records
     */
    struct IndexListItem {
        IndexListItem() : list() {}

        /**
         * @brief Return reference to the list as key for unordered_map
         */
        const std::vector<index_t>& key() const {
            return list;
        }

        std::vector<index_t> list;
    };

    /**
     * @brief Class representing C-DNS block
     */
    class CdnsBlock {
        public:

        CdnsBlock() {}

        //TODO
        //void write_cbor(CborEncoder &enc);

        BlockPreamble m_block_preamble;
        BlockStatistics m_block_statistics;

        // Block Tables
        BlockTable<StringItem, std::string> m_ip_address;
        BlockTable<ClassType> m_classtype;
        BlockTable<StringItem, std::string> m_name_rdata;
        BlockTable<QueryResponseSignature> m_qr_sig;
        BlockTable<IndexListItem, std::vector<index_t>> m_qlist;
        BlockTable<Question> m_qrr;
        BlockTable<IndexListItem, std::vector<index_t>> m_rrlist;
        BlockTable<RR> m_rr;
        BlockTable<MalformedMessageData> m_malformed_message_data;

        std::vector<QueryResponse> m_query_responses;
        std::unordered_map<AddressEventCount, uint64_t, CDNS::hash<AddressEventCount>> m_address_event_counts;
        std::vector<MalformedMessage> m_malformed_messages;
    };
}