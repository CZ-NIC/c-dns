#include "block.h"
#include "cdns_encoder.h"

void CDNS::Timestamp::write(CdnsEncoder& enc)
{
    // Start Timestamp array
    enc.write_array_start(2);

    // Write Seconds
    enc.write(m_secs);

    // Write Ticks
    enc.write(m_ticks);
}

void CDNS::ClassType::write(CdnsEncoder& enc)
{
    // Start ClassType map
    enc.write_map_start(get_map_index(CDNS::ClassTypeMapIndex::class_type_size));

    // Write type
    enc.write(get_map_index(CDNS::ClassTypeMapIndex::type));
    enc.write(type);

    // Write class
    enc.write(get_map_index(CDNS::ClassTypeMapIndex::class_));
    enc.write(class_);
}

void CDNS::QueryResponseSignature::write(CdnsEncoder& enc)
{
    std::size_t fields = !!server_address_index + !!server_port + !!qr_transport_flags + !!qr_type
                         + !!qr_sig_flags + !!query_opcode + !!qr_dns_flags + !!query_rcode
                         + !!query_classtype_index + !!query_qdcount + !!query_ancount + !!query_nscount
                         + !!query_arcount + !!query_edns_version + !!query_udp_size + !!query_opt_rdata_index
                         + !!response_rcode;

    if (fields == 0)
        return;

    // Start Query Response Signature map
    enc.write_map_start(fields);

    // Write Server address index
    if (server_address_index) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::server_address_index));
        enc.write(server_address_index.value());
    }

    // Write Server port
    if (server_port) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::server_port));
        enc.write(server_port.value());
    }

    // Write Qr transport flags
    if (qr_transport_flags) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_transport_flags));
        enc.write(qr_transport_flags.value());
    }

    // Write Qr type
    if (qr_type) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_type));
        enc.write(get_map_index(qr_type.value()));
    }

    // Write Qr signature flags
    if (qr_sig_flags) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_sig_flags));
        enc.write(qr_sig_flags.value());
    }

    // Write Query OpCode
    if (query_opcode) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_opcode));
        enc.write(query_opcode.value());
    }

    // Write Qr DNS flags
    if (qr_dns_flags) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_dns_flags));
        enc.write(qr_dns_flags.value());
    }

    // Write Query RCode
    if (query_rcode) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_rcode));
        enc.write(query_rcode.value());
    }

    // Write Query classtype index
    if (query_classtype_index) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_classtype_index));
        enc.write(query_classtype_index.value());
    }

    // Write Query qdcount
    if (query_qdcount) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_qdcount));
        enc.write(query_qdcount.value());
    }

    // Write Query ancount
    if (query_ancount) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_ancount));
        enc.write(query_ancount.value());
    }

    // Write Query nscount
    if (query_nscount) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_nscount));
        enc.write(query_nscount.value());
    }

    // Write Query arcount
    if (query_arcount) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_arcount));
        enc.write(query_arcount.value());
    }

    // Write Query EDNS version
    if (query_edns_version) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_edns_version));
        enc.write(query_edns_version.value());
    }

    // Write Query UDP size
    if (query_udp_size) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_udp_size));
        enc.write(query_udp_size.value());
    }

    // Write Query OPT RDATA index
    if (query_opt_rdata_index) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_opt_rdata_index));
        enc.write(query_opt_rdata_index.value());
    }

    // Write Response RCode
    if (response_rcode) {
        enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::response_rcode));
        enc.write(response_rcode.value());
    }
}

void CDNS::Question::write(CdnsEncoder& enc)
{
    // Start Question map
    enc.write_map_start(get_map_index(CDNS::QuestionMapIndex::question_size));

    // Write Name index
    enc.write(get_map_index(CDNS::QuestionMapIndex::name_index));
    enc.write(name_index);

    // Write Classtype index
    enc.write(get_map_index(CDNS::QuestionMapIndex::classtype_index));
    enc.write(classtype_index);
}

void CDNS::RR::write(CdnsEncoder& enc)
{
    std::size_t fields = 2 + !!ttl + !!rdata_index;

    // Start RR map
    enc.write_map_start(fields);

    // Write Name index
    enc.write(get_map_index(CDNS::RrMapIndex::name_index));
    enc.write(name_index);

    // Write Classtype index
    enc.write(get_map_index(CDNS::RrMapIndex::classtype_index));
    enc.write(classtype_index);

    // Write TTL
    if (ttl) {
        enc.write(get_map_index(CDNS::RrMapIndex::ttl));
        enc.write(ttl.value());
    }

    // Write RDATA index
    if (rdata_index) {
        enc.write(get_map_index(CDNS::RrMapIndex::rdata_index));
        enc.write(rdata_index.value());
    }
}

