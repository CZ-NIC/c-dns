/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "block.h"
#include "cdns_encoder.h"
#include "interface.h"

std::size_t CDNS::ClassType::write(CdnsEncoder& enc)
{
    std::size_t written = 0;

    // Start ClassType map
    written += enc.write_map_start(get_map_index(CDNS::ClassTypeMapIndex::class_type_size));

    // Write type
    written += enc.write(get_map_index(CDNS::ClassTypeMapIndex::type));
    written += enc.write(type);

    // Write class
    written += enc.write(get_map_index(CDNS::ClassTypeMapIndex::class_));
    written += enc.write(class_);

    return written;
}

void CDNS::ClassType::read(CdnsDecoder& dec)
{
    bool is_type = false;
    bool is_class_ = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(ClassTypeMapIndex::type):
                type = dec.read_unsigned();
                is_type = true;
                break;
            case get_map_index(ClassTypeMapIndex::class_):
                class_ = dec.read_unsigned();
                is_class_ = true;
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_type || ! is_class_)
        throw CdnsDecoderException("ClassType from input stream missing one of mandatory items");
}

std::size_t CDNS::QueryResponseSignature::write(CdnsEncoder& enc)
{
    std::size_t fields = !!server_address_index + !!server_port + !!qr_transport_flags + !!qr_type
                         + !!qr_sig_flags + !!query_opcode + !!qr_dns_flags + !!query_rcode
                         + !!query_classtype_index + !!query_qdcount + !!query_ancount + !!query_nscount
                         + !!query_arcount + !!query_edns_version + !!query_udp_size + !!query_opt_rdata_index
                         + !!response_rcode;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Query Response Signature map
    written += enc.write_map_start(fields);

    // Write Server address index
    if (server_address_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::server_address_index));
        written += enc.write(server_address_index.value());
    }

    // Write Server port
    if (server_port) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::server_port));
        written += enc.write(server_port.value());
    }

    // Write Qr transport flags
    if (qr_transport_flags) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_transport_flags));
        written += enc.write(qr_transport_flags.value());
    }

    // Write Qr type
    if (qr_type) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_type));
        written += enc.write(get_map_index(qr_type.value()));
    }

    // Write Qr signature flags
    if (qr_sig_flags) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_sig_flags));
        written += enc.write(qr_sig_flags.value());
    }

    // Write Query OpCode
    if (query_opcode) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_opcode));
        written += enc.write(query_opcode.value());
    }

    // Write Qr DNS flags
    if (qr_dns_flags) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::qr_dns_flags));
        written += enc.write(qr_dns_flags.value());
    }

    // Write Query RCode
    if (query_rcode) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_rcode));
        written += enc.write(query_rcode.value());
    }

    // Write Query classtype index
    if (query_classtype_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_classtype_index));
        written += enc.write(query_classtype_index.value());
    }

    // Write Query qdcount
    if (query_qdcount) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_qdcount));
        written += enc.write(query_qdcount.value());
    }

    // Write Query ancount
    if (query_ancount) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_ancount));
        written += enc.write(query_ancount.value());
    }

    // Write Query nscount
    if (query_nscount) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_nscount));
        written += enc.write(query_nscount.value());
    }

    // Write Query arcount
    if (query_arcount) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_arcount));
        written += enc.write(query_arcount.value());
    }

    // Write Query EDNS version
    if (query_edns_version) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_edns_version));
        written += enc.write(query_edns_version.value());
    }

    // Write Query UDP size
    if (query_udp_size) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_udp_size));
        written += enc.write(query_udp_size.value());
    }

    // Write Query OPT RDATA index
    if (query_opt_rdata_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::query_opt_rdata_index));
        written += enc.write(query_opt_rdata_index.value());
    }

    // Write Response RCode
    if (response_rcode) {
        written += enc.write(get_map_index(CDNS::QueryResponseSignatureMapIndex::response_rcode));
        written += enc.write(response_rcode.value());
    }

    return written;
}

void CDNS::QueryResponseSignature::read(CdnsDecoder& dec)
{
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(QueryResponseSignatureMapIndex::server_address_index):
                server_address_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::server_port):
                server_port = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::qr_transport_flags):
                qr_transport_flags = static_cast<QueryResponseTransportFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(QueryResponseSignatureMapIndex::qr_type):
                qr_type = static_cast<QueryResponseTypeValues>(dec.read_unsigned());
                break;
            case get_map_index(QueryResponseSignatureMapIndex::qr_sig_flags):
                qr_sig_flags = static_cast<QueryResponseFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_opcode):
                query_opcode = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::qr_dns_flags):
                qr_dns_flags = static_cast<DNSFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_rcode):
                query_rcode = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_classtype_index):
                query_classtype_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_qdcount):
                query_qdcount = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_ancount):
                query_ancount = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_nscount):
                query_nscount = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_arcount):
                query_arcount = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_edns_version):
                query_edns_version = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_udp_size):
                query_udp_size = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::query_opt_rdata_index):
                query_opt_rdata_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseSignatureMapIndex::response_rcode):
                response_rcode = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

