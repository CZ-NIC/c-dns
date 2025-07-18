/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <sstream>
#include <bitset>
#include <type_traits>

#include "block.h"
#include "cdns_encoder.h"
#include "interface.h"

std::string CDNS::ClassType::string()
{
    std::stringstream ss;

    ss << "Type: " << std::to_string(type) << std::endl;
    ss << "Class: " << std::to_string(class_) << std::endl;

    return ss.str();
}

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
    reset();
    bool is_type = false;
    bool is_class_ = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::ClassType::reset()
{
    type = 0;
    class_ = 0;
}

std::string CDNS::QueryResponseSignature::string()
{
    std::stringstream ss;

    if (server_address_index)
        ss << "Server address index: " << std::to_string(server_address_index.value()) << std::endl;

    if (server_port)
        ss << "Server port: " << std::to_string(server_port.value()) << std::endl;

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

    if (query_classtype_index)
        ss << "Query classtype index: " << std::to_string(query_classtype_index.value()) << std::endl;

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

    if (query_opt_rdata_index)
        ss << "Query OPT RDATA index: " << std::to_string(query_opt_rdata_index.value()) << std::endl;

    if (response_rcode)
        ss << "Response RCODE: " << std::to_string(response_rcode.value()) << std::endl;

    return ss.str();
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
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::QueryResponseSignature::reset()
{
    server_address_index = boost::none;
    server_port = boost::none;
    qr_transport_flags = boost::none;
    qr_type = boost::none;
    qr_sig_flags = boost::none;
    query_opcode = boost::none;
    qr_dns_flags = boost::none;
    query_rcode = boost::none;
    query_classtype_index = boost::none;
    query_qdcount = boost::none;
    query_ancount = boost::none;
    query_nscount = boost::none;
    query_arcount = boost::none;
    query_edns_version = boost::none;
    query_udp_size = boost::none;
    query_opt_rdata_index = boost::none;
    response_rcode = boost::none;
}

std::string CDNS::Question::string()
{
    std::stringstream ss;

    ss << "Name index: " << std::to_string(name_index) << std::endl;
    ss << "Classtype index: " << std::to_string(classtype_index) << std::endl;

    return ss.str();
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
    reset();
    bool is_name_index = false;
    bool is_classtype_index = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::Question::reset()
{
    name_index = 0;
    classtype_index = 0;
}

std::string CDNS::RR::string()
{
    std::stringstream ss;

    ss << "Name index: " << std::to_string(name_index) << std::endl;
    ss << "Classtype index: " << std::to_string(classtype_index) << std::endl;

    if (ttl)
        ss << "TTL: " << std::to_string(ttl.value()) << std::endl;

    if (rdata_index)
        ss << "RDATA index: " << std::to_string(rdata_index.value()) << std::endl;

    return ss.str();
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
    reset();
    bool is_name_index = false;
    bool is_classtype_index = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::RR::reset()
{
    name_index = 0;
    classtype_index = 0;
    ttl = boost::none;
    rdata_index = boost::none;
}

std::string CDNS::MalformedMessageData::string()
{
    std::stringstream ss;

    if (server_address_index)
        ss << "Server address index: " << std::to_string(server_address_index.value()) << std::endl;

    if (server_port)
        ss << "Server port: " << std::to_string(server_port.value()) << std::endl;

    if (mm_transport_flags)
        ss << "MM transport flags: " << std::bitset<8>(mm_transport_flags.value()) << std::endl;

    if (mm_payload)
        ss << "MM payload: " << mm_payload.value() << std::endl;

    return ss.str();
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
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch(dec.read_integer()) {
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

void CDNS::MalformedMessageData::reset()
{
    server_address_index = boost::none;
    server_port = boost::none;
    mm_transport_flags = boost::none;
    mm_payload = boost::none;
}

std::string CDNS::ResponseProcessingData::string()
{
    std::stringstream ss;

    if (bailiwick_index)
        ss << "Bailiwick index: " << std::to_string(bailiwick_index.value()) << std::endl;

    if (processing_flags)
        ss << "Processing flags: " << std::bitset<8>(processing_flags.value()) << std::endl;

    return ss.str();
}

std::size_t CDNS::ResponseProcessingData::write(CdnsEncoder& enc)
{
    std::size_t fields = !!bailiwick_index + !!processing_flags;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

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
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::ResponseProcessingData::reset()
{
    bailiwick_index = boost::none;
    processing_flags = boost::none;
}

std::string CDNS::QueryResponseExtended::string()
{
    std::stringstream ss;

    if (question_index)
        ss << "Question index: " << std::to_string(question_index.value()) << std::endl;

    if (answer_index)
        ss << "Answer index: " << std::to_string(answer_index.value()) << std::endl;

    if (authority_index)
        ss << "Authority index: " << std::to_string(authority_index.value()) << std::endl;

    if (additional_index)
        ss << "Additional index: " << std::to_string(additional_index.value()) << std::endl;

    return ss.str();
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
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
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

void CDNS::QueryResponseExtended::reset()
{
    question_index = boost::none;
    answer_index = boost::none;
    authority_index = boost::none;
    additional_index = boost::none;
}

std::string CDNS::BlockPreamble::string()
{
    std::stringstream ss;

    ss << "Earliest time:" << std::endl;
    ss << "\tSeconds: " << std::to_string(earliest_time.m_secs) << std::endl;
    ss << "\tTicks: " << std::to_string(earliest_time.m_ticks) << std::endl;

    if (block_parameters_index)
        ss << "Block parameters index: " << std::to_string(block_parameters_index.value()) << std::endl;

    return ss.str();
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

void CDNS::BlockPreamble::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(BlockPreambleMapIndex::earliest_time):
                earliest_time.read(dec);
                break;
            case get_map_index(BlockPreambleMapIndex::block_parameters_index):
                block_parameters_index = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

void CDNS::BlockPreamble::reset()
{
    earliest_time.reset();
    block_parameters_index = boost::none;
}

std::string CDNS::BlockStatistics::string()
{
    std::stringstream ss;

    if (processed_messages)
        ss << "Processed messages: " << std::to_string(processed_messages.value()) << std::endl;

    if (qr_data_items)
        ss << "Q/R data items: " << std::to_string(qr_data_items.value()) << std::endl;

    if (unmatched_queries)
        ss << "Unmatched queries: " << std::to_string(unmatched_queries.value()) << std::endl;

    if (unmatched_responses)
        ss << "Unmatched responses: " << std::to_string(unmatched_responses.value()) << std::endl;

    if (discarded_opcode)
        ss << "Discarded OPCODEs: " << std::to_string(discarded_opcode.value()) << std::endl;

    if (malformed_items)
        ss << "Malformed items: " << std::to_string(malformed_items.value()) << std::endl;

    return ss.str();
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

void CDNS::BlockStatistics::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(BlockStatisticsMapIndex::processed_messages):
                processed_messages = dec.read_unsigned();
                break;
            case get_map_index(BlockStatisticsMapIndex::qr_data_items):
                qr_data_items = dec.read_unsigned();
                break;
            case get_map_index(BlockStatisticsMapIndex::unmatched_queries):
                unmatched_queries = dec.read_unsigned();
                break;
            case get_map_index(BlockStatisticsMapIndex::unmatched_responses):
                unmatched_responses = dec.read_unsigned();
                break;
            case get_map_index(BlockStatisticsMapIndex::discarded_opcode):
                discarded_opcode = dec.read_unsigned();
                break;
            case get_map_index(BlockStatisticsMapIndex::malformed_items):
                malformed_items = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

void CDNS::BlockStatistics::reset()
{
    processed_messages = boost::none;
    qr_data_items = boost::none;
    unmatched_queries = boost::none;
    unmatched_responses = boost::none;
    discarded_opcode = boost::none;
    malformed_items = boost::none;
}

std::string CDNS::QueryResponse::string()
{
    std::stringstream ss;

    if (time_offset)
        ss << time_offset.value().string();

    if (client_address_index)
        ss << "Client address index: " << std::to_string(client_address_index.value()) << std::endl;

    if (client_port)
        ss << "Client port: " << std::to_string(client_port.value()) << std::endl;

    if (transaction_id)
        ss << "Transaction ID: " << std::to_string(transaction_id.value()) << std::endl;

    if (qr_signature_index)
        ss << "Query response signature index: " << std::to_string(qr_signature_index.value()) << std::endl;

    if (client_hoplimit)
        ss << "Client hoplimit: " << std::to_string(client_hoplimit.value()) << std::endl;

    if (response_delay)
        ss << "Response delay: " << std::to_string(response_delay.value()) << std::endl;

    if (query_name_index)
        ss << "QNAME index: " << std::to_string(query_name_index.value()) << std::endl;

    if (query_size)
        ss << "Query size: " << std::to_string(query_size.value()) << std::endl;

    if (response_size)
        ss << "Response size: " << std::to_string(response_size.value()) << std::endl;

    if (response_processing_data)
        ss << response_processing_data.value().string();

    if (query_extended) {
        ss << "Query extended data:" << std::endl;
        ss << query_extended.value().string();
    }

    if (response_extended) {
        ss << "Response extended data:" << std::endl;
        ss << response_extended.value().string();
    }

    if (asn)
        ss << "ASN: " << asn.value() << std::endl;

    if (country_code)
        ss << "Country code: " << country_code.value() << std::endl;

    if (round_trip_time)
        ss << "RTT: " << std::to_string(round_trip_time.value()) << std::endl;

    if (user_id)
        ss << "User ID: " << user_id.value() << std::endl;

    return ss.str();
}

std::size_t CDNS::QueryResponse::write(CdnsEncoder& enc, const Timestamp& earliest, const uint64_t& ticks_per_second)
{
    std::size_t fields = !!time_offset + !!client_address_index + !!client_port + !!transaction_id
                         + !!qr_signature_index + !!client_hoplimit + !!response_delay + !! query_name_index
                         + !!query_size + !!response_size + !!response_processing_data + !!query_extended
                         + !!response_extended + !!asn + !!country_code + !!round_trip_time + !!user_id;

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

    // Write ASN (implementation specific field)
    if (asn) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::asn));
        written += enc.write_textstring(asn.value());
    }

    // Write Country Code (implementation specific field)
    if (country_code) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::country_code));
        written += enc.write_textstring(country_code.value());
    }

    // Write Round Trip Time (implementation specific field)
    if (round_trip_time) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::round_trip_time));
        written += enc.write(round_trip_time.value());
    }

    if (user_id) {
        written += enc.write(get_map_index(CDNS::QueryResponseMapIndex::user_id));
        written += enc.write_textstring(user_id.value());
    }

    return written;
}

