#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <deque>
#include <vector>
#include <optional>

#include "format_specification.h"
#include "block_table.h"
#include "hash.h"
#include "file_preamble.h"

namespace CDNS {

    static constexpr uint64_t MILIS_PER_SEC = 1000UL;
    static constexpr uint64_t MICROS_PER_SEC = 1000000UL;
    static constexpr uint64_t NANOS_PER_SEC = 1000000000UL;

    /**
     * @brief Simple timestamp representation
     */
    struct Timestamp {
        /**
         * @brief Default constructor
         */
        Timestamp() : m_secs(0), m_ticks(0) {}

        /**
         * @brief Construct a new Timestamp object
         * @param secs Seconds since the start of UNIX epoch
         * @param ticks Subsecond number of ticks in any resolution
         */
        Timestamp(uint64_t secs, uint64_t ticks) : m_secs(secs), m_ticks(ticks) {}

        /**
         * @brief Operator `smaller than`
         * @param rhs Timestamp to compare with
         * @return `true` if the Timestamp denotes earlier time than the `rhs` one
         */
        bool operator<(const Timestamp& rhs) const {
            if (m_secs < rhs.m_secs)
                return true;

            if ((m_secs == rhs.m_secs) && (m_ticks < rhs.m_ticks))
                return true;

            return false;
        }

        /**
         * @brief Operator `smaller or equal than`
         * @param rhs Timestamp to compare with
         * @return `true` if the Timestamp denotes earlier or identical time as the `rhs` one
         */
        bool operator<=(const Timestamp& rhs) const {
            if (m_secs < rhs.m_secs)
                return true;

            if ((m_secs == rhs.m_secs) && (m_ticks <= rhs.m_ticks))
                return true;

            return false;
        }

        uint64_t m_secs;
        uint64_t m_ticks;
    };

    /**
     * @brief Block table's ClassType structure
     */
    struct ClassType {
        ClassType() : type(0), class_(0) {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const ClassType& rhs) const {
            return (type == rhs.type) && (class_ == rhs.class_);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const ClassType& rhs) const {
            return !(*this == rhs);
        }

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

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const QueryResponseSignature& rhs) const {
            return (server_address_index == rhs.server_address_index) &&
                   (server_port == rhs.server_port) &&
                   (qr_transport_flags == rhs.qr_transport_flags) &&
                   (qr_type == rhs.qr_type) && (qr_sig_flags == rhs.qr_sig_flags) &&
                   (query_opcode == rhs.query_opcode) &&  (qr_dns_flags == rhs.qr_dns_flags) &&
                   (query_rcode == rhs.query_rcode) &&
                   (query_classtype_index == rhs.query_classtype_index) &&
                   (query_qdcount == rhs.query_qdcount) && (query_ancount == rhs.query_ancount) &&
                   (query_nscount == rhs.query_nscount) && (query_arcount == rhs.query_arcount) &&
                   (query_edns_version == rhs.query_edns_version) &&
                   (query_udp_size == rhs.query_udp_size) &&
                   (query_opt_rdata_index == rhs.query_opt_rdata_index) &&
                   (response_rcode == rhs.response_rcode);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const QueryResponseSignature& rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const QueryResponseSignature& key() const {
            return *this;
        }

        std::optional<index_t> server_address_index;
        std::optional<uint16_t> server_port;
        std::optional<QueryResponseTransportFlagsMask> qr_transport_flags;
        std::optional<QueryResponseTypeValues> qr_type;
        std::optional<QueryResponseFlagsMask> qr_sig_flags;
        std::optional<uint8_t> query_opcode;
        std::optional<DNSFlagsMask> qr_dns_flags;
        std::optional<uint16_t> query_rcode;
        std::optional<index_t> query_classtype_index;
        std::optional<uint16_t> query_qdcount;
        std::optional<uint32_t> query_ancount;
        std::optional<uint16_t> query_nscount;
        std::optional<uint16_t> query_arcount;
        std::optional<uint8_t> query_edns_version;
        std::optional<uint16_t> query_udp_size;
        std::optional<index_t> query_opt_rdata_index;
        std::optional<uint16_t> response_rcode;
    };

    /**
     * @brief Block table's Question structure
     */
    struct Question {
        Question() : name_index(0), classtype_index(0) {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const Question& rhs) const {
            return (name_index == rhs.name_index) && (classtype_index == rhs.classtype_index);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const Question& rhs) const {
            return !(*this == rhs);
        }

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
        RR() : name_index(0), classtype_index(0) {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const RR& rhs) const {
            return (name_index == rhs.name_index) && (classtype_index == rhs.classtype_index) &&
                   (ttl == rhs.ttl) && (rdata_index == rhs.rdata_index);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const RR& rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const RR& key() const {
            return *this;
        }

        index_t name_index;
        index_t classtype_index;
        std::optional<uint8_t> ttl;
        std::optional<index_t> rdata_index;
    };