std::size_t CDNS::Question::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    // Start Question map
    written += enc.write_map_start(get_map_index(CDNS::QuestionMapIndex::question_size));

    // Write Name index
    written += enc.write(get_map_index(CDNS::QuestionMapIndex::name_index));
    written += enc.write(name_index);

    // Write Classtype index
    written += enc.write(get_map_index(CDNS::QuestionMapIndex::classtype_index));
    written += enc.write(classtype_index);

    return written;
}

void CDNS::Question::read(CdnsDecoder& dec)
{
    bool is_name_index = false;
    bool is_classtype_index = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(QuestionMapIndex::name_index):
                name_index = dec.read_unsigned();
                is_name_index = true;
                break;
            case get_map_index(QuestionMapIndex::classtype_index):
                classtype_index = dec.read_unsigned();
                is_classtype_index = true;
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_name_index || !is_classtype_index)
        throw CdnsDecoderException("Question from input stream missing one of mandatory items");
}

std::size_t CDNS::RR::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = 2 + !!ttl + !!rdata_index;

    // Start RR map
    written += enc.write_map_start(fields);

    // Write Name index
    written += enc.write(get_map_index(CDNS::RrMapIndex::name_index));
    written += enc.write(name_index);

    // Write Classtype index
    written += enc.write(get_map_index(CDNS::RrMapIndex::classtype_index));
    written += enc.write(classtype_index);

    // Write TTL
    if (ttl) {
        written += enc.write(get_map_index(CDNS::RrMapIndex::ttl));
        written += enc.write(ttl.value());
    }

    // Write RDATA index
    if (rdata_index) {
        written += enc.write(get_map_index(CDNS::RrMapIndex::rdata_index));
        written += enc.write(rdata_index.value());
    }

    return written;
}

void CDNS::RR::read(CdnsDecoder& dec)
{
    bool is_name_index = false;
    bool is_classtype_index = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(RrMapIndex::name_index):
                name_index = dec.read_unsigned();
                is_name_index = true;
                break;
            case get_map_index(RrMapIndex::classtype_index):
                classtype_index = dec.read_unsigned();
                is_classtype_index = true;
                break;
            case get_map_index(RrMapIndex::ttl):
                ttl = dec.read_unsigned();
                break;
            case get_map_index(RrMapIndex::rdata_index):
                rdata_index = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_name_index || !is_classtype_index)
        throw CdnsDecoderException("RR from input stream missing one of mandatory items");
}

std::size_t CDNS::MalformedMessageData::write(CdnsEncoder& enc)
{
    std::size_t fields = !!server_address_index + !!server_port + !!mm_transport_flags + !!mm_payload;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Malformed message data map
    written += enc.write_map_start(fields);

    // Write Server address index
    if (server_address_index) {
        written += enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::server_address_index));
        written += enc.write(server_address_index.value());
    }

    // Write Server port
    if (server_port) {
        written += enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::server_port));
        written += enc.write(server_port.value());
    }

    // Write MM transport flags
    if (mm_transport_flags) {
        written += enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::mm_transport_flags));
        written += enc.write(mm_transport_flags.value());
    }

    // Write MM payload
    if (mm_payload) {
        written += enc.write(get_map_index(CDNS::MalformedMessageDataMapIndex::mm_payload));
        written += enc.write_bytestring(mm_payload.value());
    }

    return written;
}

