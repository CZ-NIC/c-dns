/**
 * Copyright © 2024 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sstream>
#include <bitset>
#include <type_traits>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "interface.h"

/**
 * @brief Get human readable IP address from wire format IP address
 * @param wire_ip IP address in wire format
 * @param ipv6 TRUE if IP address in wire_ip is IPv6, FALSE if it's IPv4
 * @return String with humand readable IP address
 */
static std::string get_readable_ip_address(std::string& wire_ip, bool ipv6)
{
    int ipv = 0;
    unsigned buflen = 0;

    if (ipv6) {
        ipv = AF_INET6;
        buflen = INET6_ADDRSTRLEN + 4;
    }
    else {
        ipv = AF_INET;
        buflen = INET_ADDRSTRLEN + 4;
    }

    char addrBuf[buflen];
    auto ret = inet_ntop(ipv, wire_ip.data(), addrBuf, sizeof(addrBuf));

    if (!ret)
        return wire_ip;

    return std::string(addrBuf, strlen(addrBuf));
}

/**
 * @brief Get human readable domain name from domain name in wire format
 * @param wire_dname Domain name in wire format
 * @return String with humand readable domain name
 */
static std::string get_readable_dname(std::string& wire_dname)
{
    if (wire_dname.empty())
        return wire_dname;

    std::string dname = wire_dname;

    uint8_t labels = 0;
    int size = 0;
    auto label_len = static_cast<uint8_t>(dname[0]);
    auto pos = static_cast<uint64_t>(label_len + 1);

    while (label_len != 0) {
        size += label_len;
        if (size > dname.size())
            return wire_dname;

        labels++;
        label_len = dname[pos];

        // Replace all label length bytes with '.' character
        if (label_len != 0)
            dname[pos] = '.';

        pos += label_len + 1;
    }

    // Return domain name without the last '.'
    if (labels > 0)
        return std::string(dname.data() + 1, dname.size() - 2);
    else
        return std::string(dname.data(), dname.size() - 1);
}

std::string CDNS::GenericResourceRecord::string()
{
    std::stringstream ss;

    ss << "Domain name: " << get_readable_dname(name) << std::endl;
    ss << classtype.string();

    if (ttl)
        ss << "TTL: " << std::to_string(ttl.value()) << std::endl;

    if (rdata)
        ss << "RDATA: " << rdata.value() << std::endl;

    return ss.str();
}