void CDNS::MalformedMessageData::write(CdnsEncoder& enc)
{
    std::size_t fields = !!server_address_index + !!server_port + !!mm_transport_flags + !!mm_payload;

    if (fields == 0)
        return;

    // Start Malformed message data map
    enc.write_map_start(fields);

    // Write Server address index
    if (server_address_index) {
        enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::server_address_index));
        enc.write(server_address_index.value());
    }

    // Write Server port
    if (server_port) {
        enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::server_port));
        enc.write(server_port.value());
    }

    // Write MM transport flags
    if (mm_transport_flags) {
        enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::mm_transport_flags));
        enc.write(mm_transport_flags.value());
    }

    // Write MM payload
    if (mm_payload) {
        enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::mm_payload));
        enc.write_bytestring(mm_payload.value());
    }
}

void CDNS::ResponseProcessingData::write(CdnsEncoder& enc)
{
    std::size_t fields = !!bailiwick_index + !!processing_flags;

    if (fields == 0)
        return;

    // Start Response processing data map
    enc.write_map_start(fields);

    // Write Bailiwick index
    if (bailiwick_index) {
        enc.write(get_map_index(CDNS::ResponseProcessingDataMapIndex::bailiwick_index));
        enc.write(bailiwick_index.value());
    }

    // Write Processing flags
    if (processing_flags) {
        enc.write(get_map_index(CDNS::ResponseProcessingDataMapIndex::processing_flags));
        enc.write(processing_flags.value());
    }
}

void CDNS::QueryResponseExtended::write(CdnsEncoder& enc)
{
    std::size_t fields = !!question_index + !!answer_index + !!authority_index + !!additional_index;

    if (fields == 0)
        return;

    // Start of Query Response Extended map
    enc.write_map_start(fields);

    // Write Question index
    if (question_index) {
        enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::question_index));
        enc.write(question_index.value());
    }

    // Write Answer index
    if (answer_index) {
        enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::answer_index));
        enc.write(answer_index.value());
    }

    // Write Authority index
    if (authority_index) {
        enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::authority_index));
        enc.write(authority_index.value());
    }

    // Write Additional index
    if (additional_index) {
        enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::additional_index));
        enc.write(additional_index.value());
    }
}

void CDNS::BlockPreamble::write(CdnsEncoder& enc)
{
    std::size_t fields = !!earliest_time + !!block_parameters_index;

    if (fields == 0)
        return;

    // Start Block preamble map
    enc.write_map_start(fields);

    // Write Earliest time
    if (earliest_time) {
        enc.write(get_map_index(CDNS::BlockPreambleMapIndex::earliest_time));
        earliest_time.value().write(enc);
    }

    // Write Block parameters index
    if (block_parameters_index) {
        enc.write(get_map_index(CDNS::BlockPreambleMapIndex::block_parameters_index));
        enc.write(block_parameters_index.value());
    }
}

void CDNS::BlockStatistics::write(CdnsEncoder& enc)
{
    std::size_t fields = !!processed_messages + !!qr_data_items + !!unmatched_queries + !!unmatched_responses
                         + !!discarded_opcode + !!malformed_items;

    if (fields == 0)
        return;

    // Start Block statistics map
    enc.write_map_start(fields);

    // Write Processed messages
    if (processed_messages) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::processed_messages));
        enc.write(processed_messages.value());
    }

    // Write Qr data items
    if (qr_data_items) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::qr_data_items));
        enc.write(qr_data_items.value());
    }

    // Write Unmatech queries
    if (unmatched_queries) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_queries));
        enc.write(unmatched_queries.value());
    }

    // Write Unmatched responses
    if (unmatched_responses) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_responses));
        enc.write(unmatched_responses.value());
    }

    // Write Discarded opcode
    if (discarded_opcode) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::discarded_opcode));
        enc.write(discarded_opcode.value());
    }

    // Write Malformed items
    if (malformed_items) {
        enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::malformed_items));
        enc.write(malformed_items.value());
    }
}

