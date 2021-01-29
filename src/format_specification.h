/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <type_traits>

namespace CDNS {

    using index_t = uint32_t;

    /**
     * @enum CborType
     * @brief Cbor data types (highest 3 bits of type byte)
     */
    enum class CborType : uint8_t {
        UNSIGNED = 0x00,
        NEGATIVE = 0x20,
        BYTE_STRING = 0x40,
        TEXT_STRING = 0x60,
        ARRAY = 0x80,
        MAP = 0xA0,
        TAG = 0xC0,
        SIMPLE = 0xE0,
        BREAK = 0xFF
    };

    /**
     * @brief Get C-DNS field's map index from its enum class
     * @param t C-DNS field enumerator
     * @return C-DNS field enumerator cast to its underlying type
     */
    template<typename T>
    constexpr auto get_map_index(T t) -> typename std::underlying_type<T>::type {
        return static_cast<typename std::underlying_type<T>::type>(t);
    }

    /**
     * Indexes (keys) of standard fields in C-DNS maps
     */

    /**
     * @enum FileMapIndex
     * @brief File map indexex ( File structure isn't a map?! )
     */
    enum class FileMapIndex : uint8_t {
        file_type_id = 0,
        file_preamble = 1,
        file_block = 2,

        file_size
    };

    /**
     * @enum FilePreambleMapIndex
     * @brief File Preamble map indexes
     */
    enum class FilePreambleMapIndex : uint8_t {
        major_format_version = 0,
        minor_format_version = 1,
        private_version = 2,
        block_parameters = 3,

        file_preamble_size
    };

     /**
      * @enum BlockParametersMapIndex
      * @brief Block Parameters map indexes
      */
    enum class BlockParametersMapIndex : uint8_t {
        storage_parameters = 0,
        collection_parameters = 1,

        block_parameters_size
    };

    /**
     * @enum StorageParametersMapIndex
     * @brief Storage Parameters map indexes
     */
    enum class StorageParametersMapIndex : uint8_t {
        ticks_per_second = 0,
        max_block_items = 1,
        storage_hints = 2,
        opcodes = 3,
        rr_types = 4,
        storage_flags = 5,
        client_address_prefix_ipv4 = 6,
        client_address_prefix_ipv6 = 7,
        server_address_prefix_ipv4 = 8,
        server_address_prefix_ipv6 = 9,
        sampling_method = 10,
        anonymization_method = 11,

        storage_parameters_size
    };

    /**
     * @enum CollectionParametersMapIndex
     * @brief Collection Parameters map indexes
     */
    enum class CollectionParametersMapIndex : uint8_t {
        query_timeout = 0,
        skew_timeout = 1,
        snaplen = 2,
        promisc = 3,
        interfaces = 4,
        server_address = 5,
        vland_ids = 6,
        filter = 7,
        generator_id = 8,
        host_id = 9,

        collection_parameters_size
    };

    /**
     * @enum StorageHintsMapIndex
     * @brief Storage Hints map indexes
     */
    enum class StorageHintsMapIndex : uint8_t {
        query_response_hints = 0,
        query_response_signature_hints = 1,
        rr_hints = 2,
        other_data_hints = 3,

        storage_hints_size
    };

    /**
     * @enum BlockMapIndex
     * @brief Block map indexes
     */
    enum class BlockMapIndex : uint8_t {
        block_preamble = 0,
        block_statistics = 1,
        block_tables = 2,
        query_responses = 3,
        address_event_counts = 4,
        malformed_messages = 5,

        block_size
    };

    /**
     * @enum BlockPreambleMapIndex
     * @brief Block Preamble map indexes
     */
    enum class BlockPreambleMapIndex : uint8_t {
        earliest_time = 0,
        block_parameters_index = 1,

        block_preamble_size
    };

    /**
     * @enum BlockStatisticsMapIndex
     * @brief Block Statistics map indexes
     */
    enum class BlockStatisticsMapIndex : uint8_t {
        processed_messages = 0,
        qr_data_items = 1,
        unmatched_queries = 2,
        unmatched_responses = 3,
        discarded_opcode = 4,
        malformed_items = 5,

        block_statistics_size
    };

    /**
     * @enum BlockTablesMapIndex
     * @brief Block Tables map indexes
     */
    enum class BlockTablesMapIndex : uint8_t {
        ip_address = 0,
        classtype = 1,
        name_rdata = 2,
        qr_sig = 3,
        qlist = 4,
        qrr = 5,
        rrlist = 6,
        rr = 7,
        malformed_message_data = 8,