void CDNS::QueryResponse::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(QueryResponseMapIndex::time_offset):
                time_offset = Timestamp();
                time_offset->m_secs = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::client_address_index):
                client_address_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::client_port):
                client_port = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::transaction_id):
                transaction_id = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::qr_signature_index):
                qr_signature_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::client_hoplimit):
                client_hoplimit = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::response_delay):
                response_delay = dec.read_integer();
                break;
            case get_map_index(QueryResponseMapIndex::query_name_index):
                query_name_index = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::query_size):
                query_size = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::response_size):
                response_size = dec.read_unsigned();
                break;
            case get_map_index(QueryResponseMapIndex::response_processing_data):
                response_processing_data = ResponseProcessingData();
                response_processing_data->read(dec);
                break;
            case get_map_index(QueryResponseMapIndex::query_extended):
                query_extended = QueryResponseExtended();
                query_extended->read(dec);
                break;
            case get_map_index(QueryResponseMapIndex::response_extended):
                response_extended = QueryResponseExtended();
                response_extended->read(dec);
                break;
            case get_map_index(QueryResponseMapIndex::asn):
                asn = dec.read_textstring();
                break;
            case get_map_index(QueryResponseMapIndex::country_code):
                country_code = dec.read_textstring();
                break;
            case get_map_index(QueryResponseMapIndex::round_trip_time):
                round_trip_time = dec.read_integer();
                break;
            case get_map_index(QueryResponseMapIndex::user_id):
                user_id = dec.read_textstring();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

