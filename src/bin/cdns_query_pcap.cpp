/**
 * Copyright © 2025 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <bitset>
#include <array>
#include <arpa/inet.h>
#include <fstream>

#include "../cdns.h"

/** @def ETH_LENGTH
 *  Ethernet header length in bytes
 */
#define ETH_LENGTH 14

/** @def ETH_IPv4_VERSION
 *  Ethernet type for IPv4
 */
#define ETH_IPv4_VERSION 0x0800

/** @def ETH_IPv6_VERSION
 *  Ethernet type for IPv6
 */
#define ETH_IPv6_VERSION 0x86DD

/** @def IPv4_LENGTH
 *  Length of IPv4 header
 */
#define IPv4_LENGTH 20

/** @def IPv6_LENGTH
 *  Length of IPv6 header
 */
#define IPv6_LENGTH 40

/** @def IP_MAX_LENGTH
 *  Maximum IP header length
 */
#define IP_MAX_LENGTH 40

/** @def UDP_LENGTH
 *  UDP header length
 */
#define UDP_LENGTH 8

/** @def DNS_QUERY_MAX_LENGTH
 *  Maximum DNS query length
 */
#define DNS_QUERY_MAX_LENGTH 512

#pragma pack(push, 1)
/**
 * @struct PcapGlobalHeader
 * @brief Global header for a PCAP file
 */
struct PcapGlobalHeader {
    uint32_t magic_number = 0xa1b2c3d4; /**< Magic number identifying the file as a pcap */
    uint16_t version_major = 2;         /**< Major version */
    uint16_t version_minor = 4;         /**< Minor version */
    int32_t thiszone = 0;               /**< GMT to local correction */
    uint32_t sigfigs = 0;               /**< Accuracy of timestamps */
    uint32_t snaplen = 65535;           /**< Max length of captured packets, in octets */
    uint32_t network = 1;               /**< Data link type */
};

/**
 * @struct PcapPacketHeader
 * @brief Header for individual packets in PCAP
 */
struct PcapPacketHeader {
    uint32_t ts_sec;   /**< Timestamp seconds */
    uint32_t ts_usec;  /**< Timestamp microseconds */
    uint32_t incl_len; /**< Number of octets of packet saved in file */
    uint32_t orig_len; /**< Actual length of packet */
};
#pragma pack(pop)

struct pcap_parts {
        uint8_t eth[ETH_LENGTH] = {
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                0x00, 0x0c, 0x29, 0xab, 0xcd, 0xef,
                0x00, 0x00
        }; /**< Ethernet header */
        uint16_t eth_len = 0; /**< Length of Ethernet header */

        uint8_t ip[IP_MAX_LENGTH] = {0}; /**< IP header buffer */
        uint16_t ip_len = 0;             /**< Length of IP header */

        uint8_t udp[UDP_LENGTH] = {0}; /**< UDP header buffer */
        uint16_t udp_len = 0;          /**< Length of UDP header */

        uint8_t dns[DNS_QUERY_MAX_LENGTH] = {0}; /**< DNS payload */
        uint16_t dns_len = 0;                    /**< Length of DNS payload */

        uint16_t total_len = 0; /**< Total length of the packet */
};

/**
 * @brief Write a single packet into a PCAP file
 * @param pcap Output file stream
 * @param pp Packet components
 * @param timestamp Timestamp of the packet
 */
void write_pcap_packet(std::ofstream& pcap, struct pcap_parts pp, const CDNS::Timestamp& timestamp) {
        PcapPacketHeader ph;
        ph.ts_sec = timestamp.m_secs;
        ph.ts_usec = timestamp.m_ticks;

        ph.incl_len = pp.total_len;
        ph.orig_len = pp.total_len;

        pcap.write(reinterpret_cast<const char*>(&ph), sizeof(ph));
        pcap.write(reinterpret_cast<const char*>(pp.eth), pp.eth_len);
        pcap.write(reinterpret_cast<const char*>(pp.ip), pp.ip_len);
        pcap.write(reinterpret_cast<const char*>(pp.udp), pp.udp_len);
        pcap.write(reinterpret_cast<const char*>(pp.dns), pp.dns_len);
}

