/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interface.h"
#include "py_common.h"

namespace py = pybind11;

void init_interface(py::module& m)
{
    py::class_<CDNS::GenericResourceRecord>(m, "GenericResourceRecord")
        .def(py::init())
        .def_readwrite("name", &CDNS::GenericResourceRecord::name)
        .def_readwrite("classtype", &CDNS::GenericResourceRecord::classtype)
        .def_readwrite("ttl", &CDNS::GenericResourceRecord::ttl)
        .def_readwrite("rdata", &CDNS::GenericResourceRecord::rdata);

    py::class_<CDNS::GenericQueryResponse>(m, "GenericQueryResponse")
        .def(py::init())
        .def_readwrite("ts", &CDNS::GenericQueryResponse::ts)
        .def_readwrite("client_ip", &CDNS::GenericQueryResponse::client_ip)
        .def_readwrite("client_port", &CDNS::GenericQueryResponse::client_port)
        .def_readwrite("transaction_id", &CDNS::GenericQueryResponse::transaction_id)
        .def_readwrite("server_ip", &CDNS::GenericQueryResponse::server_ip)
        .def_readwrite("server_port", &CDNS::GenericQueryResponse::server_port)
        .def_readwrite("qr_transport_flags", &CDNS::GenericQueryResponse::qr_transport_flags)
        .def_readwrite("qr_type", &CDNS::GenericQueryResponse::qr_type)
        .def_readwrite("qr_sig_flags", &CDNS::GenericQueryResponse::qr_sig_flags)
        .def_readwrite("query_opcode", &CDNS::GenericQueryResponse::query_opcode)
        .def_readwrite("qr_dns_flags", &CDNS::GenericQueryResponse::qr_dns_flags)
        .def_readwrite("query_rcode", &CDNS::GenericQueryResponse::query_rcode)
        .def_readwrite("query_classtype", &CDNS::GenericQueryResponse::query_classtype)
        .def_readwrite("query_qdcount", &CDNS::GenericQueryResponse::query_qdcount)
        .def_readwrite("query_ancount", &CDNS::GenericQueryResponse::query_ancount)
        .def_readwrite("query_nscount", &CDNS::GenericQueryResponse::query_nscount)
        .def_readwrite("query_arcount", &CDNS::GenericQueryResponse::query_arcount)
        .def_readwrite("query_edns_version", &CDNS::GenericQueryResponse::query_edns_version)
        .def_readwrite("query_udp_size", &CDNS::GenericQueryResponse::query_udp_size)
        .def_readwrite("query_opt_rdata", &CDNS::GenericQueryResponse::query_opt_rdata)
        .def_readwrite("response_rcode", &CDNS::GenericQueryResponse::response_rcode)
        .def_readwrite("client_hoplimit", &CDNS::GenericQueryResponse::client_hoplimit)
        .def_readwrite("response_delay", &CDNS::GenericQueryResponse::response_delay)
        .def_readwrite("query_name", &CDNS::GenericQueryResponse::query_name)
        .def_readwrite("query_size", &CDNS::GenericQueryResponse::query_size)
        .def_readwrite("response_size", &CDNS::GenericQueryResponse::response_size)
        .def_readwrite("bailiwick", &CDNS::GenericQueryResponse::bailiwick)
        .def_readwrite("processing_flags", &CDNS::GenericQueryResponse::processing_flags)
        .def_readwrite("query_questions", &CDNS::GenericQueryResponse::query_questions)
        .def_readwrite("query_answers", &CDNS::GenericQueryResponse::query_answers)
        .def_readwrite("query_authority", &CDNS::GenericQueryResponse::query_authority)
        .def_readwrite("query_additional", &CDNS::GenericQueryResponse::query_additional)
        .def_readwrite("response_questions", &CDNS::GenericQueryResponse::response_questions)
        .def_readwrite("response_answers", &CDNS::GenericQueryResponse::response_answers)
        .def_readwrite("response_authority", &CDNS::GenericQueryResponse::response_authority)
        .def_readwrite("response_additional", &CDNS::GenericQueryResponse::response_additional)
        .def_readwrite("asn", &CDNS::GenericQueryResponse::asn)
        .def_readwrite("country_code", &CDNS::GenericQueryResponse::country_code)
        .def_readwrite("round_trip_time", &CDNS::GenericQueryResponse::round_trip_time)
        .def_readwrite("user_id", &CDNS::GenericQueryResponse::user_id);

    py::class_<CDNS::GenericAddressEventCount>(m, "GenericAddressEventCount")
        .def(py::init())
        .def_readwrite("ae_type", &CDNS::GenericAddressEventCount::ae_type)
        .def_readwrite("ae_code", &CDNS::GenericAddressEventCount::ae_code)
        .def_readwrite("ae_transport_flags", &CDNS::GenericAddressEventCount::ae_transport_flags)
        .def_readwrite("ip_address", &CDNS::GenericAddressEventCount::ip_address)
        .def_readwrite("ae_count", &CDNS::GenericAddressEventCount::ae_count);

    py::class_<CDNS::GenericMalformedMessage>(m, "GenericMalformedMessage")
        .def(py::init())
        .def_readwrite("ts", &CDNS::GenericMalformedMessage::ts)
        .def_readwrite("client_ip", &CDNS::GenericMalformedMessage::client_ip)
        .def_readwrite("client_port", &CDNS::GenericMalformedMessage::client_port)
        .def_readwrite("server_ip", &CDNS::GenericMalformedMessage::server_ip)
        .def_readwrite("server_port", &CDNS::GenericMalformedMessage::server_port)
        .def_readwrite("mm_transport_flags", &CDNS::GenericMalformedMessage::mm_transport_flags)
        .def_readwrite("mm_payload", &CDNS::GenericMalformedMessage::mm_payload);
}