void CDNS::QueryResponse::write(CdnsEncoder& enc)
{
    std::size_t fields = !!time_offset + !!client_address_index + !!client_port + !!transaction_id
                         + !!qr_signature_index + !!client_hoplimit + !!response_delay + !! query_name_index
                         + !!query_size + !!response_size + !!response_processing_data + !!query_extended
                         + !!response_extended;

    if (fields == 0)
        return;

    // Start Query Response map
    enc.write_map_start(fields);

    // Write Time offset
    if (time_offset) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::time_offset));
        uint64_t offset = 0; /** @todo calculate time offset */
        enc.write(offset);
    }

    // Write Client address index
    if (client_address_index) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_address_index));
        enc.write(client_address_index.value());
    }

    // Write Client port
    if (client_port) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_port));
        enc.write(client_port.value());
    }

    // Write Transaction ID
    if (transaction_id) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::transaction_id));
        enc.write(transaction_id.value());
    }

    // Write Qr signature index
    if (qr_signature_index) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::qr_signature_index));
        enc.write(qr_signature_index.value());
    }

    // Write Client hoplimit
    if (client_hoplimit) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_hoplimit));
        enc.write(client_hoplimit.value());
    }

    // Write Response delay
    if (response_delay) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_delay));
        enc.write(response_delay.value());
    }

    // Write Query name index
    if (query_name_index) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_name_index));
        enc.write(query_name_index.value());
    }

    // Write Query size
    if (query_size) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_size));
        enc.write(query_size.value());
    }

    // Write Response size
    if (response_size) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_size));
        enc.write(response_size.value());
    }

    // Write Response processing data
    if (response_processing_data) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_processing_data));
        response_processing_data.value().write(enc);
    }

    // Write Query extended
    if (query_extended) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_extended));
        query_extended.value().write(enc);
    }

    // Write Response extended
    if (response_extended) {
        enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_extended));
        response_extended.value().write(enc);
    }
}

void CDNS::AddressEventCount::write(CdnsEncoder& enc)
{
    std::size_t fields = 3 + !!ae_code + !!ae_transport_flags;

    // Start Address event count map
    enc.write_map_start(fields);

    // Write Ae type
    enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_type));
    enc.write(get_map_index(ae_type));

    // Write Ae code
    if (ae_code) {
        enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_code));
        enc.write(ae_code.value());
    }

    // Write Ae tranport flags
    if (ae_transport_flags) {
        enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_transport_flags));
        enc.write(ae_transport_flags.value());
    }

    // Write Ae address index
    enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_address_index));
    enc.write(ae_address_index);

    // Write Ae count
    enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_count));
    enc.write(ae_count);
}

void CDNS::MalformedMessage::write(CdnsEncoder& enc)
{
    std::size_t fields = !!time_offset + !!client_address_index + !!client_port + !!message_data_index;

    if (fields == 0)
        return;

    // Start Malformed message map
    enc.write_map_start(fields);

    // Write Time offset
    if (time_offset) {
        enc.write(get_map_index(CDNS::MalformedMessageMapIndex::time_offset));
        uint64_t offset = 0; /** @todo Calculate time offset */
        enc.write(offset);
    }

    // Write Client address index
    if (client_address_index) {
        enc.write(get_map_index(CDNS::MalformedMessageMapIndex::client_address_index));
        enc.write(client_address_index.value());
    }

    // Write Client port
    if (client_port) {
        enc.write(get_map_index(CDNS::MalformedMessageMapIndex::client_port));
        enc.write(client_port.value());
    }

    // Write Message data index
    if (message_data_index) {
        enc.write(get_map_index(CDNS::MalformedMessageMapIndex::message_data_index));
        enc.write(message_data_index.value());
    }
}

void CDNS::StringItem::write(CdnsEncoder& enc)
{
    enc.write_bytestring(data);
}

void CDNS::IndexListItem::write(CdnsEncoder& enc)
{
    if (list.size() == 0)
        return;

    enc.write_array_start(list.size());
    for (auto& index : list) {
        enc.write(index);
    }
}

