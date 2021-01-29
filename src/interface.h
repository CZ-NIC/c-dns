/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <boost/optional.hpp>

#include "timestamp.h"
#include "block.h"

namespace CDNS {

    /**
     * @brief Generic structure for holding data of 1 Question or Resource record before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericResourceRecord {
        GenericResourceRecord() : name(), classtype() {}

        std::string name;
        ClassType classtype;
        boost::optional<uint8_t> ttl; // Not used in Question records
        boost::optional<std::string> rdata; // Not used in Question records
    };

    /**
     * @brief Generic structure for holding 1 DNS record before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericQueryResponse {
        boost::optional<Timestamp> ts;
        boost::optional<std::string> client_ip;
        boost::optional<uint16_t> client_port;
        boost::optional<uint16_t> transaction_id;

        // Query Response Signature
        boost::optional<std::string> server_ip;
        boost::optional<uint16_t> server_port;
        boost::optional<QueryResponseTransportFlagsMask> qr_transport_flags;
        boost::optional<QueryResponseTypeValues> qr_type;
        boost::optional<QueryResponseFlagsMask> qr_sig_flags;
        boost::optional<uint8_t> query_opcode;
        boost::optional<DNSFlagsMask> qr_dns_flags;
        boost::optional<uint16_t> query_rcode;
        boost::optional<ClassType> query_classtype;
        boost::optional<uint16_t> query_qdcount;
        boost::optional<uint16_t> query_ancount;
        boost::optional<uint16_t> query_nscount;
        boost::optional<uint16_t> query_arcount;
        boost::optional<uint8_t> query_edns_version;
        boost::optional<uint16_t> query_udp_size;
        boost::optional<std::string> query_opt_rdata;
        boost::optional<uint16_t> response_rcode;

        boost::optional<uint8_t> client_hoplimit;
        boost::optional<int64_t> response_delay;
        boost::optional<std::string> query_name;
        boost::optional<std::size_t> query_size;
        boost::optional<std::size_t> response_size;

        // Response Processing Data
        boost::optional<std::string> bailiwick;
        boost::optional<ResponseProcessingFlagsMask> processing_flags;

        // Query Response Extended
        boost::optional<std::vector<GenericResourceRecord>> query_questions;
        boost::optional<std::vector<GenericResourceRecord>> query_answers;
        boost::optional<std::vector<GenericResourceRecord>> query_authority;
        boost::optional<std::vector<GenericResourceRecord>> query_additional;
        boost::optional<std::vector<GenericResourceRecord>> response_questions;
        boost::optional<std::vector<GenericResourceRecord>> response_answers;
        boost::optional<std::vector<GenericResourceRecord>> response_authority;
        boost::optional<std::vector<GenericResourceRecord>> response_additional;

        // Implementation specific fields
        boost::optional<std::string> asn; //!< Autonomous system number for client IP address
        boost::optional<std::string> country_code; //!< Country code for client IP address
        boost::optional<int64_t> round_trip_time; //!< Estimated RTT of TCP connection in ticks
    };

    /**
     * @brief Generic structure for holding 1 Address Event Count before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericAddressEventCount {
        GenericAddressEventCount() : ae_type(static_cast<AddressEventTypeValues>(0)),
                                     ip_address(),
                                     ae_count(0) {}

        AddressEventTypeValues ae_type;
        boost::optional<uint8_t> ae_code;
        boost::optional<QueryResponseTransportFlagsMask> ae_transport_flags;
        std::string ip_address;
        uint64_t ae_count;
    };

    /**
     * @brief Generic structure for holding 1 Malformed Message before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericMalformedMessage {
        boost::optional<Timestamp> ts;
        boost::optional<std::string> client_ip;
        boost::optional<uint16_t> client_port;

        // Malformed Message Data
        boost::optional<std::string> server_ip;
        boost::optional<uint16_t> server_port;
        boost::optional<QueryResponseTransportFlagsMask> mm_transport_flags;
        boost::optional<std::string> mm_payload;
    };
}