std::string CDNS::GenericQueryResponse::string()
{
    std::stringstream ss;

    if (ts)
        ss << ts.value().string();

    if (client_ip)
        ss << "Client address: " << get_readable_ip_address(client_ip.value(), client_ip.value().size() == 16) << std::endl;

    if (client_port)
        ss << "Client port: " << std::to_string(client_port.value()) << std::endl;

    if (transaction_id)
        ss << "Transaction ID: " << std::to_string(transaction_id.value()) << std::endl;

    if (server_ip)
        ss << "Server address: " << get_readable_ip_address(server_ip.value(), server_ip.value().size() == 16) << std::endl;

    if (server_port)
        ss << "Server port: " << server_port.value() << std::endl;

    if (qr_transport_flags)
        ss << "Query transport flags: " << std::bitset<8>(qr_transport_flags.value()) << std::endl;

    if (qr_type)
        ss << "Type of query: " << std::to_string(static_cast<std::underlying_type<CDNS::QueryResponseTypeValues>::type>(qr_type.value())) << std::endl;

    if (qr_sig_flags)
        ss << "Query signagure flags: " << std::bitset<8>(qr_sig_flags.value()) << std::endl;

    if (query_opcode)
        ss << "Query OPCODE: " << std::to_string(query_opcode.value()) << std::endl;

    if (qr_dns_flags)
        ss << "Query DNS flags: " << std::bitset<16>(qr_dns_flags.value()) << std::endl;

    if (query_rcode)
        ss << "Query RCODE: " << std::to_string(query_rcode.value()) << std::endl;

    if (query_classtype)
        ss << query_classtype.value().string();

    if (query_qdcount)
        ss << "Query QDCOUNT: " << std::to_string(query_qdcount.value()) << std::endl;

    if (query_ancount)
        ss << "Query ANCOUNT: " << std::to_string(query_ancount.value()) << std::endl;

    if (query_nscount)
        ss << "Query NSCOUNT: " << std::to_string(query_nscount.value()) << std::endl;

    if (query_arcount)
        ss << "Query ARCOUNT: " << std::to_string(query_arcount.value()) << std::endl;

    if (query_edns_version)
        ss << "Query EDNS version: " << std::to_string(query_edns_version.value()) << std::endl;

    if (query_udp_size)
        ss << "Query EDNS UDP payload size: " << std::to_string(query_udp_size.value()) << std::endl;

    if (query_opt_rdata)
        ss << "Query OPT RDATA: " << query_opt_rdata.value() << std::endl;

    if (response_rcode)
        ss << "Response RCODE: " << std::to_string(response_rcode.value()) << std::endl;

    if (client_hoplimit)
        ss << "Client hoplimit: " << std::to_string(client_hoplimit.value()) << std::endl;

    if (response_delay)
        ss << "Response delay: " << std::to_string(response_delay.value()) << std::endl;

    if (query_name)
        ss << "QNAME: " << get_readable_dname(query_name.value()) << std::endl;

    if (query_size)
        ss << "Query size: " << std::to_string(query_size.value()) << std::endl;

    if (response_size)
        ss << "Response size: " << std::to_string(response_size.value()) << std::endl;

    if (bailiwick)
        ss << "Bailiwick: " << bailiwick.value() << std::endl;

    if (processing_flags)
        ss << "Processing flags: " << std::bitset<8>(processing_flags.value()) << std::endl;

    if (asn)
        ss << "ASN: " << asn.value() << std::endl;

    if (country_code)
        ss << "Country code: " << country_code.value() << std::endl;

    if (round_trip_time)
        ss << "RTT: " << std::to_string(round_trip_time.value()) << std::endl;

    if (query_questions) {
        for (unsigned i = 0; i < query_questions.value().size(); i++) {
            ss << "Query question RR " << std::to_string(i) << ":" << std::endl;
            ss << query_questions.value()[i].string();
        }
    }

    if (query_answers) {
        for (unsigned i = 0; i < query_answers.value().size(); i++) {
            ss << "Query answer RR " << std::to_string(i) << ":" << std::endl;
            ss << query_answers.value()[i].string();
        }
    }

    if (query_authority) {
        for (unsigned i = 0; i < query_authority.value().size(); i++) {
            ss << "Query authority RR " << std::to_string(i) << ":" << std::endl;
            ss << query_authority.value()[i].string();
        }
    }

    if (query_additional) {
        for (unsigned i = 0; i < query_additional.value().size(); i++) {
            ss << "Query additional RR " << std::to_string(i) << ":" << std::endl;
            ss << query_additional.value()[i].string();
        }
    }

    if (response_questions) {
        for (unsigned i = 0; i < response_questions.value().size(); i++) {
            ss << "Response question RR " << std::to_string(i) << ":" << std::endl;
            ss << response_questions.value()[i].string();
        }
    }

    if (response_answers) {
        for (unsigned i = 0; i < response_answers.value().size(); i++) {
            ss << "Response answer RR " << std::to_string(i) << ":" << std::endl;
            ss << response_answers.value()[i].string();
        }
    }

    if (response_authority) {
        for (unsigned i = 0; i < response_authority.value().size(); i++) {
            ss << "Response authority RR " << std::to_string(i) << ":" << std::endl;
            ss << response_authority.value()[i].string();
        }
    }

    if (response_additional) {
        for (unsigned i = 0; i < response_additional.value().size(); i++) {
            ss << "Response additional RR " << std::to_string(i) << ":" << std::endl;
            ss << response_additional.value()[i].string();
        }
    }

    return ss.str();
}

std::string CDNS::GenericAddressEventCount::string()
{
    std::stringstream ss;

    ss << "Address event type: " << std::to_string(static_cast<std::underlying_type<CDNS::AddressEventTypeValues>::type>(ae_type)) << std::endl;

    if (ae_code)
        ss << "Address event code: " << std::to_string(ae_code.value()) << std::endl;

    if (ae_transport_flags) {
        ss << "Address event transport flags: " << std::bitset<8>(ae_transport_flags.value()) << std::endl;
    }

    ss << "Address: " << get_readable_ip_address(ip_address, ip_address.size() == 16) << std::endl;
    ss << "Address event count: " << std::to_string(ae_count) << std::endl;

    return ss.str();
}

std::string CDNS::GenericMalformedMessage::string()
{
    std::stringstream ss;

    if (ts)
        ss << ts.value().string();

    if (client_ip)
        ss << "Client IP: " << get_readable_ip_address(client_ip.value(), client_ip.value().size() == 16) << std::endl;

    if (client_port)
        ss << "Client port: " << std::to_string(client_port.value()) << std::endl;

    if (server_ip)
        ss << "Server IP: " << get_readable_ip_address(server_ip.value(), server_ip.value().size() == 16) << std::endl;

    if (server_port)
        ss << "Server port: " << std::to_string(server_port.value()) << std::endl;

    if (mm_transport_flags)
        ss << "MM transport flags: " << std::bitset<8>(mm_transport_flags.value()) << std::endl;

    if (mm_payload)
        ss << "MM payload: " << mm_payload.value() << std::endl;

    return ss.str();
}