void CDNS::MalformedMessageData::read(CdnsDecoder& dec)
{
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch(dec.read_unsigned()) {
            case get_map_index(MalformedMessageDataMapIndex::server_address_index):
                server_address_index = dec.read_unsigned();
                break;
            case get_map_index(MalformedMessageDataMapIndex::server_port):
                server_port = dec.read_unsigned();
                break;
            case get_map_index(MalformedMessageDataMapIndex::mm_transport_flags):
                mm_transport_flags = static_cast<QueryResponseTransportFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(MalformedMessageDataMapIndex::mm_payload):
                mm_payload = dec.read_bytestring();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

std::size_t CDNS::ResponseProcessingData::write(CdnsEncoder& enc)
{
    std::size_t fields = !!bailiwick_index + !!processing_flags;

    if (fields == 0)
        return 0;

    std::size_t written;

    // Start Response processing data map
    written += enc.write_map_start(fields);

    // Write Bailiwick index
    if (bailiwick_index) {
        written += enc.write(get_map_index(CDNS::ResponseProcessingDataMapIndex::bailiwick_index));
        written += enc.write(bailiwick_index.value());
    }

    // Write Processing flags
    if (processing_flags) {
        written += enc.write(get_map_index(CDNS::ResponseProcessingDataMapIndex::processing_flags));
        written += enc.write(processing_flags.value());
    }

    return written;
}

void CDNS::ResponseProcessingData::read(CdnsDecoder& dec)
{
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(ResponseProcessingDataMapIndex::bailiwick_index):
                bailiwick_index = dec.read_unsigned();
                break;
            case get_map_index(ResponseProcessingDataMapIndex::processing_flags):
                processing_flags = static_cast<ResponseProcessingFlagsMask>(dec.read_unsigned());
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

std::size_t CDNS::QueryResponseExtended::write(CdnsEncoder& enc)
{
    std::size_t fields = !!question_index + !!answer_index + !!authority_index + !!additional_index;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start of Query Response Extended map
    written += enc.write_map_start(fields);

    // Write Question index
    if (question_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::question_index));
        written += enc.write(question_index.value());
    }

    // Write Answer index
    if (answer_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::answer_index));
        written += enc.write(answer_index.value());
    }

    // Write Authority index
    if (authority_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::authority_index));
        written += enc.write(authority_index.value());
    }

    // Write Additional index
    if (additional_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseExtendedMapIndex::additional_index));
        written += enc.write(additional_index.value());
    }

    return written;
}

void CDNS::QueryResponseExtended::read(CdnsDecoder& dec)
{
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_unsigned()) {
            case get_map_index(QueryResponseExtendedMapIndex::question_index):
                question_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseExtendedMapIndex::answer_index):
                answer_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseExtendedMapIndex::authority_index):
                authority_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseExtendedMapIndex::additional_index):
                additional_index = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

std::size_t CDNS::BlockPreamble::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = 1 + !!block_parameters_index;

    // Start Block preamble map
    written += enc.write_map_start(fields);

    // Write Earliest time
    written += enc.write(get_map_index(CDNS::BlockPreambleMapIndex::earliest_time));
    written += earliest_time.write(enc);

    // Write Block parameters index
    if (block_parameters_index) {
        written += enc.write(get_map_index(CDNS::BlockPreambleMapIndex::block_parameters_index));
        written += enc.write(block_parameters_index.value());
    }

    return written;
}

std::size_t CDNS::BlockStatistics::write(CdnsEncoder& enc)
{
    std::size_t fields = !!processed_messages + !!qr_data_items + !!unmatched_queries + !!unmatched_responses
                         + !!discarded_opcode + !!malformed_items;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Block statistics map
    written += enc.write_map_start(fields);

    // Write Processed messages
    if (processed_messages) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::processed_messages));
        written += enc.write(processed_messages.value());
    }

    // Write Qr data items
    if (qr_data_items) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::qr_data_items));
        written += enc.write(qr_data_items.value());
    }

    // Write Unmatech queries
    if (unmatched_queries) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_queries));
        written += enc.write(unmatched_queries.value());
    }

    // Write Unmatched responses
    if (unmatched_responses) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_responses));
        written += enc.write(unmatched_responses.value());
    }

    // Write Discarded opcode
    if (discarded_opcode) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::discarded_opcode));
        written += enc.write(discarded_opcode.value());
    }

    // Write Malformed items
    if (malformed_items) {
        written += enc.write(get_map_index(CDNS::BlockStatisticsMapIndex::malformed_items));
        written += enc.write(malformed_items.value());
    }

    return written;
}

std::size_t CDNS::QueryResponse::write(CdnsEncoder& enc, const Timestamp& earliest, const uint64_t& ticks_per_second)
{
    std::size_t fields = !!time_offset + !!client_address_index + !!client_port + !!transaction_id
                         + !!qr_signature_index + !!client_hoplimit + !!response_delay + !! query_name_index
                         + !!query_size + !!response_size + !!response_processing_data + !!query_extended
                         + !!response_extended;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Query Response map
    written += enc.write_map_start(fields);

    // Write Time offset
    if (time_offset) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::time_offset));
        written += enc.write(static_cast<uint64_t>(time_offset->get_time_offset(earliest, ticks_per_second)));
    }

    // Write Client address index
    if (client_address_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_address_index));
        written += enc.write(client_address_index.value());
    }

    // Write Client port
    if (client_port) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_port));
        written += enc.write(client_port.value());
    }

    // Write Transaction ID
    if (transaction_id) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::transaction_id));
        written += enc.write(transaction_id.value());
    }

    // Write Qr signature index
    if (qr_signature_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::qr_signature_index));
        written += enc.write(qr_signature_index.value());
    }

    // Write Client hoplimit
    if (client_hoplimit) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::client_hoplimit));
        written += enc.write(client_hoplimit.value());
    }

    // Write Response delay
    if (response_delay) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_delay));
        written += enc.write(response_delay.value());
    }

    // Write Query name index
    if (query_name_index) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_name_index));
        written += enc.write(query_name_index.value());
    }

    // Write Query size
    if (query_size) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_size));
        written += enc.write(query_size.value());
    }

    // Write Response size
    if (response_size) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_size));
        written += enc.write(response_size.value());
    }

    // Write Response processing data
    if (response_processing_data) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_processing_data));
        written += response_processing_data.value().write(enc);
    }

    // Write Query extended
    if (query_extended) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::query_extended));
        written += query_extended.value().write(enc);
    }

    // Write Response extended
    if (response_extended) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::response_extended));
        written += response_extended.value().write(enc);
    }

    return written;
}

