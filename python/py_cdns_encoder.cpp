#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cdns_encoder.h"
#include "py_common.h"

namespace py = pybind11;

void init_cdns_encoder(py::module& m)
{
    py::register_exception<CDNS::CdnsEncoderException>(m, "CdnsEncoderException");

    py::class_<CDNS::CdnsEncoder>(m, "CdnsEncoder")
        .def(py::init<const std::string&, CDNS::CborOutputCompression>())
        .def(py::init<const int&, CDNS::CborOutputCompression>())
        .def("write_array_start", &CDNS::CdnsEncoder::write_array_start)
        .def("write_indef_array_start", &CDNS::CdnsEncoder::write_indef_array_start)
        .def("write_map_start", &CDNS::CdnsEncoder::write_map_start)
        .def("write_indef_map_start", &CDNS::CdnsEncoder::write_indef_map_start)
        .def("write_bytestring", py::overload_cast<const unsigned char*, std::size_t>(
            &CDNS::CdnsEncoder::write_bytestring))
        .def("write_bytestring", py::overload_cast<const std::string&>(
            &CDNS::CdnsEncoder::write_bytestring))
        .def("write_textstring", py::overload_cast<const unsigned char*, std::size_t>(
            &CDNS::CdnsEncoder::write_textstring))
        .def("write_textstring", py::overload_cast<const std::string&>(
            &CDNS::CdnsEncoder::write_textstring))
        .def("write_break", &CDNS::CdnsEncoder::write_break)
        .def("write_bool", py::overload_cast<bool>(&CDNS::CdnsEncoder::write))
        .def("write_uint8", py::overload_cast<uint8_t>(&CDNS::CdnsEncoder::write))
        .def("write_uint16", py::overload_cast<uint16_t>(&CDNS::CdnsEncoder::write))
        .def("write_uint32", py::overload_cast<uint32_t>(&CDNS::CdnsEncoder::write))
        .def("write_uint64", py::overload_cast<uint64_t>(&CDNS::CdnsEncoder::write))
        .def("write_int8", py::overload_cast<int8_t>(&CDNS::CdnsEncoder::write))
        .def("write_int16", py::overload_cast<int16_t>(&CDNS::CdnsEncoder::write))
        .def("write_int32", py::overload_cast<int32_t>(&CDNS::CdnsEncoder::write))
        .def("write_int64", py::overload_cast<int64_t>(&CDNS::CdnsEncoder::write))
        .def("rotate_output", &CDNS::CdnsEncoder::rotate_output<std::string>)
        .def("rotate_output", &CDNS::CdnsEncoder::rotate_output<int>);
}