void CDNS::QueryResponse::reset()
{
    time_offset = boost::none;
    client_address_index = boost::none;
    client_port = boost::none;
    transaction_id = boost::none;
    qr_signature_index = boost::none;
    client_hoplimit = boost::none;
    response_delay = boost::none;
    query_name_index = boost::none;
    query_size = boost::none;
    response_size = boost::none;
    response_processing_data = boost::none;
    query_extended = boost::none;
    response_extended = boost::none;
    asn = boost::none;
    country_code = boost::none;
    round_trip_time = boost::none;
    user_id = boost::none;
}

std::string CDNS::AddressEventCount::string()
{
    std::stringstream ss;

    ss << "Address event type: " << std::to_string(static_cast<std::underlying_type<AddressEventTypeValues>::type>(ae_type)) << std::endl;

    if (ae_code)
        ss << "Address event code: " << std::to_string(ae_code.value()) << std::endl;

    if (ae_transport_flags)
        ss << "Address event transport flags: " << std::bitset<8>(ae_transport_flags.value()) << std::endl;

    ss << "Address index: " << std::to_string(ae_address_index) << std::endl;
    ss << "Address event count: " << std::to_string(ae_count) << std::endl;

    return ss.str();
}

std::size_t CDNS::AddressEventCount::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = 3 + !!ae_code + !!ae_transport_flags;

    // Start Address event count map
    written += enc.write_map_start(fields);

    // Write Ae type
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_type));
    written += enc.write(static_cast<uint8_t>(ae_type));

    // Write Ae code
    if (ae_code) {
        written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_code));
        written += enc.write(ae_code.value());
    }

    // Write Ae address index
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_address_index));
    written += enc.write(ae_address_index);

    // Write Ae tranport flags
    if (ae_transport_flags) {
        written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_transport_flags));
        written += enc.write(static_cast<uint8_t>(ae_transport_flags.value()));
    }

    // Write Ae count
    written += enc.write(get_map_index(CDNS::AddressEventCountMapIndex::ae_count));
    written += enc.write(ae_count);

    return written;
}

