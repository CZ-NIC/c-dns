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

#include "timestamp.h"
#include "block.h"

namespace CDNS {

    /**
     * @brief Generic structure for holding data of 1 Question or Resource record before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericResourceRecord {
        /**
         * @brief Default constructor
         */
        GenericResourceRecord() : name(nullptr), classtype(nullptr), ttl(nullptr), rdata(nullptr) {}

        /**
         * @brief Construct a new Generic Resource Record object
         */
        GenericResourceRecord(std::string* domain_name, ClassType* cltype, uint8_t* hoplimit, std::string* data)
                              : name(domain_name), classtype(cltype), ttl(hoplimit), rdata(data) {}

        std::string* name;
        ClassType* classtype;
        uint8_t* ttl; // Not used in Question records
        std::string* rdata; // Not used in Question records
    };

    /**
     * @brief Generic structure for holding 1 DNS record before storing it into Block
     *
     * Serves as "interface" between the C-DNS library and its user
     */
    struct GenericQueryResponse {
        /**
         * @brief Default constructor
         */
        GenericQueryResponse() : ts(nullptr), client_ip(nullptr), client_port(nullptr), transaction_id(nullptr),
                                 server_ip(nullptr), server_port(nullptr), qr_transport_flags(nullptr),
                                 qr_type(nullptr), qr_sig_flags(nullptr), query_opcode(nullptr),
                                 qr_dns_flags(nullptr), query_rcode(nullptr), query_classtype(nullptr),
                                 query_qdcount(nullptr), query_ancount(nullptr), query_nscount(nullptr),
                                 query_arcount(nullptr), query_edns_version(nullptr), query_udp_size(nullptr),
                                 opt_rdata(nullptr), response_rcode(nullptr), client_hoplimit(nullptr),
                                 response_delay(nullptr), query_name(nullptr), query_size(nullptr),
                                 response_size(nullptr), bailiwick(nullptr), processing_flags(nullptr),
                                 query_questions(nullptr), query_answers(nullptr), query_authority(nullptr),
                                 query_additional(nullptr), response_questions(nullptr), response_answers(nullptr),
                                 response_authority(nullptr), response_additional(nullptr) {}

        /**
         * @brief Construct a new generic qr object
         */
        GenericQueryResponse(Timestamp* time, std::string* c_ip, uint16_t* c_port, uint16_t* id,
                             std::string* s_ip, uint16_t* s_port, QueryResponseTransportFlagsMask* tr_flags,
                             QueryResponseTypeValues* type, QueryResponseFlagsMask* sig_flags, uint8_t* opcode,
                             DNSFlagsMask* dns_flags, uint16_t* q_rcode, ClassType* q_classtype,
                             uint16_t* q_qdcount, uint16_t* q_ancount, uint16_t* q_nscount, uint16_t* q_arcount,
                             uint8_t* q_edns_v, uint16_t* q_udp_size, std::string* opt, uint16_t* r_rcode,
                             uint8_t* hoplimit, int64_t* r_delay, std::string* q_name, std::size_t* q_size,
                             std::size_t* r_size, std::string* bwick, ResponseProcessingFlagsMask* proc_flags,
                             std::vector<GenericResourceRecord>* q_questions, std::vector<GenericResourceRecord>* q_answers,
                             std::vector<GenericResourceRecord>* q_authority, std::vector<GenericResourceRecord>* q_additional,
                             std::vector<GenericResourceRecord>* r_questions, std::vector<GenericResourceRecord>* r_answers,
                             std::vector<GenericResourceRecord>* r_authority, std::vector<GenericResourceRecord>* r_additional)
            : ts(time), client_ip(c_ip), client_port(c_port), transaction_id(id), server_ip(s_ip),
              server_port(s_port), qr_transport_flags(tr_flags), qr_type(type), qr_sig_flags(sig_flags),
              query_opcode(opcode), qr_dns_flags(dns_flags), query_rcode(q_rcode),
              query_classtype(q_classtype), query_qdcount(q_qdcount), query_ancount(q_ancount),
              query_nscount(q_nscount), query_arcount(q_arcount), query_edns_version(q_edns_v),
              query_udp_size(q_udp_size), opt_rdata(opt), response_rcode(r_rcode),
              client_hoplimit(hoplimit), response_delay(r_delay), query_name(q_name),
              query_size(q_size), response_size(r_size), bailiwick(bwick), processing_flags(proc_flags),
              query_questions(q_questions), query_answers(q_answers), query_authority(q_authority),
              query_additional(q_additional), response_questions(r_questions),
              response_answers(r_answers), response_authority(r_authority),
              response_additional(r_additional) {}

        Timestamp* ts;
        std::string* client_ip;
        uint16_t* client_port;
        uint16_t* transaction_id;

        // Query Response Signature
        std::string* server_ip;
        uint16_t* server_port;
        QueryResponseTransportFlagsMask* qr_transport_flags;
        QueryResponseTypeValues* qr_type;
        QueryResponseFlagsMask* qr_sig_flags;
        uint8_t* query_opcode;
        DNSFlagsMask* qr_dns_flags;
        uint16_t* query_rcode;
        ClassType* query_classtype;
        uint16_t* query_qdcount;
        uint16_t* query_ancount;
        uint16_t* query_nscount;
        uint16_t* query_arcount;
        uint8_t* query_edns_version;
        uint16_t* query_udp_size;
        std::string* opt_rdata;
        uint16_t* response_rcode;

        uint8_t* client_hoplimit;
        int64_t* response_delay;
        std::string* query_name;
        std::size_t* query_size;
        std::size_t* response_size;

        //Response Processing Data
        std::string* bailiwick;
        ResponseProcessingFlagsMask* processing_flags;

        // Query Response Extended
        std::vector<GenericResourceRecord>* query_questions;
        std::vector<GenericResourceRecord>* query_answers;
        std::vector<GenericResourceRecord>* query_authority;
        std::vector<GenericResourceRecord>* query_additional;
        std::vector<GenericResourceRecord>* response_questions;
        std::vector<GenericResourceRecord>* response_answers;
        std::vector<GenericResourceRecord>* response_authority;
        std::vector<GenericResourceRecord>* response_additional;
    };

    /**
     * @brief Generic structure for holding 1 Address Event Count before storing it into Block
     *
     * Servers as "interface" between the C-DNS library and its user
     */
    struct GenericAddressEventCount {
        /**
         * @brief Default constructor
         */
        GenericAddressEventCount() : ae_type(nullptr), ae_code(nullptr), ae_transport_flags(nullptr),
                                     ip_address(nullptr) {}

        /**
         * @brief Construct a new generic aec object
         */
        GenericAddressEventCount(AddressEventTypeValues* type, uint8_t* code,
                                 QueryResponseTransportFlagsMask* tr_flags, std::string* ip)
            : ae_type(type), ae_code(code), ae_transport_flags(tr_flags), ip_address(ip) {}

        AddressEventTypeValues* ae_type;
        uint8_t* ae_code;
        QueryResponseTransportFlagsMask* ae_transport_flags;
        std::string* ip_address;
    };

    /**
     * @brief Generic structure for holding 1 Malformed Message before storing it into Block
     *
     * Servers as "interface" between the C-DNS library and its user
     */
    struct GenericMalformedMessage {
        /**
         * @brief Default constructor
         */
        GenericMalformedMessage() : ts(nullptr), client_ip(nullptr), client_port(nullptr), server_ip(nullptr),
                                    server_port(nullptr), mm_transport_flags(nullptr), mm_payload(nullptr) {}

        /**
         * @brief Construct a new generic mm object
         */
        GenericMalformedMessage(Timestamp* time, std::string* c_ip, uint16_t* c_port, std::string* s_ip,
                                uint16_t* s_port, QueryResponseTransportFlagsMask* tr_flags, std::string* payload)
            : ts(time), client_ip(c_ip), client_port(c_port), server_ip(s_ip), server_port(s_port),
              mm_transport_flags(tr_flags), mm_payload(payload) {}

        Timestamp* ts;
        std::string* client_ip;
        uint16_t* client_port;

        // Malformed Message Data
        std::string* server_ip;
        uint16_t* server_port;
        QueryResponseTransportFlagsMask* mm_transport_flags;
        std::string* mm_payload;
    };
}