    /**
     * @brief Block table's Malformed Message Data structure
     */
    struct MalformedMessageData {

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const MalformedMessageData& rhs) const {
            return (server_address_index == rhs.server_address_index) &&
                   (server_port == rhs.server_port) &&
                   (mm_transport_flags == rhs.mm_transport_flags) &&
                   (mm_payload == rhs.mm_payload);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const MalformedMessageData& rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const MalformedMessageData& key() const {
            return *this;
        }

        std::optional<index_t> server_address_index;
        std::optional<uint16_t> server_port;
        std::optional<QueryResponseTransportFlagsMask> mm_transport_flags;
        std::optional<std::string> mm_payload;
    };

    /**
     * @brief Block table's Response Processing Data structure
     */
    struct ResponseProcessingData {
        std::optional<index_t> bailiwick_index;
        std::optional<ResponseProcessingFlagsMask> processing_flags;
    };

    /**
     * @brief Block table's Query Response Extended structure
     */
    struct QueryResponseExtended {
        std::optional<index_t> question_index;
        std::optional<index_t> answer_index;
        std::optional<index_t> authority_index;
        std::optional<index_t> additional_index;
    };

    /**
     * @brief Block preamble structure
     */
    struct BlockPreamble {
        std::optional<Timestamp> earliest_time;
        std::optional<index_t> block_parameters_index;
    };

    /**
     * @brief Block statistics structure
     */
    struct BlockStatistics {
        std::optional<unsigned> processed_messages;
        std::optional<unsigned> qr_data_items;
        std::optional<unsigned> unmatched_queries;
        std::optional<unsigned> unmatched_responses;
        std::optional<unsigned> discarded_opcode;
        std::optional<unsigned> malformed_items;
    };

    /**
     * @brief QueryResponse item structure
     */
    struct QueryResponse {
        std::optional<Timestamp> time_offset;
        std::optional<index_t> client_address_index;
        std::optional<uint16_t> client_port;
        std::optional<uint16_t> transaction_id;
        std::optional<index_t> qr_signature_index;
        std::optional<uint8_t> client_hoplimit;
        std::optional<int64_t> response_delay;
        std::optional<index_t> query_name_index;
        std::optional<std::size_t> query_size;
        std::optional<std::size_t> response_size;
        std::optional<ResponseProcessingData> response_processing_data;
        std::optional<QueryResponseExtended> query_extended;
        std::optional<QueryResponseExtended> response_extended;
    };

    /**
     * @brief Address Event Count item structure
     */
    struct AddressEventCount {
        AddressEventCount() : ae_type(), ae_address_index(0) {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const AddressEventCount& rhs) const {
            return (ae_type == rhs.ae_type) && (ae_code == rhs.ae_code) &&
                   (ae_transport_flags == rhs.ae_transport_flags) &&
                   (ae_address_index == rhs.ae_address_index);
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const AddressEventCount& rhs) const {
            return !(*this == rhs);
        }

        /**
         * @brief Return reference to itself as key for unordered_map
         */
        const AddressEventCount& key() const {
            return *this;
        }

        AddressEventTypeValues ae_type;
        std::optional<uint8_t> ae_code;
        std::optional<QueryResponseTransportFlagsMask> ae_transport_flags;
        index_t ae_address_index;
        //uint64_t ae_count;
    };

    /**
     * @brief Malformed Message item structure
     */
    struct MalformedMessage {
        std::optional<uint64_t> time_offset;
        std::optional<index_t> client_address_index;
        std::optional<uint16_t> client_port;
        std::optional<index_t> message_data_index;
    };

    /**
     * @brief Structure representing byte string item
     */
    struct StringItem {
        StringItem() : data() {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const StringItem& rhs) const {
            return data == rhs.data;
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const StringItem& rhs) const {
            return data != rhs.data;
        }

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
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const IndexListItem& rhs) const {
            return list == rhs.list;
        }

        /**
         * @brief Inequality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items aren't equal
         */
        bool operator!=(const IndexListItem& rhs) const {
            return list != rhs.list;
        }

        /**
         * @brief Return reference to the list as key for unordered_map
         */
        const std::vector<index_t>& key() const {
            return list;
        }

        std::vector<index_t> list;
    };

