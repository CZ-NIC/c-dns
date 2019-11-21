#include "block.h"

bool CDNS::CdnsBlock::add_question_response_record(const generic_qr& gr, const BlockParameters& bp)
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