/**
 * @brief Construct Ethernet header based on IP version
 * @param eth_data Ethernet header buffer
 * @param gqr DNS query response
 * @return Length of Ethernet header (0 if client_ip missing)
 */
uint16_t construct_eth(uint8_t* eth_data, CDNS::GenericQueryResponse gqr) {
        if (!gqr.client_ip.has_value())
                return 0;

        std::string client_ip = *(gqr.client_ip);
        uint16_t ip_version = client_ip.size() > 4 ? ETH_IPv6_VERSION : ETH_IPv4_VERSION;
        uint16_t net_ip_version = htons(ip_version);
        std::memcpy(eth_data + 12, &net_ip_version, 2);
        return ETH_LENGTH;
}

/**
 * @brief Finish checksum calculation
 * @param sum Partial checksum
 * @return Final checksum value
 */
uint16_t checksum_finish(uint32_t sum) {
        while (sum >> 16)
                sum = (sum & 0xFFFF) + (sum >> 16);

        return static_cast<uint16_t>(~sum);
}

/**
 * @brief Add up bytes for checksum calculation
 * @param len Length of buffer
 * @param buf Buffer to compute checksum on
 * @return Summed value
 */
uint32_t checksum_add(int len, uint8_t *buf) {
        uint32_t sum = 0;
        int i;

        for (i = 0; i < len; i++) {
                if (i & 1)
                sum += (uint32_t)buf[i];
                else
                sum += (uint32_t)buf[i] << 8;
        }
        return sum;
}

/**
 * @brief Create pseudo-header for IPv4 checksum calculation
 * @param ip_data IPv4 header
 * @return Vector containing pseudo-header bytes
 */
std::vector<uint8_t> create_ipv4_pseudoheader(const uint8_t* ip_data) {
        std::vector<uint8_t> checksum_data;

        uint16_t hlen  = (ip_data[0] & 0x0f) * 4;
        uint16_t plen = htons((static_cast<uint16_t>(ip_data[2]) << 8 | ip_data[3]) - hlen);

        checksum_data.insert(checksum_data.end(), ip_data + 12, ip_data + 16);
        checksum_data.insert(checksum_data.end(), ip_data + 16, ip_data + 20);

        checksum_data.push_back(0);

        checksum_data.insert(checksum_data.end(), ip_data + 9, ip_data + 10);
        checksum_data.insert(checksum_data.end(), reinterpret_cast<uint8_t*>(&plen), reinterpret_cast<uint8_t*>(&plen) + 2);

        return checksum_data;
}

/**
 * @brief Create pseudo-header for IPv6 checksum calculation
 * @param ip_data IPv6 header
 * @return Vector containing pseudo-header bytes
 */
std::vector<uint8_t> create_ipv6_pseudoheader(const uint8_t* ip_data) {
        std::vector<uint8_t> checksum_data;

        uint16_t plen = htons(static_cast<uint16_t>(ip_data[4]) << 8 | ip_data[5]);

        checksum_data.insert(checksum_data.end(), ip_data + 8, ip_data + 24);
        checksum_data.insert(checksum_data.end(), ip_data + 24, ip_data + 40);

        checksum_data.push_back(0);

        checksum_data.insert(checksum_data.end(), ip_data + 6, ip_data + 7);
        checksum_data.insert(checksum_data.end(), reinterpret_cast<uint8_t*>(&plen), reinterpret_cast<uint8_t*>(&plen) + 2);

        return checksum_data;
}

/**
 * @brief Calculate UDP checksum for packet
 * @param pp Pointer to pcap_parts structure
 * @param is_ipv6 True if packet is IPv6
 */