     /**
     * @brief Generic structure for holding 1 DNS record before storing it into Block
     *
     * Servers as "interface" between the C-DNS library and its user
     */
    struct generic_qr {
        /**
         * @brief Default constructor
         */
        generic_qr() : ts(nullptr), client_ip(nullptr), client_port(nullptr), transaction_id(nullptr),
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
        generic_qr(Timestamp *time, std::string *c_ip, uint16_t *c_port, uint16_t *id, std::string *s_ip,
                   uint16_t *s_port, QueryResponseTransportFlagsMask *tr_flags, QueryResponseTypeValues *type,
                   QueryResponseFlagsMask *sig_flags, uint8_t *opcode, DNSFlagsMask *dns_flags,
                   uint16_t *q_rcode, ClassType *q_classtype, uint16_t *q_qdcount, uint16_t *q_ancount,
                   uint16_t *q_nscount, uint16_t *q_arcount, uint8_t *q_edns_v, uint16_t *q_udp_size,
                   std::string *opt, uint16_t *r_rcode, uint8_t *hoplimit, int64_t *r_delay,
                   std::string *q_name, std::size_t *q_size, std::size_t *r_size, std::string *bwick,
                   ResponseProcessingFlagsMask *proc_flags, std::vector<std::string*> *q_questions,
                   std::vector<std::string*> *q_answers, std::vector<std::string*> *q_authority,
                   std::vector<std::string*> *q_additional, std::vector<std::string*> *r_questions,
                   std::vector<std::string*> *r_answers, std::vector<std::string*> *r_authority,
                   std::vector<std::string*> *r_additional)
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

        Timestamp *ts;
        std::string *client_ip;
        uint16_t *client_port;
        uint16_t *transaction_id;

        // Query Response Signature
        std::string *server_ip;
        uint16_t *server_port;
        QueryResponseTransportFlagsMask *qr_transport_flags;
        QueryResponseTypeValues *qr_type;
        QueryResponseFlagsMask *qr_sig_flags;
        uint8_t *query_opcode;
        DNSFlagsMask *qr_dns_flags;
        uint16_t *query_rcode;
        ClassType *query_classtype;
        uint16_t *query_qdcount;
        uint16_t *query_ancount;
        uint16_t *query_nscount;
        uint16_t *query_arcount;
        uint8_t *query_edns_version;
        uint16_t *query_udp_size;
        std::string *opt_rdata;
        uint16_t *response_rcode;

        uint8_t *client_hoplimit;
        int64_t *response_delay;
        std::string *query_name;
        std::size_t *query_size;
        std::size_t *response_size;

        //Response Processing Data
        std::string *bailiwick;
        ResponseProcessingFlagsMask *processing_flags;

        /**
         * @todo Can't be just string but the whole Question or RR structure
         */
        std::vector<std::string*> *query_questions;
        std::vector<std::string*> *query_answers;
        std::vector<std::string*> *query_authority;
        std::vector<std::string*> *query_additional;
        std::vector<std::string*> *response_questions;
        std::vector<std::string*> *response_answers;
        std::vector<std::string*> *response_authority;
        std::vector<std::string*> *response_additional;
    };

    /**
     * @brief Generic structure for holding 1 Address Event Count before storing it into Block
     *
     * Servers as "interface" between the C-DNS library and its user
     */
    struct generic_aec {
        /**
         * @brief Default constructor
         */
        generic_aec() : ae_type(nullptr), ae_code(nullptr), ae_transport_flags(nullptr),
                        ip_address(nullptr) {}

        /**
         * @brief Construct a new generic aec object
         */
        generic_aec(AddressEventTypeValues *type, uint8_t *code, QueryResponseTransportFlagsMask *tr_flags,
                    std::string *ip)
                    : ae_type(type), ae_code(code), ae_transport_flags(tr_flags), ip_address(ip) {}

        AddressEventTypeValues *ae_type;
        uint8_t *ae_code;
        QueryResponseTransportFlagsMask *ae_transport_flags;
        std::string *ip_address;
    };

    /**
     * @brief Generic structure for holding 1 Malformed Message before storing it into Block
     *
     * Servers as "interface" between the C-DNS library and its user
     */
    struct generic_mm {
        /**
         * @brief Default constructor
         */
        generic_mm() : ts(nullptr), client_ip(nullptr), client_port(nullptr), server_ip(nullptr),
                       server_port(nullptr), mm_transport_flags(nullptr), mm_payload(nullptr) {}

        /**
         * @brief Construct a new generic mm object
         */
        generic_mm(Timestamp *time, std::string *c_ip, uint16_t *c_port, std::string *s_ip,
                   uint16_t *s_port, QueryResponseTransportFlagsMask *tr_flags, std::string *payload)
                   : ts(time), client_ip(c_ip), client_port(c_port), server_ip(s_ip), server_port(s_port),
                     mm_transport_flags(tr_flags), mm_payload(payload) {}