        block_tables_size
    };

    /**
     * @enum ClassTypeMapIndex
     * @brief ClassType map indexes
     */
    enum class ClassTypeMapIndex : uint8_t {
        type = 0,
        class_ = 1,

        class_type_size
    };

    /**
     * @enum QueryResponseSignatureMapIndex
     * @brief Query Response Signature map indexes
     */
    enum class QueryResponseSignatureMapIndex : uint8_t {
        server_address_index = 0,
        server_port = 1,
        qr_transport_flags = 2,
        qr_type = 3,
        qr_sig_flags = 4,
        query_opcode = 5,
        qr_dns_flags = 6,
        query_rcode = 7,
        query_classtype_index = 8,
        query_qdcount = 9,
        query_ancount = 10,
        query_nscount = 11,
        query_arcount = 12,
        query_edns_version = 13,
        query_udp_size = 14,
        query_opt_rdata_index = 15,
        response_rcode = 16,

        query_response_signature_size
    };

    /**
     * @enum QuestionMapIndex
     * @brief Question map indexes
     */
    enum class QuestionMapIndex : uint8_t {
        name_index = 0,
        classtype_index = 1,

        question_size
    };

    /**
     * @enum RrMapIndex
     * @brief Rr map indexes
     */
    enum class RrMapIndex : uint8_t {
        name_index = 0,
        classtype_index = 1,
        ttl = 2,
        rdata_index = 3,

        rr_size
    };

    /**
     * @enum MalformedMessageDataMapIndex
     * @brief Malformed Message Data map indexes
     */
    enum class MalformedMessageDataMapIndex : uint8_t {
        server_address_index = 0,
        server_port = 1,
        mm_transport_flags = 2,
        mm_payload = 3,

        malformed_message_data_size
    };

    /**
     * @enum QueryResponseMapIndex
     * @brief Query Response map indexes
     */
    enum class QueryResponseMapIndex : int8_t {
        time_offset = 0,
        client_address_index = 1,
        client_port = 2,
        transaction_id = 3,
        qr_signature_index = 4,
        client_hoplimit = 5,
        response_delay = 6,
        query_name_index = 7,
        query_size = 8,
        response_size = 9,
        response_processing_data = 10,
        query_extended = 11,
        response_extended = 12,
        asn = -1, //!< Autonomous system number for client IP address
        country_code = -2, //!< Country code for client IP address
        round_trip_time = -3, //!< Estimated RTT of TCP connection

        query_response_size = 16
    };

    /**
     * @enum ResponseProcessingDataMapIndex
     * @brief Response Processing Data map indexes
     */
    enum class ResponseProcessingDataMapIndex : uint8_t {
        bailiwick_index = 0,
        processing_flags = 1,

        response_processing_data_size
    };

    /**
     * @enum QueryResponseExtendedMapIndex
     * @brief Query Response Extended map indexes
     */
    enum class QueryResponseExtendedMapIndex : uint8_t {
        question_index = 0,
        answer_index = 1,
        authority_index = 2,
        additional_index = 3,

        query_response_extended_size
    };

    /**
     * @enum AddressEventCountMapIndex
     * @brief Address Event Count map indexes
     */
    enum class AddressEventCountMapIndex : uint8_t {
        ae_type = 0,
        ae_code = 1,
        ae_address_index = 2,
        ae_transport_flags = 3,
        ae_count = 4,

        address_event_count_size
    };

    /**
     * @enum MalformedMessageMapIndex
     * @brief Malformed Message map indexes
     */
    enum class MalformedMessageMapIndex : uint8_t {
        time_offset = 0,
        client_address_index = 1,
        client_port = 2,
        message_data_index = 3,

        malformed_message_size
    };

    /**
     * Bit fields enumarations
     */

    /**
     * @enum QueryResponseHintsMask
     * @brief Query Response Hints bitmask
     */
    enum QueryResponseHintsMask : uint32_t {
        time_offset                     = 1 << 0,
        client_address_index            = 1 << 1,
        client_port                     = 1 << 2,
        transaction_id                  = 1 << 3,
        qr_signature_index              = 1 << 4,
        client_hoplimit                 = 1 << 5,
        response_delay                  = 1 << 6,
        query_name_index                = 1 << 7,
        query_size                      = 1 << 8,
        response_size                   = 1 << 9,
        response_processing_data        = 1 << 10,
        query_question_sections         = 1 << 11,
        query_answer_sections           = 1 << 12,
        query_authority_sections        = 1 << 13,
        query_additional_sections       = 1 << 14,
        response_answer_sections        = 1 << 15,
        response_authority_sections     = 1 << 16,
        response_additional_sections    = 1 << 17
    };