void CDNS::CdnsBlock::write_blocktables(CdnsEncoder& enc)
{
    std::size_t fields = !!m_ip_address.size() + !!m_classtype.size() + !!m_name_rdata.size()
                         + !!m_qr_sig.size() + !!m_qlist.size() + !!m_qrr.size() + !!m_rrlist.size()
                         + !!m_rr.size() + !!m_malformed_messages.size();

    if (fields == 0)
        return;

    // Start Block tables map
    enc.write_map_start(fields);

    // Write IP addresses
    if (!!m_ip_address.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::ip_address));
        enc.write_array_start(m_ip_address.size());
        for (auto& ip : m_ip_address) {
            ip.write(enc);
        }
    }

    // Write Classtype
    if (!!m_classtype.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::classtype));
        enc.write_array_start(m_classtype.size());
        for (auto& classtype : m_classtype) {
            classtype.write(enc);
        }
    }

    // Write Name_RDATA
    if (!!m_name_rdata.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::name_rdata));
        enc.write_array_start(m_name_rdata.size());
        for (auto& name_rdata : m_name_rdata) {
            name_rdata.write(enc);
        }
    }

    // Write QR Signature
    if (!!m_qr_sig.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::qr_sig));
        enc.write_array_start(m_qr_sig.size());
        for (auto& qr_sig : m_qr_sig) {
            qr_sig.write(enc);
        }
    }

    // Write Question list
    if (!!m_qlist.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::qlist));
        enc.write_array_start(m_qlist.size());
        for (auto& qlist : m_qlist) {
            qlist.write(enc);
        }
    }

    // Write Question RRs
    if (!!m_qrr.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::qrr));
        enc.write_array_start(m_qrr.size());
        for (auto& qrr : m_qrr) {
            qrr.write(enc);
        }
    }

    // Write RR list
    if (!!m_rrlist.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::rrlist));
        enc.write_array_start(m_rrlist.size());
        for (auto& rrlist : m_rrlist) {
            rrlist.write(enc);
        }
    }

    // Write Resource records
    if (!!m_rr.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::rr));
        enc.write_array_start(m_rr.size());
        for (auto& rr : m_rr) {
            rr.write(enc);
        }
    }

    // Write Malformed messsage data
    if (!!m_malformed_messages.size()) {
        enc.write(get_map_index(CDNS::BlockTablesMapIndex::malformed_message_data));
        enc.write_array_start(m_malformed_message_data.size());
        for (auto& mmd : m_malformed_message_data) {
            mmd.write(enc);
        }
    }
}

void CDNS::CdnsBlock::write(CdnsEncoder& enc)
{

}

