#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cdns_encoder.h"
#include "cdns_decoder.h"
#include "file_preamble.h"
#include "py_common.h"

namespace py = pybind11;

void init_file_preamble(py::module& m)
{
    m.attr("VERSION_MAJOR") = &CDNS::VERSION_MAJOR;
    m.attr("VERSION_MINOR") = &CDNS::VERSION_MINOR;
    m.attr("VERSION_PRIVATE") = &CDNS::VERSION_PRIVATE;
    m.attr("DEFAULT_TICKS_PER_SECOND") = &CDNS::DEFAULT_TICKS_PER_SECOND;
    m.attr("DEFAULT_MAX_BLOCK_ITEMS") = &CDNS::DEFAULT_MAX_BLOCK_ITEMS;
    m.attr("DEFAULT_QR_HINTS") = &CDNS::DEFAULT_QR_HINTS;
    m.attr("DEFAULT_QR_SIG_HINTS") = &CDNS::DEFAULT_QR_SIG_HINTS;
    m.attr("DEFAULT_RR_HINTS") = &CDNS::DEFAULT_RR_HINTS;
    m.attr("DEFAULT_OTHER_DATA_HINTS") = &CDNS::DEFAULT_OTHER_DATA_HINTS;

    py::class_<CDNS::StorageHints>(m, "StorageHints")
        .def(py::init())
        .def("write", &CDNS::StorageHints::write)
        .def("read", &CDNS::StorageHints::read)
        .def("reset", &CDNS::StorageHints::reset)
        .def_readwrite("query_response_hints", &CDNS::StorageHints::query_response_hints)
        .def_readwrite("query_response_signature_hints", &CDNS::StorageHints::query_response_signature_hints)
        .def_readwrite("rr_hints", &CDNS::StorageHints::rr_hints)
        .def_readwrite("other_data_hints", &CDNS::StorageHints::other_data_hints);

    py::class_<CDNS::StorageParameters>(m, "StorageParameters")
        .def(py::init())
        .def("write", &CDNS::StorageParameters::write)
        .def("read", &CDNS::StorageParameters::read)
        .def("reset", &CDNS::StorageParameters::reset)
        .def_readwrite("ticks_per_second", &CDNS::StorageParameters::ticks_per_second)
        .def_readwrite("max_block_items", &CDNS::StorageParameters::max_block_items)
        .def_readwrite("storage_hints", &CDNS::StorageParameters::storage_hints)
        .def_readwrite("opcodes", &CDNS::StorageParameters::opcodes)
        .def_readwrite("rr_types", &CDNS::StorageParameters::rr_types)
        .def_readwrite("storage_flags", &CDNS::StorageParameters::storage_flags)
        .def_readwrite("client_address_prefix_ipv4", &CDNS::StorageParameters::client_address_prefix_ipv4)
        .def_readwrite("client_address_prefix_ipv6", &CDNS::StorageParameters::client_address_prefix_ipv6)
        .def_readwrite("server_address_prefix_ipv4", &CDNS::StorageParameters::server_address_prefix_ipv4)
        .def_readwrite("server_address_prefix_ipv6", &CDNS::StorageParameters::server_address_prefix_ipv6)
        .def_readwrite("sampling_method", &CDNS::StorageParameters::sampling_method)
        .def_readwrite("anonymization_method", &CDNS::StorageParameters::anonymization_method);

    py::class_<CDNS::CollectionParameters>(m, "CollectionParameters")
        .def(py::init())
        .def("write", &CDNS::CollectionParameters::write)
        .def("read", &CDNS::CollectionParameters::read)
        .def("reset", &CDNS::CollectionParameters::reset)
        .def_readwrite("query_timeout", &CDNS::CollectionParameters::query_timeout)
        .def_readwrite("skew_timeout", &CDNS::CollectionParameters::skew_timeout)
        .def_readwrite("snaplen", &CDNS::CollectionParameters::snaplen)
        .def_readwrite("promisc", &CDNS::CollectionParameters::promisc)
        .def_readwrite("interfaces", &CDNS::CollectionParameters::interfaces)
        .def_readwrite("server_address", &CDNS::CollectionParameters::server_address)
        .def_readwrite("vlan_ids", &CDNS::CollectionParameters::vlan_ids)
        .def_readwrite("filter", &CDNS::CollectionParameters::filter)
        .def_readwrite("generator_id", &CDNS::CollectionParameters::generator_id)
        .def_readwrite("host_id", &CDNS::CollectionParameters::host_id);

    py::class_<CDNS::BlockParameters>(m, "BlockParameters")
        .def(py::init())
        .def("write", &CDNS::BlockParameters::write)
        .def("read", &CDNS::BlockParameters::read)
        .def("reset", &CDNS::BlockParameters::reset)
        .def_readwrite("storage_parameters", &CDNS::BlockParameters::storage_parameters)
        .def_readwrite("collection_parameters", &CDNS::BlockParameters::collection_parameters);

    py::class_<CDNS::FilePreamble>(m, "FilePreamble")
        .def(py::init())
        .def(py::init<std::vector<CDNS::BlockParameters>&, boost::optional<uint8_t>>())
        .def("add_block_parameters", &CDNS::FilePreamble::add_block_parameters)
        .def("block_parameters_size", &CDNS::FilePreamble::block_parameters_size)
        .def("get_block_parameters", &CDNS::FilePreamble::get_block_parameters,
            py::return_value_policy::reference_internal)
        .def("write", &CDNS::FilePreamble::write)
        .def("read", &CDNS::FilePreamble::read)
        .def("reset", &CDNS::FilePreamble::reset)
        .def_readwrite("m_major_format_version", &CDNS::FilePreamble::m_major_format_version)
        .def_readwrite("m_minor_format_version", &CDNS::FilePreamble::m_minor_format_version)
        .def_readwrite("m_private_version", &CDNS::FilePreamble::m_private_version)
        .def_readwrite("m_block_parameters", &CDNS::FilePreamble::m_block_parameters);
}