    /**
     * @enum QueryResponseSignatureHintsMask
     * @brief Query Response Signature Hints bitmask
     */
    enum QueryResponseSignatureHintsMask : uint32_t {
        server_address_index    = 1 << 0,
        server_port             = 1 << 1,
        qr_transport_flags      = 1 << 2,
        qr_type                 = 1 << 3,
        qr_sig_flags            = 1 << 4,
        query_opcode            = 1 << 5,
        qr_dns_flags            = 1 << 6,
        query_rcode             = 1 << 7,
        query_classtype_index   = 1 << 8,
        query_qdcount           = 1 << 9,
        query_ancount           = 1 << 10,
        query_nscount           = 1 << 11,
        query_arcount           = 1 << 12,
        query_edns_version      = 1 << 13,
        query_udp_size          = 1 << 14,
        query_opt_rdata_index   = 1 << 15,
        response_rcode          = 1 << 16
    };

    /**
     * @enum RrHintsMask
     * @brief Rr Hints bitmask
     */
    enum RrHintsMask : uint8_t {
        ttl         = 1 << 0,
        rdata_index = 1 << 1
    };

    /**
     * @enum OtherDataHintsMask
     * @brief Other Data Hints bitmask
     */
    enum OtherDataHintsMask : uint8_t {
        malformed_messages   = 1 << 0,
        address_event_counts = 1 << 1
    };

    /**
     * @enum StorageFlagsMask
     * @brief Storage Flags bitmask
     */
    enum StorageFlagsMask : uint8_t {
        anonymized_data  = 1 << 0,
        sampled_data     = 1 << 1,
        normalized_data  = 1 << 2
    };

    /**
     * @enum QueryResponseTransportFlagsMask
     * @brief Query Response Transport Flags bitmask
     */
    enum QueryResponseTransportFlagsMask : uint8_t {
        ip_address = 1 << 0,

        transport_flags = 15 << 1,
        udp             = 0 << 1,
        tcp             = 1 << 1,
        tls             = 2 << 1,
        dtls            = 3 << 1,
        https           = 4 << 1,
        non_standard    = 15 << 1,

        query_trailingdata = 1 << 5
    };

    /**
     * @enum QueryResponseFlagsMask
     * @brief Query Response Flags bitmask
     */
    enum QueryResponseFlagsMask : uint8_t {
        has_query                 = 1 << 0,
        has_response              = 1 << 1,
        query_has_opt             = 1 << 2,
        response_has_opt          = 1 << 3,
        query_has_no_question     = 1 << 4,
        response_has_no_question  = 1 << 5
    };

    /**
     * @enum DNSFlagsMask
     * @brief DNS Flags bitmask
     */
    enum DNSFlagsMask : uint16_t {
        query_cd    = 1 << 0,
        query_ad    = 1 << 1,
        query_z     = 1 << 2,
        query_ra    = 1 << 3,
        query_rd    = 1 << 4,
        query_tc    = 1 << 5,
        query_aa    = 1 << 6,
        query_do    = 1 << 7,
        response_cd = 1 << 8,
        response_ad = 1 << 9,
        response_z  = 1 << 10,
        response_ra = 1 << 11,
        response_rd = 1 << 12,
        response_tc = 1 << 13,
        response_aa = 1 << 14
    };

    /**
     * @enum ResponseProcessingFlagsMask
     * @brief Response Processing Flags bitmask
     */
    enum ResponseProcessingFlagsMask : uint8_t {
        from_cache = 1 << 0
    };

    /**
     * Field values enumerations
     */

    /**
     * @enum QueryResponseTypeValues
     * @brief Query Response Type field possible values
     */
    enum class QueryResponseTypeValues : uint8_t {
        stub = 0,
        client = 1,
        resolver = 2,
        auth = 3,
        forwarder = 4,
        tool = 5
    };

    /**
     * @enum AddressEventTypeValues
     * @brief Address Event Type field possible values
     */
    enum class AddressEventTypeValues : uint8_t {
        tcp_reset = 0,
        icmp_time_exceeded = 1,
        icmp_dest_unreachable = 2,
        icmpv6_time_exceeded = 3,
        icmpv6_dest_unreachable = 4,
        icmpv6_packet_too_big = 5
    };
}