std::size_t CDNS::AddressEventCount::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = 3 + !!ae_code + !!ae_transport_flags;

    // Start Address event count map
    written += enc.write_map_start(fields);

    // Write Ae type
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_type));
    written += enc.write(get_map_index(ae_type));

    // Write Ae code
    if (ae_code) {
        written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_code));
        written += enc.write(ae_code.value());
    }

    // Write Ae tranport flags
    if (ae_transport_flags) {
        written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_transport_flags));
        written += enc.write(ae_transport_flags.value());
    }

    // Write Ae address index
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_address_index));
    written += enc.write(ae_address_index);

    // Write Ae count
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_count));
    written += enc.write(ae_count);

    return written;
}

std::size_t CDNS::MalformedMessage::write(CdnsEncoder& enc, const Timestamp& earliest, const uint64_t& ticks_per_second)
{
    std::size_t fields = !!time_offset + !!client_address_index + !!client_port + !!message_data_index;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Malformed message map
    written += enc.write_map_start(fields);

    // Write Time offset
    if (time_offset) {
        written += enc.write(get_map_index(CDNS::MalformedMessageMapIndex::time_offset));
        written += enc.write(static_cast<uint64_t>(time_offset->get_time_offset(earliest, ticks_per_second)));
    }

    // Write Client address index
    if (client_address_index) {
        written += enc.write(get_map_index(CDNS::MalformedMessageMapIndex::client_address_index));
        written += enc.write(client_address_index.value());
    }

    // Write Client port
    if (client_port) {
        written += enc.write(get_map_index(CDNS::MalformedMessageMapIndex::client_port));
        written += enc.write(client_port.value());
    }

    // Write Message data index
    if (message_data_index) {
        written += enc.write(get_map_index(CDNS::MalformedMessageMapIndex::message_data_index));
        written += enc.write(message_data_index.value());
    }

    return written;
}

std::size_t CDNS::StringItem::write(CdnsEncoder& enc)
{
    return enc.write_bytestring(data);
}

std::size_t CDNS::IndexListItem::write(CdnsEncoder& enc)
{
    if (list.size() == 0)
        return 0;

    std::size_t written = 0;

    written += enc.write_array_start(list.size());
    for (auto& index : list) {
        written += enc.write(index);
    }

    return written;
}

std::size_t CDNS::CdnsBlock::write_blocktables(CdnsEncoder& enc, std::size_t& fields)
{
    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Block tables map
    written += enc.write_map_start(fields);

    // Write IP addresses
    if (!!m_ip_address.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::ip_address));
        written += enc.write_array_start(m_ip_address.size());
        for (auto& ip : m_ip_address) {
            written += ip.write(enc);
        }
    }

    // Write Classtype
    if (!!m_classtype.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::classtype));
        written += enc.write_array_start(m_classtype.size());
        for (auto& classtype : m_classtype) {
            written += classtype.write(enc);
        }
    }

    // Write Name_RDATA
    if (!!m_name_rdata.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::name_rdata));
        written += enc.write_array_start(m_name_rdata.size());
        for (auto& name_rdata : m_name_rdata) {
            written += name_rdata.write(enc);
        }
    }

    // Write QR Signature
    if (!!m_qr_sig.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::qr_sig));
        written += enc.write_array_start(m_qr_sig.size());
        for (auto& qr_sig : m_qr_sig) {
            written += qr_sig.write(enc);
        }
    }

    // Write Question list
    if (!!m_qlist.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::qlist));
        written += enc.write_array_start(m_qlist.size());
        for (auto& qlist : m_qlist) {
            written += qlist.write(enc);
        }
    }

    // Write Question RRs
    if (!!m_qrr.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::qrr));
        written += enc.write_array_start(m_qrr.size());
        for (auto& qrr : m_qrr) {
            written += qrr.write(enc);
        }
    }

    // Write RR list
    if (!!m_rrlist.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::rrlist));
        written += enc.write_array_start(m_rrlist.size());
        for (auto& rrlist : m_rrlist) {
            written += rrlist.write(enc);
        }
    }

    // Write Resource records
    if (!!m_rr.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::rr));
        written += enc.write_array_start(m_rr.size());
        for (auto& rr : m_rr) {
            written += rr.write(enc);
        }
    }

    // Write Malformed messsage data
    if (!!m_malformed_messages.size()) {
        written += enc.write(get_map_index(CDNS::BlockTablesMapIndex::malformed_message_data));
        written += enc.write_array_start(m_malformed_message_data.size());
        for (auto& mmd : m_malformed_message_data) {
            written += mmd.write(enc);
        }
    }

    return written;
}