bool CDNS::CdnsBlock::add_question_response_record(const GenericQueryResponse& gr, const BlockParameters& bp)
{
    uint32_t qr_hints = bp.storage_parameters.storage_hints.query_response_hints;
    uint32_t qr_sig_hints = bp.storage_parameters.storage_hints.query_response_signature_hints;
    uint8_t rr_hints = bp.storage_parameters.storage_hints.rr_hints;
    uint8_t other_data_hints = bp.storage_parameters.storage_hints.other_data_hints;

    /**
     * Check if it'll be the first record in the block and set earliest time and stats if yes
     * @todo Update block statistics!!!
     */
    if (m_query_responses.size() == 0) {
        m_block_preamble.earliest_time = *gr.ts;
    }
    else if (m_block_preamble.earliest_time < *gr.ts) {
        m_block_preamble.earliest_time = *gr.ts;
    }

    /**
     * Fill Query Response
     */

    QueryResponse qr;

    // Time offset
    if ((qr_hints & QueryResponseHintsMask::time_offset) && gr.ts)
        qr.time_offset = *gr.ts;

    // Client IP address
    if ((qr_hints & QueryResponseHintsMask::client_address_index) && gr.client_ip)
        qr.client_address_index = add_ip_address(*gr.client_ip);

    // Client port
    if ((qr_hints & QueryResponseHintsMask::client_port) && gr.client_port)
        qr.client_port = *gr.client_port;

    // DNS transaction ID
    if ((qr_hints & QueryResponseHintsMask::transaction_id) && gr.transaction_id)
        qr.transaction_id = *gr.transaction_id;

    // Fill Query Response Signature
    if (qr_hints & QueryResponseHintsMask::qr_signature_index) {
        QueryResponseSignature qrs;
        bool qrs_filled = false;

        // Server IP address
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::server_address_index) && gr.server_ip) {
            qrs.server_address_index = add_ip_address(*gr.server_ip);
            qrs_filled |= !!qrs.server_address_index;
        }

        // Server port
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::server_port) && gr.server_port) {
            qrs.server_port = *gr.server_port;
            qrs_filled |= !!qrs.server_port;
        }

        // Transport flags (IP version, transport protocol, trailing data)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_transport_flags) && gr.qr_transport_flags) {
            qrs.qr_transport_flags = *gr.qr_transport_flags;
            qrs_filled |= !!qrs.qr_transport_flags;
        }

        // Query type (stub, resolver, etc.)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_type) && gr.qr_type) {
            qrs.qr_type = *gr.qr_type;
            qrs_filled |= !!qrs.qr_type;
        }

        // QR Signature flags (is query, is response, etc.)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_sig_flags) && gr.qr_sig_flags) {
            qrs.qr_sig_flags = *gr.qr_sig_flags;
            qrs_filled |= !!qrs.qr_sig_flags;
        }

        // Query OpCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_opcode) && gr.query_opcode) {
            qrs.query_opcode = *gr.query_opcode;
            qrs_filled |= !!qrs.query_opcode;
        }

        // DNS header flags
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_dns_flags) && gr.qr_dns_flags) {
            qrs.qr_dns_flags = *gr.qr_dns_flags;
            qrs_filled |= !!qrs.qr_dns_flags;
        }

        // Query RCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_rcode) && gr.query_rcode) {
            qrs.query_rcode = *gr.query_rcode;
            qrs_filled |= !!qrs.query_rcode;
        }

        // Query question type and class
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_classtype_index) && gr.query_classtype) {
            qrs.query_classtype_index = add_classtype(*gr.query_classtype);
            qrs_filled |= !!qrs.query_classtype_index;
        }

        // Query question count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_qdcount) && gr.query_qdcount) {
            qrs.query_qdcount = *gr.query_qdcount;
            qrs_filled |= !!qrs.query_qdcount;
        }

        // Query answer count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_ancount) && gr.query_ancount) {
            qrs.query_ancount = *gr.query_ancount;
            qrs_filled |= !!qrs.query_ancount;
        }

        // Query authority records count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_nscount) && gr.query_nscount) {
            qrs.query_nscount = *gr.query_nscount;
            qrs_filled |= !!qrs.query_nscount;
        }

        // Query additional records count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_arcount) && gr.query_arcount) {
            qrs.query_arcount = *gr.query_arcount;
            qrs_filled |= !!qrs.query_arcount;
        }

        // EDNS version
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_edns_version) && gr.query_edns_version) {
            qrs.query_edns_version = *gr.query_edns_version;
            qrs_filled |= !!qrs.query_edns_version;
        }

        // EDNS UDP size
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_udp_size) && gr.query_udp_size) {
            qrs.query_udp_size = *gr.query_udp_size;
            qrs_filled |= !!qrs.query_udp_size;
        }

        // EDNS record's rdata
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_opt_rdata_index) && gr.opt_rdata) {
            qrs.query_opt_rdata_index = add_name_rdata(*gr.opt_rdata);
            qrs_filled |= !!qrs.query_opt_rdata_index;
        }

        // Response RCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::response_rcode) && gr.response_rcode) {
            qrs.response_rcode = *gr.response_rcode;
            qrs_filled |= !!qrs.response_rcode;
        }

        // Add Query Response Signature to Block table
        if (qrs_filled)
            qr.qr_signature_index = add_qr_signature(qrs);
    }

    // Client hoplimit (TTL)
    if ((qr_hints & QueryResponseHintsMask::client_hoplimit) && gr.client_hoplimit)
        qr.client_hoplimit = *gr.client_hoplimit;

    // Response delay
    if ((qr_hints & QueryResponseHintsMask::response_delay) && gr.response_delay)
        qr.response_delay = *gr.response_delay;

    // Question name
    if ((qr_hints & QueryResponseHintsMask::query_name_index) && gr.query_name)
        qr.query_name_index = add_name_rdata(*gr.query_name);

    // Query DNS size
    if ((qr_hints & QueryResponseHintsMask::query_size) && gr.query_size)
        qr.query_size = *gr.query_size;

    // Response DNS size
    if ((qr_hints & QueryResponseHintsMask::response_size) && gr.response_size)
        qr.response_size = *gr.response_size;

    // Fill Response Processing Data
    if (qr_hints & QueryResponseHintsMask::response_processing_data) {
        ResponseProcessingData rpd;
        bool rpd_filled = false;

        // Response Bailiwick
        if (gr.bailiwick) {
            rpd.bailiwick_index = add_name_rdata(*gr.bailiwick);
            rpd_filled |= !!rpd.bailiwick_index;
        }

        // Response processing flags (Is response from cache?)
        if (gr.processing_flags) {
            rpd.processing_flags = *gr.processing_flags;
            rpd_filled |= !!rpd.processing_flags;
        }

        if (rpd_filled)
            qr.response_processing_data = rpd;
    }

    /**
     * @todo Query-extended and Reponse-extended fields
     */

    /**
     * Add Query Response to the Block
     */
    m_query_responses.push_back(qr);

    return true;
}