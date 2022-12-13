/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "format_specification.h"
#include "py_common.h"

namespace py = pybind11;

void init_format_specification(py::module& m)
{
    py::enum_<CDNS::CborType>(m, "CborType")
        .value("UNSIGNED", CDNS::CborType::UNSIGNED)
        .value("NEGATIVE", CDNS::CborType::NEGATIVE)
        .value("BYTE_STRING", CDNS::CborType::BYTE_STRING)
        .value("TEXT_STRING", CDNS::CborType::TEXT_STRING)
        .value("ARRAY", CDNS::CborType::ARRAY)
        .value("MAP", CDNS::CborType::MAP)
        .value("TAG", CDNS::CborType::TAG)
        .value("SIMPLE", CDNS::CborType::SIMPLE)
        .value("BREAK", CDNS::CborType::BREAK)
        .export_values();

    py::enum_<CDNS::FileMapIndex>(m, "FileMapIndex")
        .value("file_type_id", CDNS::FileMapIndex::file_type_id)
        .value("file_preamble", CDNS::FileMapIndex::file_preamble)
        .value("file_block", CDNS::FileMapIndex::file_block)
        .value("file_size", CDNS::FileMapIndex::file_size)
        .export_values();

    py::enum_<CDNS::FilePreambleMapIndex>(m, "FilePreambleMapIndex")
        .value("major_format_version", CDNS::FilePreambleMapIndex::major_format_version)
        .value("minor_format_version", CDNS::FilePreambleMapIndex::minor_format_version)
        .value("private_version", CDNS::FilePreambleMapIndex::private_version)
        .value("block_parameters", CDNS::FilePreambleMapIndex::block_parameters)
        .value("file_preamble_size", CDNS::FilePreambleMapIndex::file_preamble_size)
        .export_values();

    py::enum_<CDNS::BlockParametersMapIndex>(m, "BlockParametersMapIndex")
        .value("storage_parameters", CDNS::BlockParametersMapIndex::storage_parameters)
        .value("collection_parameters", CDNS::BlockParametersMapIndex::collection_parameters)
        .value("block_parameters_size", CDNS::BlockParametersMapIndex::block_parameters_size)
        .export_values();

    py::enum_<CDNS::StorageParametersMapIndex>(m, "StorageParametersMapIndex")
        .value("ticks_per_second", CDNS::StorageParametersMapIndex::ticks_per_second)
        .value("max_block_items", CDNS::StorageParametersMapIndex::max_block_items)
        .value("storage_hints", CDNS::StorageParametersMapIndex::storage_hints)
        .value("opcodes", CDNS::StorageParametersMapIndex::opcodes)
        .value("rr_types", CDNS::StorageParametersMapIndex::rr_types)
        .value("storage_flags", CDNS::StorageParametersMapIndex::storage_flags)
        .value("client_address_prefix_ipv4", CDNS::StorageParametersMapIndex::client_address_prefix_ipv4)
        .value("client_address_prefix_ipv6", CDNS::StorageParametersMapIndex::client_address_prefix_ipv6)
        .value("server_address_prefix_ipv4", CDNS::StorageParametersMapIndex::server_address_prefix_ipv4)
        .value("server_address_prefix_ipv6", CDNS::StorageParametersMapIndex::server_address_prefix_ipv6)
        .value("sampling_method", CDNS::StorageParametersMapIndex::sampling_method)
        .value("anonymization_method", CDNS::StorageParametersMapIndex::anonymization_method)
        .value("storage_parameters_size", CDNS::StorageParametersMapIndex::storage_parameters_size)
        .export_values();

    py::enum_<CDNS::CollectionParametersMapIndex>(m, "CollectionParametersMapIndex")
        .value("query_timeout", CDNS::CollectionParametersMapIndex::query_timeout)
        .value("skew_timeout", CDNS::CollectionParametersMapIndex::skew_timeout)
        .value("snaplen", CDNS::CollectionParametersMapIndex::snaplen)
        .value("promisc", CDNS::CollectionParametersMapIndex::promisc)
        .value("interfaces", CDNS::CollectionParametersMapIndex::interfaces)
        .value("server_address", CDNS::CollectionParametersMapIndex::server_address)
        .value("vlan_ids", CDNS::CollectionParametersMapIndex::vlan_ids)
        .value("filter", CDNS::CollectionParametersMapIndex::filter)
        .value("generator_id", CDNS::CollectionParametersMapIndex::generator_id)
        .value("host_id", CDNS::CollectionParametersMapIndex::host_id)
        .value("collection_parameters_size", CDNS::CollectionParametersMapIndex::collection_parameters_size)
        .export_values();

    py::enum_<CDNS::StorageHintsMapIndex>(m, "StorageHintsMapIndex")
        .value("query_response_hints", CDNS::StorageHintsMapIndex::query_response_hints)
        .value("query_response_signature_hints", CDNS::StorageHintsMapIndex::query_response_signature_hints)
        .value("rr_hints", CDNS::StorageHintsMapIndex::rr_hints)
        .value("other_data_hints", CDNS::StorageHintsMapIndex::other_data_hints)
        .value("storage_hints_size", CDNS::StorageHintsMapIndex::storage_hints_size)
        .export_values();

    py::enum_<CDNS::BlockMapIndex>(m, "BlockMapIndex")
        .value("block_preamble", CDNS::BlockMapIndex::block_preamble)
        .value("block_statistics", CDNS::BlockMapIndex::block_statistics)
        .value("block_tables", CDNS::BlockMapIndex::block_tables)
        .value("query_responses", CDNS::BlockMapIndex::query_responses)
        .value("address_event_counts", CDNS::BlockMapIndex::address_event_counts)
        .value("malformed_messages", CDNS::BlockMapIndex::malformed_messages)
        .value("block_size", CDNS::BlockMapIndex::block_size)
        .export_values();

    py::enum_<CDNS::BlockPreambleMapIndex>(m, "BlockPreambleMapIndex")
        .value("earliest_time", CDNS::BlockPreambleMapIndex::earliest_time)
        .value("block_parameters_index", CDNS::BlockPreambleMapIndex::block_parameters_index)
        .value("block_preamble_size", CDNS::BlockPreambleMapIndex::block_preamble_size)
        .export_values();

    py::enum_<CDNS::BlockStatisticsMapIndex>(m, "BlockStatisticsMapIndex")
        .value("processed_messages", CDNS::BlockStatisticsMapIndex::processed_messages)
        .value("qr_data_items", CDNS::BlockStatisticsMapIndex::qr_data_items)
        .value("unmatched_queries", CDNS::BlockStatisticsMapIndex::unmatched_queries)
        .value("unmatched_responses", CDNS::BlockStatisticsMapIndex::unmatched_responses)
        .value("discarded_opcode", CDNS::BlockStatisticsMapIndex::discarded_opcode)
        .value("malformed_items", CDNS::BlockStatisticsMapIndex::malformed_items)
        .value("block_statistics_size", CDNS::BlockStatisticsMapIndex::block_statistics_size)
        .export_values();

    py::enum_<CDNS::BlockTablesMapIndex>(m, "BlockTablesMapIndex")
        .value("ip_address", CDNS::BlockTablesMapIndex::ip_address)
        .value("classtype", CDNS::BlockTablesMapIndex::classtype)
        .value("name_rdata", CDNS::BlockTablesMapIndex::name_rdata)
        .value("qr_sig", CDNS::BlockTablesMapIndex::qr_sig)
        .value("qlist", CDNS::BlockTablesMapIndex::qlist)
        .value("qrr", CDNS::BlockTablesMapIndex::qrr)
        .value("rrlist", CDNS::BlockTablesMapIndex::rrlist)
        .value("rr", CDNS::BlockTablesMapIndex::rr)
        .value("malformed_message_data", CDNS::BlockTablesMapIndex::malformed_message_data)
        .value("block_tables_size", CDNS::BlockTablesMapIndex::block_tables_size)
        .export_values();

    py::enum_<CDNS::ClassTypeMapIndex>(m, "ClassTypeMapIndex")
        .value("type", CDNS::ClassTypeMapIndex::type)
        .value("class_", CDNS::ClassTypeMapIndex::class_)
        .value("class_type_size", CDNS::ClassTypeMapIndex::class_type_size)
        .export_values();

    py::enum_<CDNS::QueryResponseSignatureMapIndex>(m, "QueryResponseSignatureMapIndex")
        .value("server_address_index", CDNS::QueryResponseSignatureMapIndex::server_address_index)
        .value("server_port", CDNS::QueryResponseSignatureMapIndex::server_port)
        .value("qr_transport_flags", CDNS::QueryResponseSignatureMapIndex::qr_transport_flags)
        .value("qr_type", CDNS::QueryResponseSignatureMapIndex::qr_type)
        .value("qr_sig_flags", CDNS::QueryResponseSignatureMapIndex::qr_sig_flags)
        .value("query_opcode", CDNS::QueryResponseSignatureMapIndex::query_opcode)
        .value("qr_dns_flags", CDNS::QueryResponseSignatureMapIndex::qr_dns_flags)
        .value("query_rcode", CDNS::QueryResponseSignatureMapIndex::query_rcode)
        .value("query_classtype_index", CDNS::QueryResponseSignatureMapIndex::query_classtype_index)
        .value("query_qdcount", CDNS::QueryResponseSignatureMapIndex::query_qdcount)
        .value("query_ancount", CDNS::QueryResponseSignatureMapIndex::query_ancount)
        .value("query_nscount", CDNS::QueryResponseSignatureMapIndex::query_nscount)
        .value("query_arcount", CDNS::QueryResponseSignatureMapIndex::query_arcount)
        .value("query_edns_version", CDNS::QueryResponseSignatureMapIndex::query_edns_version)
        .value("query_udp_size", CDNS::QueryResponseSignatureMapIndex::query_udp_size)
        .value("query_opt_rdata_index", CDNS::QueryResponseSignatureMapIndex::query_opt_rdata_index)
        .value("response_rcode", CDNS::QueryResponseSignatureMapIndex::response_rcode)
        .value("query_response_signature_size", CDNS::QueryResponseSignatureMapIndex::query_response_signature_size)
        .export_values();

    py::enum_<CDNS::QuestionMapIndex>(m, "QuestionMapIndex")
        .value("name_index", CDNS::QuestionMapIndex::name_index)
        .value("classtype_index", CDNS::QuestionMapIndex::classtype_index)
        .value("question_size", CDNS::QuestionMapIndex::question_size)
        .export_values();

    py::enum_<CDNS::RrMapIndex>(m, "RrMapIndex")
        .value("name_index", CDNS::RrMapIndex::name_index)
        .value("classtype_index", CDNS::RrMapIndex::classtype_index)
        .value("ttl", CDNS::RrMapIndex::ttl)
        .value("rdata_index", CDNS::RrMapIndex::rdata_index)
        .value("rr_size", CDNS::RrMapIndex::rr_size)
        .export_values();

    py::enum_<CDNS::MalformedMessageDataMapIndex>(m, "MalformedMessageDataMapIndex")
        .value("server_address_index", CDNS::MalformedMessageDataMapIndex::server_address_index)
        .value("server_port", CDNS::MalformedMessageDataMapIndex::server_port)
        .value("mm_transport_flags", CDNS::MalformedMessageDataMapIndex::mm_transport_flags)
        .value("mm_payload", CDNS::MalformedMessageDataMapIndex::mm_payload)
        .value("malformed_message_data_size", CDNS::MalformedMessageDataMapIndex::malformed_message_data_size)
        .export_values();

    py::enum_<CDNS::QueryResponseMapIndex>(m, "QueryResponseMapIndex")
        .value("time_offset", CDNS::QueryResponseMapIndex::time_offset)
        .value("client_address_index", CDNS::QueryResponseMapIndex::client_address_index)
        .value("client_port", CDNS::QueryResponseMapIndex::client_port)
        .value("transaction_id", CDNS::QueryResponseMapIndex::transaction_id)
        .value("qr_signature_index", CDNS::QueryResponseMapIndex::qr_signature_index)
        .value("client_hoplimit", CDNS::QueryResponseMapIndex::client_hoplimit)
        .value("response_delay", CDNS::QueryResponseMapIndex::response_delay)
        .value("query_name_index", CDNS::QueryResponseMapIndex::query_name_index)
        .value("query_size", CDNS::QueryResponseMapIndex::query_size)
        .value("response_size", CDNS::QueryResponseMapIndex::response_size)
        .value("response_processing_data", CDNS::QueryResponseMapIndex::response_processing_data)
        .value("query_extended", CDNS::QueryResponseMapIndex::query_extended)
        .value("response_extended", CDNS::QueryResponseMapIndex::response_extended)
        .value("asn", CDNS::QueryResponseMapIndex::asn)
        .value("country_code", CDNS::QueryResponseMapIndex::country_code)
        .value("round_trip_time", CDNS::QueryResponseMapIndex::round_trip_time)
        .value("query_response_size", CDNS::QueryResponseMapIndex::query_response_size)
        .export_values();

    py::enum_<CDNS::ResponseProcessingDataMapIndex>(m, "ResponseProcessingDataMapIndex")
        .value("bailiwick_index", CDNS::ResponseProcessingDataMapIndex::bailiwick_index)
        .value("processing_flags", CDNS::ResponseProcessingDataMapIndex::processing_flags)
        .value("response_processing_data_size", CDNS::ResponseProcessingDataMapIndex::response_processing_data_size)
        .export_values();

    py::enum_<CDNS::QueryResponseExtendedMapIndex>(m, "QueryResponseExtendedMapIndex")
        .value("question_index", CDNS::QueryResponseExtendedMapIndex::question_index)
        .value("answer_index", CDNS::QueryResponseExtendedMapIndex::answer_index)
        .value("authority_index", CDNS::QueryResponseExtendedMapIndex::authority_index)
        .value("additional_index", CDNS::QueryResponseExtendedMapIndex::additional_index)
        .value("query_response_extended_size", CDNS::QueryResponseExtendedMapIndex::query_response_extended_size)
        .export_values();

    py::enum_<CDNS::AddressEventCountMapIndex>(m, "AddressEventCountMapIndex")
        .value("ae_type", CDNS::AddressEventCountMapIndex::ae_type)
        .value("ae_code", CDNS::AddressEventCountMapIndex::ae_code)
        .value("ae_address_index", CDNS::AddressEventCountMapIndex::ae_address_index)
        .value("ae_transport_flags", CDNS::AddressEventCountMapIndex::ae_transport_flags)
        .value("ae_count", CDNS::AddressEventCountMapIndex::ae_count)
        .value("address_event_count_size", CDNS::AddressEventCountMapIndex::address_event_count_size)
        .export_values();

    py::enum_<CDNS::MalformedMessageMapIndex>(m, "MalformedMessageMapIndex")
        .value("time_offset", CDNS::MalformedMessageMapIndex::time_offset)
        .value("client_address_index", CDNS::MalformedMessageMapIndex::client_address_index)
        .value("client_port", CDNS::MalformedMessageMapIndex::client_port)
        .value("message_data_index", CDNS::MalformedMessageMapIndex::message_data_index)
        .value("malformed_message_size", CDNS::MalformedMessageMapIndex::malformed_message_size)
        .export_values();

    py::enum_<CDNS::QueryResponseHintsMask>(m, "QueryResponseHintsMask")
        .value("time_offset", CDNS::QueryResponseHintsMask::time_offset)
        .value("client_address_index", CDNS::QueryResponseHintsMask::client_address_index)
        .value("client_port", CDNS::QueryResponseHintsMask::client_port)
        .value("transaction_id", CDNS::QueryResponseHintsMask::transaction_id)
        .value("qr_signature_index", CDNS::QueryResponseHintsMask::qr_signature_index)
        .value("client_hoplimit", CDNS::QueryResponseHintsMask::client_hoplimit)
        .value("response_delay", CDNS::QueryResponseHintsMask::response_delay)
        .value("query_name_index", CDNS::QueryResponseHintsMask::query_name_index)
        .value("query_size", CDNS::QueryResponseHintsMask::query_size)
        .value("response_size", CDNS::QueryResponseHintsMask::response_size)
        .value("response_processing_data", CDNS::QueryResponseHintsMask::response_processing_data)
        .value("query_question_sections", CDNS::QueryResponseHintsMask::query_question_sections)
        .value("query_answer_sections", CDNS::QueryResponseHintsMask::query_answer_sections)
        .value("query_authority_sections", CDNS::QueryResponseHintsMask::query_authority_sections)
        .value("query_additional_sections", CDNS::QueryResponseHintsMask::query_additional_sections)
        .value("response_answer_sections", CDNS::QueryResponseHintsMask::response_answer_sections)
        .value("response_authority_sections", CDNS::QueryResponseHintsMask::response_authority_sections)
        .value("response_additional_sections", CDNS::QueryResponseHintsMask::response_additional_sections)
        .export_values();

    py::enum_<CDNS::QueryResponseSignatureHintsMask>(m, "QueryResponseSignatureHintsMask")
        .value("server_address_index", CDNS::QueryResponseSignatureHintsMask::server_address_index)
        .value("server_port", CDNS::QueryResponseSignatureHintsMask::server_port)
        .value("qr_transport_flags", CDNS::QueryResponseSignatureHintsMask::qr_transport_flags)
        .value("qr_type", CDNS::QueryResponseSignatureHintsMask::qr_type)
        .value("qr_sig_flags", CDNS::QueryResponseSignatureHintsMask::qr_sig_flags)
        .value("query_opcode", CDNS::QueryResponseSignatureHintsMask::query_opcode)
        .value("qr_dns_flags", CDNS::QueryResponseSignatureHintsMask::qr_dns_flags)
        .value("query_rcode", CDNS::QueryResponseSignatureHintsMask::query_rcode)
        .value("query_classtype_index", CDNS::QueryResponseSignatureHintsMask::query_classtype_index)
        .value("query_qdcount", CDNS::QueryResponseSignatureHintsMask::query_qdcount)
        .value("query_ancount", CDNS::QueryResponseSignatureHintsMask::query_ancount)
        .value("query_nscount", CDNS::QueryResponseSignatureHintsMask::query_nscount)
        .value("query_arcount", CDNS::QueryResponseSignatureHintsMask::query_arcount)
        .value("query_edns_version", CDNS::QueryResponseSignatureHintsMask::query_edns_version)
        .value("query_udp_size", CDNS::QueryResponseSignatureHintsMask::query_udp_size)
        .value("query_opt_rdata_index", CDNS::QueryResponseSignatureHintsMask::query_opt_rdata_index)
        .value("response_rcode", CDNS::QueryResponseSignatureHintsMask::response_rcode)
        .export_values();

    py::enum_<CDNS::RrHintsMask>(m, "RrHintsMask")
        .value("ttl", CDNS::RrHintsMask::ttl)
        .value("rdata_index", CDNS::RrHintsMask::rdata_index)
        .export_values();

    py::enum_<CDNS::OtherDataHintsMask>(m, "OtherDataHintsMask")
        .value("malformed_messages", CDNS::OtherDataHintsMask::malformed_messages)
        .value("address_event_counts", CDNS::OtherDataHintsMask::address_event_counts)
        .export_values();

    py::enum_<CDNS::StorageFlagsMask>(m, "StorageFlagsMask")
        .value("anonymized_data", CDNS::StorageFlagsMask::anonymized_data)
        .value("sampled_data", CDNS::StorageFlagsMask::sampled_data)
        .value("normalized_data", CDNS::StorageFlagsMask::normalized_data)
        .export_values();

    py::enum_<CDNS::QueryResponseTransportFlagsMask>(m, "QueryResponseTransportFlagsMask")
        .value("ip_address", CDNS::QueryResponseTransportFlagsMask::ip_address)
        .value("transport_flags", CDNS::QueryResponseTransportFlagsMask::transport_flags)
        .value("udp", CDNS::QueryResponseTransportFlagsMask::udp)
        .value("tcp", CDNS::QueryResponseTransportFlagsMask::tcp)
        .value("tls", CDNS::QueryResponseTransportFlagsMask::tls)
        .value("dtls", CDNS::QueryResponseTransportFlagsMask::dtls)
        .value("https", CDNS::QueryResponseTransportFlagsMask::https)
        .value("non_standard", CDNS::QueryResponseTransportFlagsMask::non_standard)
        .value("query_trailingdata", CDNS::QueryResponseTransportFlagsMask::query_trailingdata)
        .export_values();

    py::enum_<CDNS::QueryResponseFlagsMask>(m, "QueryResponseFlagsMask")
        .value("has_query", CDNS::QueryResponseFlagsMask::has_query)
        .value("has_response", CDNS::QueryResponseFlagsMask::has_response)
        .value("query_has_opt", CDNS::QueryResponseFlagsMask::query_has_opt)
        .value("response_has_opt", CDNS::QueryResponseFlagsMask::response_has_opt)
        .value("query_has_no_question", CDNS::QueryResponseFlagsMask::query_has_no_question)
        .value("response_has_no_question", CDNS::QueryResponseFlagsMask::response_has_no_question)
        .export_values();

    py::enum_<CDNS::DNSFlagsMask>(m, "DNSFlagsMask")
        .value("query_cd", CDNS::DNSFlagsMask::query_cd)
        .value("query_ad", CDNS::DNSFlagsMask::query_ad)
        .value("query_z", CDNS::DNSFlagsMask::query_z)
        .value("query_ra", CDNS::DNSFlagsMask::query_ra)
        .value("query_rd", CDNS::DNSFlagsMask::query_rd)
        .value("query_tc", CDNS::DNSFlagsMask::query_tc)
        .value("query_aa", CDNS::DNSFlagsMask::query_aa)
        .value("query_do", CDNS::DNSFlagsMask::query_do)
        .value("response_cd", CDNS::DNSFlagsMask::response_cd)
        .value("response_ad", CDNS::DNSFlagsMask::response_ad)
        .value("response_z", CDNS::DNSFlagsMask::response_z)
        .value("response_ra", CDNS::DNSFlagsMask::response_ra)
        .value("response_rd", CDNS::DNSFlagsMask::response_rd)
        .value("response_tc", CDNS::DNSFlagsMask::response_tc)
        .value("response_aa", CDNS::DNSFlagsMask::response_aa)
        .export_values();

    py::enum_<CDNS::ResponseProcessingFlagsMask>(m, "ResponseProcessingFlagsMask")
        .value("from_cache", CDNS::ResponseProcessingFlagsMask::from_cache)
        .export_values();

    py::enum_<CDNS::QueryResponseTypeValues>(m, "QueryResponseTypeValues")
        .value("stub", CDNS::QueryResponseTypeValues::stub)
        .value("client", CDNS::QueryResponseTypeValues::client)
        .value("resolver", CDNS::QueryResponseTypeValues::resolver)
        .value("auth", CDNS::QueryResponseTypeValues::auth)
        .value("forwarder", CDNS::QueryResponseTypeValues::forwarder)
        .value("tool", CDNS::QueryResponseTypeValues::tool)
        .export_values();

    py::enum_<CDNS::AddressEventTypeValues>(m, "AddressEventTypeValues")
        .value("tcp_reset", CDNS::AddressEventTypeValues::tcp_reset)
        .value("icmp_time_exceeded", CDNS::AddressEventTypeValues::icmp_time_exceeded)
        .value("icmp_dest_unreachable", CDNS::AddressEventTypeValues::icmp_dest_unreachable)
        .value("icmpv6_time_exceeded", CDNS::AddressEventTypeValues::icmpv6_time_exceeded)
        .value("icmpv6_dest_unreachable", CDNS::AddressEventTypeValues::icmpv6_dest_unreachable)
        .value("icmpv6_packet_too_big", CDNS::AddressEventTypeValues::icmpv6_packet_too_big)
        .export_values();
}