std::size_t CDNS::CdnsBlock::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t blocktable_fields = !!m_ip_address.size() + !!m_classtype.size() + !!m_name_rdata.size()
                         + !!m_qr_sig.size() + !!m_qlist.size() + !!m_qrr.size() + !!m_rrlist.size()
                         + !!m_rr.size() + !!m_malformed_messages.size();

    std::size_t fields = 1 + !!m_block_statistics + !!blocktable_fields + !!m_query_responses.size()
                         + !!m_address_event_counts.size() + !!m_malformed_messages.size();

    // Start C-DNS Block
    written += enc.write_map_start(fields);

    // Write Block preamble
    written += enc.write(get_map_index(CDNS::BlockMapIndex::block_preamble));
    written += m_block_preamble.write(enc);

    // Write Block statistics
    if (m_block_statistics) {
        written += enc.write(get_map_index(CDNS::BlockMapIndex::block_statistics));
        written += m_block_statistics->write(enc);
    }

    // Write Block tables
    if (blocktable_fields > 0) {
        written += enc.write(get_map_index(CDNS::BlockMapIndex::block_tables));
        written += write_blocktables(enc, blocktable_fields);
    }

    // Write Query Responses
    if (!!m_query_responses.size()) {
        written += enc.write(get_map_index(CDNS::BlockMapIndex::query_responses));
        written += enc.write_array_start(m_query_responses.size());
        for (auto& qr : m_query_responses) {
            written += qr.write(enc, m_block_preamble.earliest_time, m_block_parameters.storage_parameters.ticks_per_second);
        }
    }

    // Write Address event counts
    if (!!m_address_event_counts.size()) {
        written += enc.write(get_map_index(CDNS::BlockMapIndex::address_event_counts));
        written += enc.write_array_start(m_address_event_counts.size());
        for (auto& aec : m_address_event_counts) {
            auto tmp = aec.first;
            tmp.ae_count = aec.second;
            written += tmp.write(enc);
        }
    }

    // Write Malformed messages
    if (!!m_malformed_messages.size()) {
        written += enc.write(get_map_index(CDNS::BlockMapIndex::malformed_messages));
        written += enc.write_array_start(m_malformed_messages.size());
        for (auto& mm : m_malformed_messages) {
            written += mm.write(enc, m_block_preamble.earliest_time, m_block_parameters.storage_parameters.ticks_per_second);
        }
    }

    return written;
}

CDNS::index_t CDNS::CdnsBlock::add_generic_qlist(const std::vector<GenericResourceRecord>& glist) {
    std::vector<index_t> qlist;

    for (auto grr : glist) {
        // Check for mandatory items in Question
        if (!grr.name || !grr.classtype)
            continue;

        Question q;
        q.name_index = add_name_rdata(*grr.name);
        q.classtype_index = add_classtype(*grr.classtype);
        qlist.push_back(add_question(q));
    }

    return add_question_list(qlist);
}

CDNS::index_t CDNS::CdnsBlock::add_generic_rrlist(const std::vector<GenericResourceRecord>& glist) {
    const uint8_t& rr_hints = m_block_parameters.storage_parameters.storage_hints.rr_hints;
    std::vector<index_t> rrlist;

    for (auto grr : glist) {
        // Check for mandatory items in Resource record
        if (!grr.name || !grr.classtype)
            continue;

        RR rr;
        rr.name_index = add_name_rdata(*grr.name);
        rr.classtype_index = add_classtype(*grr.classtype);
        if ((rr_hints & RrHintsMask::ttl) && grr.ttl)
            rr.ttl = *grr.ttl;
        if ((rr_hints & RrHintsMask::rdata_index) && grr.rdata)
            rr.rdata_index = add_name_rdata(*grr.rdata);
        rrlist.push_back(add_rr(rr));
    }

    return add_rr_list(rrlist);
}