void udp_checksum(struct pcap_parts* pp, bool is_ipv6) {
        std::vector<uint8_t> checksum_data;

        if (is_ipv6) {
                checksum_data = create_ipv6_pseudoheader(pp->ip);
        } else {
                checksum_data = create_ipv4_pseudoheader(pp->ip);
        }

        std::memset(pp->udp + 6, 0, 2);

        checksum_data.insert(checksum_data.end(), pp->udp, pp->udp + pp->udp_len);
        checksum_data.insert(checksum_data.end(), pp->dns, pp->dns + pp->dns_len);

        if (checksum_data.size() % 2 != 0)
                checksum_data.push_back(0);

        uint32_t sum = checksum_add(checksum_data.size(), checksum_data.data());
        uint16_t chksum = htons(checksum_finish(sum));
        std::memcpy(pp->udp + 6, &chksum, 2);
}

/**
 * @brief Construct IP header for packet
 * @param ip_data Buffer to write IP header into
 * @param gqr DNS query response
 * @param payload_length Length of payload
 * @return Length of IP header
 */
uint16_t construct_ip(uint8_t* ip_data, CDNS::GenericQueryResponse gqr, uint16_t payload_length) {
        const uint8_t ipv4_header[] = {
                0x45, 0x00, 0x00, 0x1c,
                0x00, 0x00, 0x40, 0x00,
                0x40, 0x11, 0x00, 0x00
        };

        const uint8_t ipv6_header[] = {
                0x60, 0x00, 0x00, 0x00,
                0x00, 0x08, 0x11, 0x40
        };

        const uint8_t* header;
        size_t header_len;
        size_t addr_size;
        size_t data_size_offset = 0;

        if (!gqr.client_ip.has_value() || !gqr.server_ip.has_value())
                return 0;

        std::string src = *(gqr.client_ip);
        std::string dst = *(gqr.server_ip);

        if (src.length() > 4) {
                header = ipv6_header;
                header_len = 8;
                addr_size = 16;
                data_size_offset = 4;

                payload_length = htons(payload_length);
        } else {
                header = ipv4_header;
                header_len = 12;
                addr_size = 4;
                data_size_offset = 2;

                payload_length = htons(payload_length + IPv4_LENGTH);
        }

        std::memcpy(ip_data, header, header_len);
        std::memcpy(ip_data + data_size_offset, &payload_length, 2);
        std::memcpy(ip_data + header_len, src.data(), addr_size);
        std::memcpy(ip_data + header_len + addr_size, dst.data(), addr_size);

        if (src.length() > 4) {
                return IPv6_LENGTH;
        } else {
                return IPv4_LENGTH;
        }
}

/**
 * @brief Construct UDP header for packet
 * @param udp_data Buffer to write UDP header into
 * @param gqr DNS query response
 * @param payload Pointer to payload data
 * @param payload_size Size of payload
 * @return Length of UDP header
 */
uint16_t construct_udp(uint8_t* udp_data, CDNS::GenericQueryResponse gqr, uint8_t* payload, uint16_t payload_size) {
        if (!gqr.client_port.has_value() || !gqr.server_port.has_value())
                return 0;

        uint16_t client_port = htons(*(gqr.client_port));
        uint16_t server_port = htons(*(gqr.server_port));

        payload_size = htons(payload_size + UDP_LENGTH);

        std::memcpy(udp_data, &client_port, 2);
        std::memcpy(udp_data + 2, &server_port, 2);
        std::memcpy(udp_data + 4, &payload_size, 2);

        return UDP_LENGTH;
}

/**
 * @brief Create DNS query footer (type and class)
 * @param gqr DNS query response
 * @return Optional array of 4 bytes containing the footer
 */
boost::optional<std::array<uint8_t, 4>> create_dns_query_footer(CDNS::GenericQueryResponse gqr) {
        if (gqr.query_classtype.has_value()) {
                auto query_class_type = *(gqr.query_classtype);
                uint16_t type = query_class_type.type;
                uint16_t class_ = query_class_type.class_;

                std::array<uint8_t, 4> query_footer;

                query_footer[0] = static_cast<uint8_t>((type >> 8) & 0xFF);
                query_footer[1] = static_cast<uint8_t>(type & 0xFF);

                query_footer[2] = static_cast<uint8_t>((class_ >> 8) & 0xFF);
                query_footer[3] = static_cast<uint8_t>(class_ & 0xFF);

                return query_footer;
        }
        return boost::none;
}