void CDNS::AddressEventCount::read(CdnsDecoder& dec)
{
    reset();
    bool is_ae_type = false;
    bool is_ae_address_index = false;
    bool is_ae_count = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(AddressEventCountMapIndex::ae_type):
                ae_type = static_cast<AddressEventTypeValues>(dec.read_unsigned());
                is_ae_type = true;
                break;
            case get_map_index(AddressEventCountMapIndex::ae_code):
                ae_code = dec.read_unsigned();
                break;
            case get_map_index(AddressEventCountMapIndex::ae_address_index):
                ae_address_index = dec.read_unsigned();
                is_ae_address_index = true;
                break;
            case get_map_index(AddressEventCountMapIndex::ae_transport_flags):
                ae_transport_flags = static_cast<QueryResponseTransportFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(AddressEventCountMapIndex::ae_count):
                ae_count = dec.read_unsigned();
                is_ae_count = true;
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_ae_type || !is_ae_address_index || !is_ae_count)
        throw CdnsDecoderException("AddressEventCount from input stream missing one of mandatory items");
}

void CDNS::AddressEventCount::reset()
{
    ae_type = AddressEventTypeValues::tcp_reset;
    ae_code = boost::none;
    ae_address_index = 0;
    ae_transport_flags = boost::none;
    ae_count = 0;
}

std::string CDNS::MalformedMessage::string()
{
    std::stringstream ss;

    if (time_offset)
        ss << time_offset.value().string();

    if (client_address_index)
        ss << "Client address index: " << std::to_string(client_address_index.value()) << std::endl;

    if (client_port)
        ss << "Client port: " << std::to_string(client_port.value()) << std::endl;

    if (message_data_index)
        ss << "Message data index: " << std::to_string(message_data_index.value()) << std::endl;

    return ss.str();
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

void CDNS::MalformedMessage::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(MalformedMessageMapIndex::time_offset):
                time_offset = Timestamp();
                time_offset->m_secs = dec.read_unsigned();
                break;
            case get_map_index(MalformedMessageMapIndex::client_address_index):
                client_address_index = dec.read_unsigned();
                break;
            case get_map_index(MalformedMessageMapIndex::client_port):
                client_port = dec.read_unsigned();
                break;
            case get_map_index(MalformedMessageMapIndex::message_data_index):
                message_data_index = dec.read_unsigned();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

void CDNS::MalformedMessage::reset()
{
    time_offset = boost::none;
    client_address_index = boost::none;
    client_port = boost::none;
    message_data_index = boost::none;
}

std::size_t CDNS::StringItem::write(CdnsEncoder& enc)
{
    return enc.write_bytestring(data);
}

void CDNS::StringItem::read(CdnsDecoder& dec)
{
    reset();
    data = dec.read_bytestring();
}

void CDNS::StringItem::reset()
{
    data.clear();
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

void CDNS::IndexListItem::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_array_start(indef);
    list.reserve(length);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        list.push_back(dec.read_unsigned());
        length--;
    }
}

