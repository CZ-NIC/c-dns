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
#include "timestamp.h"
#include "cdns_encoder.h"

namespace CDNS {
    struct GenericQueryResponse;
    struct GenericAddressEventCount;
    struct GenericMalformedMessage;

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

        /**
         * @brief Serialize the ClassType to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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

        /**
         * @brief Calculate hash for QueryResponseSignature
         * @param qrs Data to calculate hash on
         * @return Hash value for "qrs"
         */
        friend std::size_t hash_value(const QueryResponseSignature& qrs) {
            std::size_t hash = ~0U;
            if (qrs.server_address_index)
                hash = hash_value(qrs.server_address_index.value());
            if (qrs.server_port)
                hash = hash_value(qrs.server_port.value(), hash);
            if (qrs.qr_transport_flags)
                hash = hash_value(qrs.qr_transport_flags.value(), hash);
            if (qrs.qr_type)
                hash = hash_value(qrs.qr_type.value(), hash);
            if (qrs.qr_sig_flags)
                hash = hash_value(qrs.qr_sig_flags.value(), hash);
            if (qrs.query_opcode)
                hash = hash_value(qrs.query_opcode.value(), hash);
            if (qrs.qr_dns_flags)
                hash = hash_value(qrs.qr_dns_flags.value(), hash);
            if (qrs.query_rcode)
                hash = hash_value(qrs.query_rcode.value(), hash);
            if (qrs.query_classtype_index)
                hash = hash_value(qrs.query_classtype_index.value(), hash);
            if (qrs.query_qdcount)
                hash = hash_value(qrs.query_qdcount.value(), hash);
            if (qrs.query_ancount)
                hash = hash_value(qrs.query_ancount.value(), hash);
            if (qrs.query_nscount)
                hash = hash_value(qrs.query_nscount.value(), hash);
            if (qrs.query_arcount)
                hash = hash_value(qrs.query_arcount.value(), hash);
            if (qrs.query_edns_version)
                hash = hash_value(qrs.query_edns_version.value(), hash);
            if (qrs.query_udp_size)
                hash = hash_value(qrs.query_udp_size.value(), hash);
            if (qrs.query_opt_rdata_index)
                hash = hash_value(qrs.query_opt_rdata_index.value(), hash);
            if (qrs.response_rcode)
                hash = hash_value(qrs.response_rcode.value(), hash);

            return hash;
        }

        /**
         * @brief Serialize the QueryResponseSignature to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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

        /**
         * @brief Serialize the Question to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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
         * @brief Calculate hash for RR
         * @param rr Data to calculate hash on
         * @return Hash value for "rr"
         */
        friend std::size_t hash_value(const RR& rr) {
            std::size_t hash = hash_value(rr.name_index);
            hash = hash_value(rr.classtype_index, hash);
            if (rr.ttl)
                hash = hash_value(rr.ttl.value(), hash);
            if (rr.rdata_index)
                hash = hash_value(rr.rdata_index.value(), hash);

            return hash;
        }

        /**
         * @brief Returns reference to itself as key for unordered_map
         */
        const RR& key() const {
            return *this;
        }

        /**
         * @brief Serialize the RR to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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

        /**
         * @brief Calculate hash for MalformedMessageData
         * @param mmd Data to calculate hash on
         * @return Hash value for "mmd"
         */
        friend std::size_t hash_value(const MalformedMessageData& mmd) {
            std::size_t hash = ~0U;
            if (mmd.server_address_index)
                hash = hash_value(mmd.server_address_index.value());
            if (mmd.server_port)
                hash = hash_value(mmd.server_port.value(), hash);
            if (mmd.mm_transport_flags)
                hash = hash_value(mmd.mm_transport_flags.value(), hash);
            if (mmd.mm_payload)
                hash = hash_value(mmd.mm_payload.value(), hash);

            return hash;
        }

