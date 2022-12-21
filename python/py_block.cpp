/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <tuple>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "block.h"
#include "interface.h"
#include "py_common.h"

namespace py = pybind11;

void init_block(py::module& m)
{
    py::class_<CDNS::ClassType>(m, "ClassType")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::ClassType::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::ClassType::write)
        .def("read", &CDNS::ClassType::read)
        .def("reset", &CDNS::ClassType::reset)
        .def_readwrite("type", &CDNS::ClassType::type)
        .def_readwrite("class_", &CDNS::ClassType::class_);

    py::class_<CDNS::QueryResponseSignature>(m, "QueryResponseSignature")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::QueryResponseSignature::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::QueryResponseSignature::write)
        .def("read", &CDNS::QueryResponseSignature::read)
        .def("reset", &CDNS::QueryResponseSignature::reset)
        .def_readwrite("server_address_index", &CDNS::QueryResponseSignature::server_address_index)
        .def_readwrite("server_port", &CDNS::QueryResponseSignature::server_port)
        .def_readwrite("qr_transport_flags", &CDNS::QueryResponseSignature::qr_transport_flags)
        .def_readwrite("qr_type", &CDNS::QueryResponseSignature::qr_type)
        .def_readwrite("qr_sig_flags", &CDNS::QueryResponseSignature::qr_sig_flags)
        .def_readwrite("query_opcode", &CDNS::QueryResponseSignature::query_opcode)
        .def_readwrite("qr_dns_flags", &CDNS::QueryResponseSignature::qr_dns_flags)
        .def_readwrite("query_rcode", &CDNS::QueryResponseSignature::query_rcode)
        .def_readwrite("query_classtype_index", &CDNS::QueryResponseSignature::query_classtype_index)
        .def_readwrite("query_qdcount", &CDNS::QueryResponseSignature::query_qdcount)
        .def_readwrite("query_ancount", &CDNS::QueryResponseSignature::query_ancount)
        .def_readwrite("query_nscount", &CDNS::QueryResponseSignature::query_nscount)
        .def_readwrite("query_arcount", &CDNS::QueryResponseSignature::query_arcount)
        .def_readwrite("query_edns_version", &CDNS::QueryResponseSignature::query_edns_version)
        .def_readwrite("query_udp_size", &CDNS::QueryResponseSignature::query_udp_size)
        .def_readwrite("query_opt_rdata_index", &CDNS::QueryResponseSignature::query_opt_rdata_index)
        .def_readwrite("response_rcode", &CDNS::QueryResponseSignature::response_rcode);

    py::class_<CDNS::Question>(m, "Question")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::Question::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::Question::write)
        .def("read", &CDNS::Question::read)
        .def("reset", &CDNS::Question::reset)
        .def_readwrite("name_index", &CDNS::Question::name_index)
        .def_readwrite("classtype_index", &CDNS::Question::classtype_index);

    py::class_<CDNS::RR>(m, "RR")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::RR::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::RR::write)
        .def("read", &CDNS::RR::read)
        .def("reset", &CDNS::RR::reset)
        .def_readwrite("name_index", &CDNS::RR::name_index)
        .def_readwrite("classtype_index", &CDNS::RR::classtype_index)
        .def_readwrite("ttl", &CDNS::RR::ttl)
        .def_readwrite("rdata_index", &CDNS::RR::rdata_index);

    py::class_<CDNS::MalformedMessageData>(m, "MalformedMessageData")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::MalformedMessageData::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::MalformedMessageData::write)
        .def("read", &CDNS::MalformedMessageData::read)
        .def("reset", &CDNS::MalformedMessageData::reset)
        .def_readwrite("server_address_index", &CDNS::MalformedMessageData::server_address_index)
        .def_readwrite("server_port", &CDNS::MalformedMessageData::server_port)
        .def_readwrite("mm_transport_flags", &CDNS::MalformedMessageData::mm_transport_flags)
        .def_readwrite("mm_payload", &CDNS::MalformedMessageData::mm_payload);

    py::class_<CDNS::ResponseProcessingData>(m, "ResponseProcessingData")
        .def(py::init())
        .def("write", &CDNS::ResponseProcessingData::write)
        .def("read", &CDNS::ResponseProcessingData::read)
        .def("reset", &CDNS::ResponseProcessingData::reset)
        .def_readwrite("bailiwick_index", &CDNS::ResponseProcessingData::bailiwick_index)
        .def_readwrite("processing_flags", &CDNS::ResponseProcessingData::processing_flags);

    py::class_<CDNS::QueryResponseExtended>(m, "QueryResponseExtended")
        .def(py::init())
        .def("write", &CDNS::QueryResponseExtended::write)
        .def("read", &CDNS::QueryResponseExtended::read)
        .def("reset", &CDNS::QueryResponseExtended::reset)
        .def_readwrite("question_index", &CDNS::QueryResponseExtended::question_index)
        .def_readwrite("answer_index", &CDNS::QueryResponseExtended::answer_index)
        .def_readwrite("authority_index", &CDNS::QueryResponseExtended::authority_index)
        .def_readwrite("additional_index", &CDNS::QueryResponseExtended::additional_index);

    py::class_<CDNS::BlockPreamble>(m, "BlockPreamble")
        .def(py::init())
        .def("write", &CDNS::BlockPreamble::write)
        .def("read", &CDNS::BlockPreamble::read)
        .def("reset", &CDNS::BlockPreamble::reset)
        .def_readwrite("earliest_time", &CDNS::BlockPreamble::earliest_time)
        .def_readwrite("block_parameters_index", &CDNS::BlockPreamble::block_parameters_index);

    py::class_<CDNS::BlockStatistics>(m, "BlockStatistics")
        .def(py::init())
        .def("write", &CDNS::BlockStatistics::write)
        .def("read", &CDNS::BlockStatistics::read)
        .def("reset", &CDNS::BlockStatistics::reset)
        .def_readwrite("processed_messages", &CDNS::BlockStatistics::processed_messages)
        .def_readwrite("qr_data_items", &CDNS::BlockStatistics::qr_data_items)
        .def_readwrite("unmatched_queries", &CDNS::BlockStatistics::unmatched_queries)
        .def_readwrite("unmatched_responses", &CDNS::BlockStatistics::unmatched_responses)
        .def_readwrite("discarded_opcode", &CDNS::BlockStatistics::discarded_opcode)
        .def_readwrite("malformed_items", &CDNS::BlockStatistics::malformed_items);

    py::class_<CDNS::QueryResponse>(m, "QueryResponse")
        .def(py::init())
        .def("write", &CDNS::QueryResponse::write)
        .def("read", &CDNS::QueryResponse::read)
        .def("reset", &CDNS::QueryResponse::reset)
        .def_readwrite("time_offset", &CDNS::QueryResponse::time_offset)
        .def_readwrite("client_address_index", &CDNS::QueryResponse::client_address_index)
        .def_readwrite("client_port", &CDNS::QueryResponse::client_port)
        .def_readwrite("transaction_id", &CDNS::QueryResponse::transaction_id)
        .def_readwrite("qr_signature_index", &CDNS::QueryResponse::qr_signature_index)
        .def_readwrite("client_hoplimit", &CDNS::QueryResponse::client_hoplimit)
        .def_readwrite("response_delay", &CDNS::QueryResponse::response_delay)
        .def_readwrite("query_name_index", &CDNS::QueryResponse::query_name_index)
        .def_readwrite("query_size", &CDNS::QueryResponse::query_size)
        .def_readwrite("response_size", &CDNS::QueryResponse::response_size)
        .def_readwrite("response_processing_data", &CDNS::QueryResponse::response_processing_data)
        .def_readwrite("query_extended", &CDNS::QueryResponse::query_extended)
        .def_readwrite("response_extended", &CDNS::QueryResponse::response_extended)
        .def_readwrite("asn", &CDNS::QueryResponse::asn)
        .def_readwrite("country_code", &CDNS::QueryResponse::country_code)
        .def_readwrite("round_trip_time", &CDNS::QueryResponse::round_trip_time);

    py::class_<CDNS::AddressEventCount>(m, "AddressEventCount")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::AddressEventCount::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::AddressEventCount::write)
        .def("read", &CDNS::AddressEventCount::read)
        .def("reset", &CDNS::AddressEventCount::reset)
        .def_readwrite("ae_type", &CDNS::AddressEventCount::ae_type)
        .def_readwrite("ae_code", &CDNS::AddressEventCount::ae_code)
        .def_readwrite("ae_address_index", &CDNS::AddressEventCount::ae_address_index)
        .def_readwrite("ae_transport_flags", &CDNS::AddressEventCount::ae_transport_flags)
        .def_readwrite("ae_count", &CDNS::AddressEventCount::ae_count);

    py::class_<CDNS::MalformedMessage>(m, "MalformedMessage")
        .def(py::init())
        .def("write", &CDNS::MalformedMessage::write)
        .def("read", &CDNS::MalformedMessage::read)
        .def("reset", &CDNS::MalformedMessage::reset)
        .def_readwrite("time_offset", &CDNS::MalformedMessage::time_offset)
        .def_readwrite("client_address_index", &CDNS::MalformedMessage::client_address_index)
        .def_readwrite("client_port", &CDNS::MalformedMessage::client_port)
        .def_readwrite("message_data_index", &CDNS::MalformedMessage::message_data_index);

    py::class_<CDNS::StringItem>(m, "StringItem")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::StringItem::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::StringItem::write)
        .def("read", &CDNS::StringItem::read)
        .def("reset", &CDNS::StringItem::reset)
        .def_readwrite("data", &CDNS::StringItem::data);

    py::class_<CDNS::IndexListItem>(m, "IndexListItem")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("key", &CDNS::IndexListItem::key, py::return_value_policy::reference_internal)
        .def("write", &CDNS::IndexListItem::write)
        .def("read", &CDNS::IndexListItem::read)
        .def("reset", &CDNS::IndexListItem::reset)
        .def_readwrite("list", &CDNS::IndexListItem::list);

    py::class_<CDNS::CdnsBlock>(m, "CdnsBlock")
        .def(py::init())
        .def(py::init<CDNS::BlockParameters&, CDNS::index_t>())
        .def(py::init<CDNS::CdnsBlock&>())
        .def("write", &CDNS::CdnsBlock::write)
        .def("get_block_parameters_index", &CDNS::CdnsBlock::get_block_parameters_index)
        .def("add_ip_address", &CDNS::CdnsBlock::add_ip_address)
        .def("get_ip_address", &CDNS::CdnsBlock::get_ip_address)
        .def("add_classtype", &CDNS::CdnsBlock::add_classtype)
        .def("get_classtype", &CDNS::CdnsBlock::get_classtype)
        .def("add_name_rdata", &CDNS::CdnsBlock::add_name_rdata)
        .def("get_name_rdata", &CDNS::CdnsBlock::get_name_rdata)
        .def("add_qr_signature", &CDNS::CdnsBlock::add_qr_signature)
        .def("get_qr_signature", &CDNS::CdnsBlock::get_qr_signature)
        .def("add_question_list", &CDNS::CdnsBlock::add_question_list)
        .def("get_question_list", &CDNS::CdnsBlock::get_question_list)
        .def("add_question", &CDNS::CdnsBlock::add_question)
        .def("get_question", &CDNS::CdnsBlock::get_question)
        .def("add_rr_list", &CDNS::CdnsBlock::add_rr_list)
        .def("get_rr_list", &CDNS::CdnsBlock::get_rr_list)
        .def("add_rr", &CDNS::CdnsBlock::add_rr)
        .def("get_rr", &CDNS::CdnsBlock::get_rr)
        .def("add_malformed_message_data", &CDNS::CdnsBlock::add_malformed_message_data)
        .def("get_malformed_message_data", &CDNS::CdnsBlock::get_malformed_message_data)
        .def("add_generic_qlist", &CDNS::CdnsBlock::add_generic_qlist)
        .def("add_generic_rrlist", &CDNS::CdnsBlock::add_generic_rrlist)
        .def("add_question_response_record", py::overload_cast<const CDNS::GenericQueryResponse&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_question_response_record),
            py::arg("qr"), py::arg("stats") = py::none())
        .def("add_question_response_record", py::overload_cast<const CDNS::QueryResponse&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_question_response_record),
            py::arg("qr"), py::arg("stats") = py::none())
        .def("add_address_event_count", py::overload_cast<const CDNS::GenericAddressEventCount&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_address_event_count),
            py::arg("gaec"), py::arg("stats") = py::none())
        .def("add_address_event_count", py::overload_cast<const CDNS::AddressEventCount&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_address_event_count),
            py::arg("aec"), py::arg("stats") = py::none())
        .def("add_malformed_message", py::overload_cast<const CDNS::GenericMalformedMessage&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_malformed_message),
            py::arg("gmm"), py::arg("stats") = py::none())
        .def("add_malformed_message", py::overload_cast<const CDNS::MalformedMessage&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlock::add_malformed_message),
            py::arg("mm"), py::arg("stats") = py::none())
        .def("get_item_count", &CDNS::CdnsBlock::get_item_count)
        .def("get_qr_count", &CDNS::CdnsBlock::get_qr_count)
        .def("get_aec_count", &CDNS::CdnsBlock::get_aec_count)
        .def("get_mm_count", &CDNS::CdnsBlock::get_mm_count)
        .def("full", &CDNS::CdnsBlock::full)
        .def("set_block_parameters", &CDNS::CdnsBlock::set_block_parameters)
        .def("clear", &CDNS::CdnsBlock::clear)
        .def_readwrite("m_block_preamble", &CDNS::CdnsBlock::m_block_preamble)
        .def_readwrite("m_block_statistics", &CDNS::CdnsBlock::m_block_statistics)
        .def_readwrite("m_ip_address", &CDNS::CdnsBlock::m_ip_address)
        .def_readwrite("m_classtype", &CDNS::CdnsBlock::m_classtype)
        .def_readwrite("m_name_rdata", &CDNS::CdnsBlock::m_name_rdata)
        .def_readwrite("m_qr_sig", &CDNS::CdnsBlock::m_qr_sig)
        .def_readwrite("m_qlist", &CDNS::CdnsBlock::m_qlist)
        .def_readwrite("m_qrr", &CDNS::CdnsBlock::m_qrr)
        .def_readwrite("m_rrlist", &CDNS::CdnsBlock::m_rrlist)
        .def_readwrite("m_rr", &CDNS::CdnsBlock::m_rr)
        .def_readwrite("m_malformed_message_data", &CDNS::CdnsBlock::m_malformed_message_data)
        .def_readwrite("m_query_responses", &CDNS::CdnsBlock::m_query_responses)
        .def_readwrite("m_address_event_counts", &CDNS::CdnsBlock::m_address_event_counts)
        .def_readwrite("m_malformed_messages", &CDNS::CdnsBlock::m_malformed_messages);

    py::class_<CDNS::CdnsBlockRead>(m, "CdnsBlockRead")
        .def(py::init())
        .def(py::init<CDNS::CdnsDecoder&, std::vector<CDNS::BlockParameters>&>())
        .def(py::init<CDNS::CdnsBlockRead&>())
        .def("read", &CDNS::CdnsBlockRead::read)
        .def("read_generic_qr", [](CDNS::CdnsBlockRead& self) {
            bool end = false;
            auto ret = self.read_generic_qr(end);
            return std::make_tuple(std::move(ret), end);
        })
        .def("read_generic_aec", [](CDNS::CdnsBlockRead& self) {
            bool end = false;
            auto ret = self.read_generic_aec(end);
            return std::make_tuple(std::move(ret), end);
        })
        .def("read_generic_mm", [](CDNS::CdnsBlockRead& self) {
            bool end = false;
            auto ret = self.read_generic_mm(end);
            return std::make_tuple(std::move(ret), end);
        })
        .def("write", &CDNS::CdnsBlockRead::write)
        .def("get_block_parameters_index", &CDNS::CdnsBlockRead::get_block_parameters_index)
        .def("add_ip_address", &CDNS::CdnsBlockRead::add_ip_address)
        .def("get_ip_address", &CDNS::CdnsBlockRead::get_ip_address)
        .def("add_classtype", &CDNS::CdnsBlockRead::add_classtype)
        .def("get_classtype", &CDNS::CdnsBlockRead::get_classtype)
        .def("add_name_rdata", &CDNS::CdnsBlockRead::add_name_rdata)
        .def("get_name_rdata", &CDNS::CdnsBlockRead::get_name_rdata)
        .def("add_qr_signature", &CDNS::CdnsBlockRead::add_qr_signature)
        .def("get_qr_signature", &CDNS::CdnsBlockRead::get_qr_signature)
        .def("add_question_list", &CDNS::CdnsBlockRead::add_question_list)
        .def("get_question_list", &CDNS::CdnsBlockRead::get_question_list)
        .def("add_question", &CDNS::CdnsBlockRead::add_question)
        .def("get_question", &CDNS::CdnsBlockRead::get_question)
        .def("add_rr_list", &CDNS::CdnsBlockRead::add_rr_list)
        .def("get_rr_list", &CDNS::CdnsBlockRead::get_rr_list)
        .def("add_rr", &CDNS::CdnsBlockRead::add_rr)
        .def("get_rr", &CDNS::CdnsBlockRead::get_rr)
        .def("add_malformed_message_data", &CDNS::CdnsBlockRead::add_malformed_message_data)
        .def("get_malformed_message_data", &CDNS::CdnsBlockRead::get_malformed_message_data)
        .def("add_generic_qlist", &CDNS::CdnsBlockRead::add_generic_qlist)
        .def("add_generic_rrlist", &CDNS::CdnsBlockRead::add_generic_rrlist)
        .def("add_question_response_record", py::overload_cast<const CDNS::GenericQueryResponse&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_question_response_record),
            py::arg("qr"), py::arg("stats") = py::none())
        .def("add_question_response_record", py::overload_cast<const CDNS::QueryResponse&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_question_response_record),
            py::arg("qr"), py::arg("stats") = py::none())
        .def("add_address_event_count", py::overload_cast<const CDNS::GenericAddressEventCount&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_address_event_count),
            py::arg("gaec"), py::arg("stats") = py::none())
        .def("add_address_event_count", py::overload_cast<const CDNS::AddressEventCount&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_address_event_count),
            py::arg("aec"), py::arg("stats") = py::none())
        .def("add_malformed_message", py::overload_cast<const CDNS::GenericMalformedMessage&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_malformed_message),
            py::arg("gmm"), py::arg("stats") = py::none())
        .def("add_malformed_message", py::overload_cast<const CDNS::MalformedMessage&,
            const boost::optional<CDNS::BlockStatistics>&>(&CDNS::CdnsBlockRead::add_malformed_message),
            py::arg("mm"), py::arg("stats") = py::none())
        .def("get_item_count", &CDNS::CdnsBlockRead::get_item_count)
        .def("get_qr_count", &CDNS::CdnsBlockRead::get_qr_count)
        .def("get_aec_count", &CDNS::CdnsBlockRead::get_aec_count)
        .def("get_mm_count", &CDNS::CdnsBlockRead::get_mm_count)
        .def("full", &CDNS::CdnsBlockRead::full)
        .def("set_block_parameters", &CDNS::CdnsBlockRead::set_block_parameters)
        .def("clear", &CDNS::CdnsBlockRead::clear)
        .def_readwrite("m_block_preamble", &CDNS::CdnsBlockRead::m_block_preamble)
        .def_readwrite("m_block_statistics", &CDNS::CdnsBlockRead::m_block_statistics)
        .def_readwrite("m_ip_address", &CDNS::CdnsBlockRead::m_ip_address)
        .def_readwrite("m_classtype", &CDNS::CdnsBlockRead::m_classtype)
        .def_readwrite("m_name_rdata", &CDNS::CdnsBlockRead::m_name_rdata)
        .def_readwrite("m_qr_sig", &CDNS::CdnsBlockRead::m_qr_sig)
        .def_readwrite("m_qlist", &CDNS::CdnsBlockRead::m_qlist)
        .def_readwrite("m_qrr", &CDNS::CdnsBlockRead::m_qrr)
        .def_readwrite("m_rrlist", &CDNS::CdnsBlockRead::m_rrlist)
        .def_readwrite("m_rr", &CDNS::CdnsBlockRead::m_rr)
        .def_readwrite("m_malformed_message_data", &CDNS::CdnsBlockRead::m_malformed_message_data)
        .def_readwrite("m_query_responses", &CDNS::CdnsBlockRead::m_query_responses)
        .def_readwrite("m_address_event_counts", &CDNS::CdnsBlockRead::m_address_event_counts)
        .def_readwrite("m_malformed_messages", &CDNS::CdnsBlockRead::m_malformed_messages);
}