void CDNS::IndexListItem::reset()
{
    list.clear();
}

std::string CDNS::CdnsBlock::string()
{
    std::stringstream ss;

    ss << m_block_preamble.string();

    if (m_block_statistics)
        ss << m_block_statistics.value().string();

    ss << "IP address BlockTable items: " << std::to_string(m_ip_address.size()) << std::endl;
    ss << "ClassType BlockTable items: " << std::to_string(m_classtype.size()) << std::endl;
    ss << "NAME/RDATA BlockTable items: " << std::to_string(m_name_rdata.size()) << std::endl;
    ss << "Q/R signature BlockTable items: " << std::to_string(m_qr_sig.size()) << std::endl;
    ss << "QuestionList BlockTable items: " << std::to_string(m_qlist.size()) << std::endl;
    ss << "Question BlockTable items: " << std::to_string(m_qrr.size()) << std::endl;
    ss << "RRList BlockTable items: " << std::to_string(m_rrlist.size()) << std::endl;
    ss << "RR BlockTable items: " << std::to_string(m_rr.size()) << std::endl;
    ss << "MalformedMessageData BlockTable items: " << std::to_string(m_malformed_message_data.size()) << std::endl;

    ss << "Query/Response items: " << std::to_string(m_query_responses.size()) << std::endl;
    ss << "Address event count items: " << std::to_string(m_address_event_counts.size()) << std::endl;
    ss << "Malformed message items: " << std::to_string(m_malformed_messages.size()) << std::endl;

    return ss.str();
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
    if (!!m_malformed_message_data.size()) {
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
                         + !!m_rr.size() + !!m_malformed_message_data.size();

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

    for (auto& grr : glist) {
        Question q;
        q.name_index = add_name_rdata(grr.name);
        q.classtype_index = add_classtype(grr.classtype);
        qlist.push_back(add_question(q));
    }

    return add_question_list(qlist);
}

CDNS::index_t CDNS::CdnsBlock::add_generic_rrlist(const std::vector<GenericResourceRecord>& glist) {
    const uint8_t& rr_hints = m_block_parameters.storage_parameters.storage_hints.rr_hints;
    std::vector<index_t> rrlist;

    for (auto& grr : glist) {
        RR rr;
        rr.name_index = add_name_rdata(grr.name);
        rr.classtype_index = add_classtype(grr.classtype);
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
        if ((qr_sig_hints & QueryResponseSignatureHintsMask::query_opt_rdata_index) && gr.query_opt_rdata) {
            qrs.query_opt_rdata_index = add_name_rdata(*gr.query_opt_rdata);
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

    // Fill implementation specific fields

    // ASN
    if (gr.asn) {
        qr.asn = *gr.asn;
        qr_filled = true;
    }

    // Country Code
    if (gr.country_code) {
        qr.country_code = *gr.country_code;
        qr_filled = true;
    }

    // TCP Round Trip Time
    if (gr.round_trip_time) {
        qr.round_trip_time = *gr.round_trip_time;
        qr_filled = true;
    }

    // User ID
    if (gr.user_id) {
        qr.user_id = *gr.user_id;
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
                            + !!qr.response_extended + !!qr.asn + !!qr.country_code + !!qr.round_trip_time
                            + !!qr.user_id;

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

bool CDNS::CdnsBlock::add_address_event_count(const GenericAddressEventCount& gaec,
                                             const boost::optional<BlockStatistics>& stats)
{
    // Check if Address Event Counts are buffered in this Block
    if (!(m_block_parameters.storage_parameters.storage_hints.other_data_hints & OtherDataHintsMask::address_event_counts))
        return false;

    /**
     * Fill Address Event Count
     */

    AddressEventCount aec;

    // Address event type
    aec.ae_type = gaec.ae_type;

    // Address event code
    if (gaec.ae_code)
        aec.ae_code = *gaec.ae_code;

    // Tranport flags
    if (gaec.ae_transport_flags)
        aec.ae_transport_flags = *gaec.ae_transport_flags;

    // IP address
    aec.ae_address_index = add_ip_address(gaec.ip_address);

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

bool CDNS::CdnsBlock::add_address_event_count(const AddressEventCount& aec,
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

void CDNS::CdnsBlockRead::read_blocktables(CdnsDecoder& dec)
{
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(BlockTablesMapIndex::ip_address):
                dec.read_array([this](CdnsDecoder& dec){
                    StringItem tmp;
                    tmp.data = dec.read_bytestring();
                    m_ip_address.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::classtype):
                dec.read_array([this](CdnsDecoder& dec){
                    ClassType tmp;
                    tmp.read(dec);
                    m_classtype.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::name_rdata):
                dec.read_array([this](CdnsDecoder& dec){
                    StringItem tmp;
                    tmp.data = dec.read_bytestring();
                    m_name_rdata.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::qr_sig):
                dec.read_array([this](CdnsDecoder& dec){
                    QueryResponseSignature tmp;
                    tmp.read(dec);
                    m_qr_sig.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::qlist):
                dec.read_array([this](CdnsDecoder& dec){
                    IndexListItem tmp;
                    tmp.read(dec);
                    m_qlist.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::qrr):
                dec.read_array([this](CdnsDecoder& dec){
                    Question tmp;
                    tmp.read(dec);
                    m_qrr.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::rrlist):
                dec.read_array([this](CdnsDecoder& dec){
                    IndexListItem tmp;
                    tmp.read(dec);
                    m_rrlist.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::rr):
                dec.read_array([this](CdnsDecoder& dec){
                    RR tmp;
                    tmp.read(dec);
                    m_rr.add_value(std::move(tmp));
                });
                break;
            case get_map_index(BlockTablesMapIndex::malformed_message_data):
                dec.read_array([this](CdnsDecoder& dec){
                    MalformedMessageData tmp;
                    tmp.read(dec);
                    m_malformed_message_data.add_value(std::move(tmp));
                });
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

std::vector<CDNS::GenericResourceRecord> CDNS::CdnsBlockRead::fill_generic_q_list(std::vector<index_t>& list)
{
    std::vector<GenericResourceRecord> gq_list;
    gq_list.reserve(list.size());

    for (auto qi : list) {
        GenericResourceRecord gq;
        Question q = get_question(qi);
        gq.name = get_name_rdata(q.name_index);
        gq.classtype = get_classtype(q.classtype_index);
        gq_list.push_back(gq);
    }

    return gq_list;
}

std::vector<CDNS::GenericResourceRecord> CDNS::CdnsBlockRead::fill_generic_rr_list(std::vector<index_t>& list)
{
    std::vector<GenericResourceRecord> grr_list;
    grr_list.reserve(list.size());

    for (auto rri : list) {
        GenericResourceRecord grr;
        RR rr = get_rr(rri);
        grr.name = get_name_rdata(rr.name_index);
        grr.classtype = get_classtype(rr.classtype_index);
        grr.ttl = rr.ttl;
        if (rr.rdata_index)
            grr.rdata = get_name_rdata(*rr.rdata_index);

        grr_list.push_back(std::move(grr));
    }

    return grr_list;
}

void CDNS::CdnsBlockRead::read(CdnsDecoder& dec, std::vector<BlockParameters>& block_parameters)
{
    if (block_parameters.empty())
        throw CdnsDecoderException("Given Block parameters array is empty!");

    clear();
    bool is_m_block_preamble = false;
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(BlockMapIndex::block_preamble):
                m_block_preamble.read(dec);
                if (m_block_preamble.block_parameters_index) {
                    if (*m_block_preamble.block_parameters_index < block_parameters.size())
                        m_block_parameters = block_parameters[*m_block_preamble.block_parameters_index];
                    else
                        throw CdnsDecoderException("Block parameters index for C-DNS block is too high");
                }
                is_m_block_preamble = true;
                break;
            case get_map_index(BlockMapIndex::block_statistics):
                m_block_statistics = BlockStatistics();
                m_block_statistics->read(dec);
                break;
            case get_map_index(BlockMapIndex::block_tables):
                read_blocktables(dec);
                break;
            case get_map_index(BlockMapIndex::query_responses):
                dec.read_array([this](CdnsDecoder& dec){
                    QueryResponse tmp;
                    tmp.read(dec);
                    m_query_responses.push_back(std::move(tmp));
                });
                break;
            case get_map_index(BlockMapIndex::address_event_counts):
                dec.read_array([this](CdnsDecoder& dec){
                    AddressEventCount tmp;
                    tmp.read(dec);
                    m_address_event_counts[tmp] = tmp.ae_count;
                });
                break;
            case get_map_index(BlockMapIndex::malformed_messages):
                dec.read_array([this](CdnsDecoder& dec){
                    MalformedMessage tmp;
                    tmp.read(dec);
                    m_malformed_messages.push_back(std::move(tmp));
                });
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_m_block_preamble)
        throw CdnsDecoderException("CdnsBlock from input stream missing one of mandatory items");

    if (!m_block_preamble.block_parameters_index)
        m_block_parameters = block_parameters[0];

    for (auto& qr : m_query_responses) {
        if (qr.time_offset) {
            uint64_t offset = qr.time_offset->m_secs;
            qr.time_offset = m_block_preamble.earliest_time;
            qr.time_offset->add_time_offset(offset, m_block_parameters.storage_parameters.ticks_per_second);
        }
    }

    for (auto& mm : m_malformed_messages) {
        if (mm.time_offset) {
            uint64_t offset = mm.time_offset->m_secs;
            mm.time_offset = m_block_preamble.earliest_time;
            mm.time_offset->add_time_offset(offset, m_block_parameters.storage_parameters.ticks_per_second);
        }
    }

    m_qr_read = 0;
    m_aec_read = m_address_event_counts.begin();
    m_mm_read = 0;
}

CDNS::GenericQueryResponse CDNS::CdnsBlockRead::read_generic_qr(bool& end)
{
    // Check if there are unread query responses in this block
    if (m_qr_read >= m_query_responses.size()) {
        end = true;
        return GenericQueryResponse();
    }

    end = false;
    GenericQueryResponse gqr;
    QueryResponse& qr = m_query_responses[m_qr_read];

    gqr.ts = qr.time_offset;

    if (qr.client_address_index)
        gqr.client_ip = get_ip_address(*qr.client_address_index);

    gqr.client_port = qr.client_port;
    gqr.transaction_id = qr.transaction_id;

    // Get Query Response Signature if present
    if (qr.qr_signature_index) {
        QueryResponseSignature qrs = get_qr_signature(*qr.qr_signature_index);

        if (qrs.server_address_index)
            gqr.server_ip = get_ip_address(*qrs.server_address_index);

        gqr.server_port = qrs.server_port;
        gqr.qr_transport_flags = qrs.qr_transport_flags;
        gqr.qr_type = qrs.qr_type;
        gqr.qr_sig_flags = qrs.qr_sig_flags;
        gqr.query_opcode = qrs.query_opcode;
        gqr.qr_dns_flags = qrs.qr_dns_flags;
        gqr.query_rcode = qrs.query_rcode;

        if (qrs.query_classtype_index)
            gqr.query_classtype = get_classtype(*qrs.query_classtype_index);

        gqr.query_qdcount = qrs.query_qdcount;
        gqr.query_ancount = qrs.query_ancount;
        gqr.query_nscount = qrs.query_nscount;
        gqr.query_arcount = qrs.query_arcount;
        gqr.query_edns_version = qrs.query_edns_version;
        gqr.query_udp_size = qrs.query_udp_size;

        if (qrs.query_opt_rdata_index)
            gqr.query_opt_rdata = get_name_rdata(*qrs.query_opt_rdata_index);

        gqr.response_rcode = qrs.response_rcode;
    }

    gqr.client_hoplimit = qr.client_hoplimit;
    gqr.response_delay = qr.response_delay;

    if (qr.query_name_index)
        gqr.query_name = get_name_rdata(*qr.query_name_index);

    gqr.query_size = qr.query_size;
    gqr.response_size = qr.response_size;

    // Get Response Processing Data if present
    if (qr.response_processing_data) {
        if (qr.response_processing_data->bailiwick_index)
            gqr.bailiwick = get_name_rdata(*qr.response_processing_data->bailiwick_index);

        gqr.processing_flags = qr.response_processing_data->processing_flags;
    }

    // Get Query Extended if present
    if (qr.query_extended) {
        if (qr.query_extended->question_index) {
            auto qlist = get_question_list(*qr.query_extended->question_index);
            gqr.query_questions = fill_generic_q_list(qlist);
        }

        if (qr.query_extended->answer_index) {
            auto rrlist = get_rr_list(*qr.query_extended->answer_index);
            gqr.query_answers = fill_generic_rr_list(rrlist);
        }

        if (qr.query_extended->authority_index) {
            auto rrlist = get_rr_list(*qr.query_extended->authority_index);
            gqr.query_authority = fill_generic_rr_list(rrlist);
        }

        if (qr.query_extended->additional_index) {
            auto rrlist = get_rr_list(*qr.query_extended->additional_index);
            gqr.query_additional = fill_generic_rr_list(rrlist);
        }
    }

    // Get Response Extended if present
    if (qr.response_extended) {
        if (qr.response_extended->question_index) {
            auto qlist = get_question_list(*qr.response_extended->question_index);
            gqr.response_questions = fill_generic_q_list(qlist);
        }

        if (qr.response_extended->answer_index) {
            auto rrlist = get_rr_list(*qr.response_extended->answer_index);
            gqr.response_answers = fill_generic_rr_list(rrlist);
        }

        if (qr.response_extended->authority_index) {
            auto rrlist = get_rr_list(*qr.response_extended->authority_index);
            gqr.response_authority = fill_generic_rr_list(rrlist);
        }

        if (qr.response_extended->additional_index) {
            auto rrlist = get_rr_list(*qr.response_extended->additional_index);
            gqr.response_additional = fill_generic_rr_list(rrlist);
        }
    }

    // Get implementation specific fields
    gqr.asn = qr.asn;
    gqr.country_code = qr.country_code;
    gqr.round_trip_time = qr.round_trip_time;
    gqr.user_id = qr.user_id;

    m_qr_read++;
    return gqr;
}

CDNS::GenericAddressEventCount CDNS::CdnsBlockRead::read_generic_aec(bool& end)
{
    // Check if there are unread address event counts in this block
    if (m_aec_read == m_address_event_counts.end()) {
        end = true;
        return GenericAddressEventCount();
    }

    end = false;
    GenericAddressEventCount gaec;
    AddressEventCount aec = m_aec_read->first;
    aec.ae_count = m_aec_read->second;

    gaec.ae_type = aec.ae_type;
    gaec.ae_code = aec.ae_code;
    gaec.ae_transport_flags = aec.ae_transport_flags;
    gaec.ip_address = get_ip_address(aec.ae_address_index);
    gaec.ae_count = aec.ae_count;

    m_aec_read++;
    return gaec;
}

CDNS::GenericMalformedMessage CDNS::CdnsBlockRead::read_generic_mm(bool& end)
{
    // Check if there are unread malformed messages in this block
    if (m_mm_read >= m_malformed_messages.size()) {
        end = true;
        return GenericMalformedMessage();
    }

    end = false;
    GenericMalformedMessage gmm;
    MalformedMessage& mm = m_malformed_messages[m_mm_read];

    gmm.ts = mm.time_offset;

    if (mm.client_address_index)
        gmm.client_ip = get_ip_address(*mm.client_address_index);

    gmm.client_port = mm.client_port;

    // Get Malformed Message Data if present
    if (mm.message_data_index) {
        MalformedMessageData mmd = get_malformed_message_data(*mm.message_data_index);

        if (mmd.server_address_index)
            gmm.server_ip = get_ip_address(*mmd.server_address_index);

        gmm.server_port = mmd.server_port;
        gmm.mm_transport_flags = mmd.mm_transport_flags;
        gmm.mm_payload = mmd.mm_payload;
    }

    m_mm_read++;
    return gmm;
}