        /**
         * @brief Serialize the MalformedMessageData to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        std::optional<index_t> server_address_index;
        std::optional<uint16_t> server_port;
        std::optional<QueryResponseTransportFlagsMask> mm_transport_flags;
        std::optional<std::string> mm_payload;
    };

    /**
     * @brief Block table's Response Processing Data structure
     */
    struct ResponseProcessingData {
        /**
         * @brief Serialize the ResponseProcessingData to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        std::optional<index_t> bailiwick_index;
        std::optional<ResponseProcessingFlagsMask> processing_flags;
    };

    /**
     * @brief Block table's Query Response Extended structure
     */
    struct QueryResponseExtended {
        /**
         * @brief Serialize the QueryResponseExtended to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        std::optional<index_t> question_index;
        std::optional<index_t> answer_index;
        std::optional<index_t> authority_index;
        std::optional<index_t> additional_index;
    };

    /**
     * @brief Block preamble structure
     */
    struct BlockPreamble {
        BlockPreamble() : earliest_time() {}
        /**
         * @brief Serialize the BlockPreamble to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        Timestamp earliest_time;
        std::optional<index_t> block_parameters_index;
    };

    /**
     * @brief Block statistics structure
     */
    struct BlockStatistics {
        /**
         * @brief Serialize the BlockStatistics to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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
        /**
         * @brief Serialize the QueryResponse to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @param earliest Earliest timestamp in the Block
         * @param ticks_per_second Subsecond resolution of timestamps
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc, const Timestamp& earliest, const uint64_t& ticks_per_second);

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
        AddressEventCount() : ae_type(), ae_address_index(0), ae_count(0) {}

        /**
         * @brief Equality operator (needed for KeyRef class)
         * @param rhs Item to compare with
         * @return `true` if the items are equal
         */
        bool operator==(const AddressEventCount& rhs) const {
            return (ae_type == rhs.ae_type) && (ae_code == rhs.ae_code) &&
                   (ae_transport_flags == rhs.ae_transport_flags) &&
                   (ae_address_index == rhs.ae_address_index) &&
                   (ae_count == rhs.ae_count);
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

        /**
         * @brief Calculate hash for AddressEventCount
         * @param aec Data to calculate hash on
         * @return Hash value for "aec"
         */
        friend std::size_t hash_value(const AddressEventCount& aec) {
            std::size_t hash = hash_value(aec.ae_type);
            if (aec.ae_code)
                hash = hash_value(aec.ae_code.value(), hash);
            if (aec.ae_transport_flags)
                hash = hash_value(aec.ae_transport_flags.value(), hash);

            hash = hash_value(aec.ae_address_index, hash);
            return hash;
        }

        /**
         * @brief Serialize the AddressEventCount to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        AddressEventTypeValues ae_type;
        std::optional<uint8_t> ae_code;
        std::optional<QueryResponseTransportFlagsMask> ae_transport_flags;
        index_t ae_address_index;
        uint64_t ae_count;
    };

    /**
     * @brief Malformed Message item structure
     */
    struct MalformedMessage {
        /**
         * @brief Serialize the MalformedMessage to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @param earliest Earliest timestamp in the Block
         * @param ticks_per_second Subsecond resolution of timestamps
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc, const Timestamp& earliest, const uint64_t& ticks_per_second);

        std::optional<Timestamp> time_offset;
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
        const StringItem& key() const {
            return *this;
        }

        /**
         * @brief Calculate hash for StringItem
         * @param si Data to calculate hash on
         * @return Hash value for "si"
         */
        friend std::size_t hash_value(const StringItem& si) {
            std::size_t hash = hash_value(si.data.data(), si.data.size() * sizeof(char));
            return hash;
        }

        /**
         * @brief Serialize the StringItem to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

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
        const IndexListItem& key() const {
            return *this;
        }

        /**
         * @brief Calculate hash for IndexListItem
         * @param ili Data to calculate hash on
         * @return Hash value for "ili"
         */
        friend std::size_t hash_value(const IndexListItem& ili) {
            std::size_t hash = hash_value(ili.list.data(), ili.list.size() * sizeof(index_t));
            return hash;
        }

        /**
         * @brief Serialize the IndexListItem to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        std::vector<index_t> list;
    };

    /**
     * @brief Class representing C-DNS block
     */
    class CdnsBlock {
        public:

        /**
         * @brief Construct a new CdnsBlock object
         * @param bp Block parameters for this block
         * @param bp_index Index of the given Block parameters in corresponding File preamble
         */
        CdnsBlock(BlockParameters& bp, index_t bp_index) : m_block_parameters(bp) {
            m_block_preamble.block_parameters_index = bp_index;
        }

        ~CdnsBlock() { clear(); }

        /**
         * @brief Serialize Block to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @return Number of uncompressed bytes written
         */
        std::size_t write(CdnsEncoder& enc);

        /**
         * @brief Get index for Block parameters of this block
         * @return Index of this block's Block parameters
         */
        const index_t get_block_parameters_index() const {
            if (m_block_preamble.block_parameters_index)
                return m_block_preamble.block_parameters_index.value();
            else
                return 0;
        }

        /**
         * @brief Add IP address to IP address Block table
         * @param address IP address to add to the Block table
         * @return Index of the IP address in Block table
         */
        index_t add_ip_address(const std::string& address) {
            index_t ret;

            if (!m_ip_address.find(reinterpret_cast<const StringItem&>(address), ret)) {
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

            if (!m_name_rdata.find(reinterpret_cast<const StringItem&>(nrd), ret)) {
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

            if (!m_qlist.find(reinterpret_cast<const IndexListItem&>(qlist), ret)) {
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

            if (!m_rrlist.find(reinterpret_cast<const IndexListItem&>(rrlist), ret)) {
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
         * @throw std::exception if inserting DNS record to the Block fails
         * @return `true` if the Block is full (DNS record is still inserted), `false` otherwise
         */
        bool add_question_response_record(const GenericQueryResponse& qr);

        /**
         * @todo Add new Address Event Count to C-DNS block.
         */
        //bool add_address_event_count(const generic_aec& aec, const BlockParameters& bp);

        /**
         * @todo Add new Malformed Message to C-DNS block.
         */
        //bool add_malformed_message(const generic_mm& mm, const BlockParameters& bp);

        /**
         * @brief Get the overall number of items in Block (QueryResponse + AddressEventCount + MalformedMessage)
         * @return Current number of items in the Block
         */
        std::size_t get_item_count() const {
            return m_query_responses.size() + m_address_event_counts.size() + m_malformed_messages.size();
        }

        /**
         * @brief Clear the contents of the C-DNS Block
         */
        void clear() {
            m_block_preamble.earliest_time = {0, 0};
            if (m_block_statistics)
                m_block_statistics = std::nullopt;

            m_ip_address.clear();
            m_classtype.clear();
            m_name_rdata.clear();
            m_qr_sig.clear();
            m_qlist.clear();
            m_qrr.clear();
            m_rrlist.clear();
            m_rr.clear();
            m_malformed_message_data.clear();

            m_query_responses.clear();
            m_address_event_counts.clear();
            m_malformed_messages.clear();
        }

        private:

        /**
         * @brief Serialize Block tables to C-DNS CBOR representation
         * @param enc C-DNS encoder
         * @param fields Number of non-empty fields in Block tables map
         * @return Number of uncompressed bytes written
         */
        std::size_t write_blocktables(CdnsEncoder& enc, std::size_t& fields);

        BlockPreamble m_block_preamble;
        std::optional<BlockStatistics> m_block_statistics;

        // Block Tables
        BlockTable<StringItem> m_ip_address;
        BlockTable<ClassType> m_classtype;
        BlockTable<StringItem> m_name_rdata;
        BlockTable<QueryResponseSignature> m_qr_sig;
        BlockTable<IndexListItem> m_qlist;
        BlockTable<Question> m_qrr;
        BlockTable<IndexListItem> m_rrlist;
        BlockTable<RR> m_rr;
        BlockTable<MalformedMessageData> m_malformed_message_data;

        std::vector<QueryResponse> m_query_responses;
        std::unordered_map<AddressEventCount, uint64_t, CDNS::hash<AddressEventCount>> m_address_event_counts;
        std::vector<MalformedMessage> m_malformed_messages;

        BlockParameters& m_block_parameters;
    };
}