        Timestamp *ts;
        std::string *client_ip;
        uint16_t *client_port;

        // Malformed Message Data
        std::string *server_ip;
        uint16_t *server_port;
        QueryResponseTransportFlagsMask *mm_transport_flags;
        std::string *mm_payload;
    };

    /**
     * @brief Class representing C-DNS block
     */
    class CdnsBlock {
        public:

        CdnsBlock() {}

        /**
         * @todo void write_cbor(CborEncoder &enc);
         */

        /**
         * @brief Add IP address to IP address Block table
         * @param address IP address to add to the Block table
         * @return Index of the IP address in Block table
         */
        index_t add_ip_address(const std::string& address) {
            index_t ret;

            if (!m_ip_address.find(address, ret)) {
                StringItem tmp;
                tmp.data = address;
                ret = m_ip_address.add_value(std::move(tmp));
            }

            return ret;
        }

        /**
         * @brief Add Classtype structure to Classtype Block table
         * @param classtype Classtype structure to add to the Block table
         * @return Index of the Classtype in Block table
         */
        index_t add_classtype(const ClassType& classtype) {
            return m_classtype.add(classtype);
        }

        /**
         * @brief Add NAME or RDATA to name_rdata Block table
         * @param nrd NAME or RDATA to add to the Block table
         * @return Index of the NAME or RDATA in Block table
         */
        index_t add_name_rdata(const std::string& nrd) {
            index_t ret;

            if (!m_name_rdata.find(nrd, ret)) {
                StringItem tmp;
                tmp.data = nrd;
                ret = m_name_rdata.add_value(std::move(tmp));
            }

            return ret;
        }

        /**
         * @brief Add Query Response Signature to QR Signature Block table
         * @param qr_sig Query Response Signature to add to Block table
         * @return Index of the Query Response Signature in Block table
         */
        index_t add_qr_signature(const QueryResponseSignature& qr_sig) {
            return m_qr_sig.add(qr_sig);
        }

        /**
         * @brief Add Question list to Question list Block table
         * @param qlist Question list to add to Block table
         * @return Index of the Question list in Block table
         */
        index_t add_question_list(const std::vector<index_t>& qlist) {
            index_t ret;

            if (!m_qlist.find(qlist, ret)) {
                IndexListItem tmp;
                tmp.list = qlist;
                ret = m_qlist.add_value(std::move(tmp));
            }

            return ret;
        }

        /**
         * @brief Add Question record to Question record Block table
         * @param qrr Question record to add to Block table
         * @return Index of the Question record in Block table
         */
        index_t add_question(const Question& qrr) {
            return m_qrr.add(qrr);
        }

        /**
         * @brief Add Resource record list to Resource record list Block table
         * @param rrlist Resource record list to add to Block table
         * @return Index of the Resource record list in Block table
         */
        index_t add_rr_list(const std::vector<index_t>& rrlist) {
            index_t ret;

            if (!m_rrlist.find(rrlist, ret)) {
                IndexListItem tmp;
                tmp.list = rrlist;
                ret = m_rrlist.add_value(std::move(tmp));
            }

            return ret;
        }

        /**
         * @brief Add Resource record to Resource record Block table
         * @param rr Resource record to add to Block table
         * @return Index of the Resource record in Block table
         */
        index_t add_rr(const RR& rr) {
            return m_rr.add(rr);
        }

        /**
         * @brief Add Malformed message data to Malformed message data Block table
         * @param mmd Malformed message data to add to Block table
         * @return Index of the Malformed message data in Block table
         */
        index_t add_malformed_message_data(const MalformedMessageData& mmd) {
            return m_malformed_message_data.add(mmd);
        }

        /**
         * @brief Add new DNS record to C-DNS block. Uses generic structure to hold all DNS record data and
         * adds it to the Block
         * @param qr Generic structure holding data of new DNS record
         * @param bp Block parameters with hints for the current Block
         * @return `true` when the DNS record is inserted into the Block
         */
        bool add_question_response_record(const generic_qr& qr, const BlockParameters& bp);

        /**
         * @todo Add new Address Event Count to C-DNS block.
         */
        //bool add_address_event_count(const generic_aec& aec, const BlockParameters& bp);

        /**
         * @todo Add new Malformed Message to C-DNS block.
         */
        //bool add_malformed_message(const generic_mm& mm, const BlockParameters& bp);

        BlockPreamble m_block_preamble;
        std::optional<BlockStatistics> m_block_statistics;

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