bool CDNS::CdnsBlock::add_question_response_record(const GenericQueryResponse& gr,
                                                   const boost::optional<BlockStatistics>& stats)
{
    uint32_t qr_hints = m_block_parameters.storage_parameters.storage_hints.query_response_hints;
    uint32_t qr_sig_hints = m_block_parameters.storage_parameters.storage_hints.query_response_signature_hints;
    uint8_t rr_hints = m_block_parameters.storage_parameters.storage_hints.rr_hints;
    uint8_t other_data_hints = m_block_parameters.storage_parameters.storage_hints.other_data_hints;

    // Check if it'll be the first record in the block and set earliest time if yes
    if (gr.ts && ((m_query_responses.size() == 0 && m_malformed_messages.size() == 0) ||
                  (*gr.ts < m_block_preamble.earliest_time)))
        m_block_preamble.earliest_time = *gr.ts;

    /**
     * Fill Query Response
     */

    QueryResponse qr;
    bool qr_filled = false;

    // Time offset
    if ((qr_hints & QueryResponseHintsMask::time_offset) && gr.ts) {
        qr.time_offset = *gr.ts;
        qr_filled = true;
    }

    // Client IP address
    if ((qr_hints & QueryResponseHintsMask::client_address_index) && gr.client_ip) {
        qr.client_address_index = add_ip_address(*gr.client_ip);
        qr_filled = true;
    }

    // Client port
    if ((qr_hints & QueryResponseHintsMask::client_port) && gr.client_port) {
        qr.client_port = *gr.client_port;
        qr_filled = true;
    }

    // DNS transaction ID
    if ((qr_hints & QueryResponseHintsMask::transaction_id) && gr.transaction_id) {
        qr.transaction_id = *gr.transaction_id;
        qr_filled = true;
    }

    // Fill Query Response Signature
    if (qr_hints & QueryResponseHintsMask::qr_signature_index) {
        QueryResponseSignature qrs;
        bool qrs_filled = false;

        // Server IP address
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::server_address_index) && gr.server_ip) {
            qrs.server_address_index = add_ip_address(*gr.server_ip);
            qrs_filled = true;
        }

        // Server port
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::server_port) && gr.server_port) {
            qrs.server_port = *gr.server_port;
            qrs_filled = true;
        }

        // Transport flags (IP version, transport protocol, trailing data)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_transport_flags) && gr.qr_transport_flags) {
            qrs.qr_transport_flags = *gr.qr_transport_flags;
            qrs_filled = true;
        }

        // Query type (stub, resolver, etc.)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_type) && gr.qr_type) {
            qrs.qr_type = *gr.qr_type;
            qrs_filled = true;
        }

        // QR Signature flags (is query, is response, etc.)
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_sig_flags) && gr.qr_sig_flags) {
            qrs.qr_sig_flags = *gr.qr_sig_flags;
            qrs_filled = true;
        }

        // Query OpCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_opcode) && gr.query_opcode) {
            qrs.query_opcode = *gr.query_opcode;
            qrs_filled = true;
        }

        // DNS header flags
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::qr_dns_flags) && gr.qr_dns_flags) {
            qrs.qr_dns_flags = *gr.qr_dns_flags;
            qrs_filled = true;
        }

        // Query RCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_rcode) && gr.query_rcode) {
            qrs.query_rcode = *gr.query_rcode;
            qrs_filled = true;
        }

        // Query question type and class
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_classtype_index) && gr.query_classtype) {
            qrs.query_classtype_index = add_classtype(*gr.query_classtype);
            qrs_filled = true;
        }

        // Query question count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_qdcount) && gr.query_qdcount) {
            qrs.query_qdcount = *gr.query_qdcount;
            qrs_filled = true;
        }

        // Query answer count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_ancount) && gr.query_ancount) {
            qrs.query_ancount = *gr.query_ancount;
            qrs_filled = true;
        }

        // Query authority records count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_nscount) && gr.query_nscount) {
            qrs.query_nscount = *gr.query_nscount;
            qrs_filled = true;
        }

        // Query additional records count
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_arcount) && gr.query_arcount) {
            qrs.query_arcount = *gr.query_arcount;
            qrs_filled = true;
        }

        // EDNS version
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_edns_version) && gr.query_edns_version) {
            qrs.query_edns_version = *gr.query_edns_version;
            qrs_filled = true;
        }

        // EDNS UDP size
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_udp_size) && gr.query_udp_size) {
            qrs.query_udp_size = *gr.query_udp_size;
            qrs_filled = true;
        }

        // EDNS record's rdata
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_opt_rdata_index) && gr.opt_rdata) {
            qrs.query_opt_rdata_index = add_name_rdata(*gr.opt_rdata);
            qrs_filled = true;
        }

        // Response RCode
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::response_rcode) && gr.response_rcode) {
            qrs.response_rcode = *gr.response_rcode;
            qrs_filled = true;
        }

        // Add Query Response Signature to Block table
        if (qrs_filled) {
            qr.qr_signature_index = add_qr_signature(qrs);
            qr_filled = true;
        }
    }

    // Client hoplimit (TTL)
    if ((qr_hints & QueryResponseHintsMask::client_hoplimit) && gr.client_hoplimit) {
        qr.client_hoplimit = *gr.client_hoplimit;
        qr_filled = true;
    }

    // Response delay
    if ((qr_hints & QueryResponseHintsMask::response_delay) && gr.response_delay) {
        qr.response_delay = *gr.response_delay;
        qr_filled = true;
    }

    // Question name
    if ((qr_hints & QueryResponseHintsMask::query_name_index) && gr.query_name) {
        qr.query_name_index = add_name_rdata(*gr.query_name);
        qr_filled = true;
    }

    // Query DNS size
    if ((qr_hints & QueryResponseHintsMask::query_size) && gr.query_size) {
        qr.query_size = *gr.query_size;
        qr_filled = true;
    }

    // Response DNS size
    if ((qr_hints & QueryResponseHintsMask::response_size) && gr.response_size) {
        qr.response_size = *gr.response_size;
        qr_filled = true;
    }

    // Fill Response Processing Data
    if (qr_hints & QueryResponseHintsMask::response_processing_data) {
        ResponseProcessingData rpd;
        bool rpd_filled = false;

        // Response Bailiwick
        if (gr.bailiwick) {
            rpd.bailiwick_index = add_name_rdata(*gr.bailiwick);
            rpd_filled = true;
        }

        // Response processing flags (Is response from cache?)
        if (gr.processing_flags) {
            rpd.processing_flags = *gr.processing_flags;
            rpd_filled = true;
        }

        if (rpd_filled) {
            qr.response_processing_data = rpd;
            qr_filled = true;
        }
    }

    // Fill Query Extended
    QueryResponseExtended qe;
    bool qe_filled = false;

    if ((qr_hints & QueryResponseHintsMask::query_question_sections) && gr.query_questions
        && (gr.query_questions->size() > 0)) {
        qe.question_index = add_generic_qlist(*gr.query_questions);
        qe_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::query_answer_sections) && gr.query_answers
        && (gr.query_answers->size() > 0)) {
        qe.answer_index = add_generic_rrlist(*gr.query_answers);
        qe_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::query_authority_sections) && gr.query_authority
        && (gr.query_authority->size() > 0)) {
        qe.authority_index = add_generic_rrlist(*gr.query_authority);
        qe_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::query_additional_sections) && gr.query_additional
        && (gr.query_additional->size() > 0)) {
        qe.additional_index = add_generic_rrlist(*gr.query_additional);
        qe_filled = true;
    }

    if (qe_filled) {
        qr.query_extended = qe;
        qr_filled = true;
    }

    // Fill Response Extended
    QueryResponseExtended re;
    bool re_filled = false;

    if ((qr_hints & QueryResponseHintsMask::query_question_sections) && gr.response_questions
        && (gr.response_questions->size() > 0)) {
        re.question_index = add_generic_qlist(*gr.response_questions);
        re_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::response_answer_sections) && gr.response_answers
        && (gr.response_answers->size() > 0)) {
        re.answer_index = add_generic_rrlist(*gr.response_answers);
        re_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::response_authority_sections) && gr.response_authority
        && (gr.response_authority->size() > 0)) {
        re.authority_index = add_generic_rrlist(*gr.response_authority);
        re_filled = true;
    }

    if ((qr_hints & QueryResponseHintsMask::response_additional_sections) && gr.response_additional
        && (gr.response_additional->size() > 0)) {
        re.additional_index = add_generic_rrlist(*gr.response_additional);
        re_filled = true;
    }

    if (re_filled) {
        qr.response_extended = re;
        qr_filled = true;
    }

    /**
     * Add Query Response to the Block
     */
    if (qr_filled)
        m_query_responses.push_back(qr);

    // Update block statistics
    if (stats)
        m_block_statistics = stats;

    // Indicate if the Block is full (DNS record is inserted anyway, the limit is just a guideline)
    return full() ? true : false;
}