/**
 * @brief Create DNS query name in wire format
 * @param gqr DNS query response
 * @return Optional vector containing query name in DNS wire format
 */
boost::optional<std::vector<uint8_t>> create_dns_query(CDNS::GenericQueryResponse gqr) {
        if (!gqr.query_name.has_value())
                return boost::none;

        auto query_name = *(gqr.query_name);
        const auto qd_size = query_name.size();

        std::vector<uint8_t> dynamic_bitset;
        uint8_t temp_byte = 0;

        for (uint8_t c : query_name) {
        std::bitset<8> char_bits(c);
        temp_byte = 0;

        for (size_t i = 0; i < 8; ++i) {
                temp_byte |= (char_bits[7 - i] << (7 - i));
        }
        dynamic_bitset.push_back(temp_byte);
        }
        return dynamic_bitset;
}

/**
 * @brief Create DNS header flags
 * @param isResponse True if packet is a response
 * @param opcode DNS opcode
 * @param rcode DNS response code
 * @param dnsFlags DNS flags
 * @return 16-bit flags field
 */
uint16_t create_dns_header_flags(bool isResponse, uint8_t opcode, uint8_t rcode, uint16_t dnsFlags) {
        uint16_t flags = 0;

        flags |= (isResponse ? (1 << 15) : 0);

        flags |= ((opcode & 0xF) << 11);

        flags |= ((dnsFlags & 0x35) << 4);

        return flags;
}

/**
 * @brief Check if DNS query has all required fields
 * @param gqr DNS query response
 * @return True if valid DNS query
 */
bool is_valid_dns(const CDNS::GenericQueryResponse gqr) {
    return gqr.transaction_id.has_value()
        && gqr.qr_dns_flags.has_value()
        && gqr.query_opcode.has_value()
        && gqr.query_qdcount.has_value()
        && gqr.query_ancount.has_value()
        && gqr.query_nscount.has_value()
        && gqr.query_arcount.has_value();
}

/**
 * @brief Create DNS header
 * @param gqr DNS query response
 * @return Optional 12-byte DNS header
 */
boost::optional<std::array<uint8_t, 12>> create_dns_header(CDNS::GenericQueryResponse gqr) {
        if (!is_valid_dns(gqr))
                return boost::none;

        auto transaction_id               = *(gqr.transaction_id);
        auto dns_flags                    = *(gqr.qr_dns_flags);
        auto op_code                      = *(gqr.query_opcode);
        uint16_t qdcount = *(gqr.query_qdcount);
        uint16_t ancount = 0;
        uint16_t nscount = 0;
        uint16_t arcount = 0;

        auto header_flags = create_dns_header_flags(false, op_code, 0, dns_flags);

        std::array<uint8_t, 12> wire_packet;

        uint16_t packet[6] = {
                transaction_id,
                header_flags,
                qdcount,
                ancount,
                nscount,
                arcount
        };

        for (size_t i = 0; i < 6; ++i) {
                wire_packet[i * 2]     = static_cast<uint8_t>((packet[i] >> 8) & 0xFF);
                wire_packet[i * 2 + 1] = static_cast<uint8_t>(packet[i] & 0xFF);
        }

        return wire_packet;
}

/**
 * @brief Construct DNS payload (header + query + footer)
 * @param dns_data Buffer to write DNS data
 * @param gqr DNS query response
 * @return Total size of DNS payload
 */
uint16_t construct_dns(uint8_t* dns_data, CDNS::GenericQueryResponse gqr) {
        boost::optional<std::array<uint8_t, 12>> wire_header = create_dns_header(gqr);
        boost::optional<std::vector<uint8_t>>    wire_query  = create_dns_query(gqr);
        boost::optional<std::array<uint8_t, 4>>  wire_footer = create_dns_query_footer(gqr);

        uint16_t offset = 0;

        if (wire_header && wire_query && wire_footer) {
                std::memcpy(dns_data + offset, wire_header->data(), wire_header->size());
                offset += wire_header->size();

                std::memcpy(dns_data + offset, wire_query->data(), wire_query->size());
                offset += wire_query->size();

                std::memcpy(dns_data + offset, wire_footer->data(), wire_footer->size());
                offset += wire_footer->size();

                return offset;
        }

        return 0;
}

