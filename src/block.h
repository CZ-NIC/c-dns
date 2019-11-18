#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <deque>
#include <vector>

#include "format_specification.h"
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
        uint16_t type;
        uint16_t class_;
    };

    /**
     * @brief Block table's Query Response Signature structure
     */
    struct QueryResponseSignature {
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
        index_t name_index;
        index_t classtype_index;
    };

    /**
     * @brief Block table's RR structure
     */
    struct RR {
        index_t name_index;
        index_t clastype_index;
        uint8_t ttl;
        index_t rdata_index;
    };

    /**
     * @brief Block table's Malformed Message Data structure
     */
    struct MalformedMessageData {
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
     * @brief Block tables's Query Response Extended structure
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
     * @brief Representation of one block table's table
     */
    template<typename T>
    class BlockTable {
        public:

        private:
        std::deque<T> m_items;
        std::unordered_map<T, index_t, CDNS::hash<T>> m_indexes;
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
        BlockTable<std::string> m_ip_address;
        BlockTable<ClassType> m_classtype;
        BlockTable<std::string> m_name_rdata;
        BlockTable<QueryResponseSignature> m_qr_sig;
        BlockTable<std::vector<index_t>> m_qlist;
        BlockTable<Question> m_qrr;
        BlockTable<std::vector<index_t>> m_rrlist;
        BlockTable<RR> m_rr;
        BlockTable<MalformedMessageData> m_malformed_message_data;

        std::vector<QueryResponse> m_query_responses;
        std::unordered_map<AddressEventCount, uint64_t, CDNS::hash<AddressEventCount>> m_address_event_counts;
        std::vector<MalformedMessage> m_malformed_messages;
    };
}