bool CDNS::CdnsBlock::add_question_response_record(const QueryResponse& qr,
                                                   const boost::optional<BlockStatistics>& stats)
{
    std::size_t fields = !!qr.time_offset + !!qr.client_address_index + !!qr.client_port
                            + !!qr.transaction_id + !!qr.qr_signature_index + !!qr.client_hoplimit
                            + !!qr.response_delay + !!qr.query_name_index + !!qr.query_size
                            + !!qr.response_size + !!qr.response_processing_data + !!qr.query_extended
                            + !!qr.response_extended;

    if (fields == 0)
        return full() ? true : false;

    if (qr.time_offset && ((m_query_responses.size() == 0 && m_malformed_messages.size() == 0) ||
                            (qr.time_offset < m_block_preamble.earliest_time)))
        m_block_preamble.earliest_time = *qr.time_offset;

    m_query_responses.push_back(qr);

    if (stats)
        m_block_statistics = stats;

    return full() ? true : false;
}

bool CDNS::CdnsBlock::add_addres_event_count(const GenericAddressEventCount& gaec,
                                             const boost::optional<BlockStatistics>& stats)
{
    // Check if Address Event Counts are buffered in this Block
    if (!(m_block_parameters.storage_parameters.storage_hints.other_data_hints & OtherDataHintsMask::address_event_counts))
        return false;

    // Check if mandatory fiels are present
    if (!gaec.ae_type || !gaec.ip_address)
        return false;

    /**
     * Fill Address Event Count
     */

    AddressEventCount aec;

    // Address event type
    if (gaec.ae_type)
        aec.ae_type = *gaec.ae_type;

    // Address event code
    if (gaec.ae_code)
        aec.ae_code = *gaec.ae_code;

    // Tranport flags
    if (gaec.ae_transport_flags)
        aec.ae_transport_flags = *gaec.ae_transport_flags;

    // IP address
    if (gaec.ip_address)
        aec.ae_address_index = add_ip_address(*gaec.ip_address);

    /*
     * Count Address Event to the Block
     */
    auto found = m_address_event_counts.find(aec);
    if (found != m_address_event_counts.end())
        found->second++;
    else
        m_address_event_counts[aec] = 1;

    // Update block statistics
    if (stats)
        m_block_statistics = stats;

    // Indicate if the Block is full (Address Event is inserted anyway, the limit is just a guideline)
    return full() ? true : false;
}