/**
 * @brief Process a single query response and write it as PCAP packet
 * @param gqr DNS query response
 * @param pcap Output PCAP file stream
 */
void process_query_response(const CDNS::GenericQueryResponse& gqr, std::ofstream& pcap) {
        struct pcap_parts pp;

        pp.dns_len = construct_dns(pp.dns, gqr);
        pp.total_len += pp.dns_len;

        pp.udp_len = construct_udp(pp.udp, gqr, pp.dns, pp.dns_len);
        pp.total_len += pp.udp_len;

        pp.ip_len = construct_ip(pp.ip, gqr, pp.udp_len + pp.dns_len);
        pp.total_len += pp.ip_len;

        udp_checksum(&pp, pp.ip_len > 30);

        pp.eth_len = construct_eth(pp.eth, gqr);
        pp.total_len += pp.eth_len;

        write_pcap_packet(pcap, pp, *gqr.ts);
}

/**
 * @brief Check if query has all required fields for processing
 * @param gqr DNS query response
 * @return True if valid query
 */
bool is_valid_query(const CDNS::GenericQueryResponse& gqr) {
        return gqr.client_ip.has_value()
            && gqr.server_ip.has_value()
            && gqr.client_port.has_value()
            && gqr.server_port.has_value()
            && gqr.ts.has_value();
}

/**
 * @brief Read all blocks from CDNS file and process queries
 * @param reader CDNS reader object
 * @param pcap Output PCAP file stream
 */
void process_cdns_data(CDNS::CdnsReader& reader, std::ofstream& pcap) {
        bool end = false;

        while (true) {
                CDNS::CdnsBlockRead block = reader.read_block(end);
                if (end) break;

                std::cout << block.string() << std::endl;

                while (true) {
                        CDNS::GenericQueryResponse gqr = block.read_generic_qr(end);
                        if (end) break;

                        if (!is_valid_query(gqr)) continue;

                        process_query_response(gqr, pcap);
                }
        }
}

/**
 * @brief Write global PCAP header
 * @param pcap Output PCAP file stream
 */
void write_global_header(std::ofstream& pcap) {
        PcapGlobalHeader gh;
        pcap.write(reinterpret_cast<const char*>(&gh), sizeof(gh));
}

/**
 * @file cdns_query_pcap.cpp
 * @brief Implementation of cdns_query_pcap command line tool.
 *
 * cdns_query_pcap reads a C-DNS file and converts DNS queries/responses
 * into a PCAP file containing DNS packets. \n
 * Usage: cdns_query_pcap <INPUT_FILE.cdns> <OUTPUT_FILE.pcap> \n
 * Options: \n
 *      -h                  : Print this help message and exit \n
 */
static void print_help()
{
        std::cout << "cdns_query_pcap:" << std::endl;
        std::cout << "Reads a C-DNS file and converts DNS queries/responses into a PCAP file." << std::endl;
        std::cout << "Usage: cdns_query_pcap <INPUT_FILE.cdns> <OUTPUT_FILE.pcap>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-h                  : Print this help message and exit" << std::endl;
}

int main(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
                if (std::string(argv[i]) == "-h") {
                print_help();
                return 0;
                }
        }

        if (argc != 3) {
                std::cerr << "No input or output file specified!\n";
                print_help();
                return 1;
        }

        const std::string input_file = argv[1];
        const std::string output_file = argv[2];

        std::ifstream ifs(input_file, std::ios::binary);
        if (!ifs.is_open()) {
                std::cerr << "Failed to open input file: " << input_file << '\n';
                return 1;
        }

        std::ofstream pcap(output_file, std::ios::binary);
        if (!pcap.is_open()) {
                std::cerr << "Failed to open output PCAP file: " << output_file << '\n';
                return 1;
        }

        CDNS::CdnsReader reader(ifs);
        write_global_header(pcap);
        process_cdns_data(reader, pcap);

        return 0;
}