bool CDNS::CdnsBlock::add_addres_event_count(const AddressEventCount& aec,
                                             const boost::optional<BlockStatistics>& stats)
{
    if (!(m_block_parameters.storage_parameters.storage_hints.other_data_hints & OtherDataHintsMask::address_event_counts))
        return false;

    auto found = m_address_event_counts.find(aec);
    if (found != m_address_event_counts.end())
        found->second++;
    else
        m_address_event_counts[aec] = 1;

    if (stats)
        m_block_statistics = stats;

    return full() ? true : false;
}

bool CDNS::CdnsBlock::add_malformed_message(const GenericMalformedMessage& gmm,
                                            const boost::optional<BlockStatistics>& stats)
{
    // Check if Malformed messages are buffered in this Block
    if (!(m_block_parameters.storage_parameters.storage_hints.other_data_hints & OtherDataHintsMask::malformed_messages))
        return false;

    // Check if it'll be the first item in the block and set earliest time if yes
    if (gmm.ts && ((m_query_responses.size() == 0 && m_malformed_messages.size() == 0) ||
                  (*gmm.ts < m_block_preamble.earliest_time)))
        m_block_preamble.earliest_time = *gmm.ts;

    /**
     * Fill Malformed Message
     */

    MalformedMessage mm;
    bool mm_filled = false;

    // Time offset
    if (gmm.ts) {
        mm.time_offset = *gmm.ts;
        mm_filled = true;
    }

    // Client IP address
    if (gmm.client_ip) {
        mm.client_address_index = add_ip_address(*gmm.client_ip);
        mm_filled = true;
    }

    // Client port
    if (gmm.client_port) {
        mm.client_port = *gmm.client_port;
        mm_filled = true;
    }

    // Fill Malformed Message Data
    MalformedMessageData mmd;
    bool mmd_filled = false;

    // Server address
    if (gmm.server_ip) {
        mmd.server_address_index = add_ip_address(*gmm.server_ip);
        mmd_filled = true;
    }

    // Server port
    if (gmm.server_port) {
        mmd.server_port = *gmm.server_port;
        mmd_filled = true;
    }

    // Transport flags
    if (gmm.mm_transport_flags) {
        mmd.mm_transport_flags = *gmm.mm_transport_flags;
        mmd_filled = true;
    }

    // Malformed message payload
    if (gmm.mm_payload) {
        mmd.mm_payload = *gmm.mm_payload;
        mmd_filled = true;
    }

    // Add Malformed Message Data to Block table
    if (mmd_filled) {
        mm.message_data_index = add_malformed_message_data(mmd);
        mm_filled = true;
    }

    /**
     * Add Malformed Message to the Block
     */
    if (mm_filled)
        m_malformed_messages.push_back(mm);

    // Update block statistics
    if (stats)
        m_block_statistics = stats;

    // Indicate if the Block is full (Malformed message is inserted anyway, the limit is just a guideline)
    return full() ? true : false;
}

bool CDNS::CdnsBlock::add_malformed_message(const MalformedMessage& mm,
                                            const boost::optional<BlockStatistics>& stats)
{
    std::size_t fields = !!mm.time_offset + !!mm.client_address_index + !!mm.client_port
                            + !!mm.message_data_index;

    if (fields == 0)
        return full() ? true : false;

    if (mm.time_offset && ((m_query_responses.size() == 0 && m_malformed_messages.size() == 0) ||
                            (mm.time_offset < m_block_preamble.earliest_time)))
        m_block_preamble.earliest_time = *mm.time_offset;

    m_malformed_messages.push_back(mm);

    if (stats)
        m_block_statistics = stats;

    return full() ? true : false;
}
