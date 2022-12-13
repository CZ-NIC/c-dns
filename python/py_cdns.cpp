#include <tuple>
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include "cdns.h"
#include "py_common.h"

namespace py = pybind11;

void init_cdns(py::module& m)
{
    py::class_<std::ifstream>(m, "Ifstream")
        .def(py::init<const std::string&>());

    py::class_<CDNS::CdnsExporter>(m, "CdnsExporter")
        .def(py::init<CDNS::FilePreamble&, const std::string&, CDNS::CborOutputCompression>())
        .def(py::init<CDNS::FilePreamble&, const int&, CDNS::CborOutputCompression>())
        .def("buffer_qr", &CDNS::CdnsExporter::buffer_qr, py::arg("qr"),
            py::arg("stats") = py::none())
        .def("buffer_aec", &CDNS::CdnsExporter::buffer_aec, py::arg("aec"),
            py::arg("stats") = py::none())
        .def("buffer_mm", &CDNS::CdnsExporter::buffer_mm, py::arg("mm"),
            py::arg("stats") = py::none())
        .def("write_block", py::overload_cast<CDNS::CdnsBlock&>(&CDNS::CdnsExporter::write_block))
        .def("write_block", py::overload_cast<>(&CDNS::CdnsExporter::write_block))
        .def("rotate_output", &CDNS::CdnsExporter::rotate_output<std::string>)
        .def("rotate_output", &CDNS::CdnsExporter::rotate_output<int>)
        .def("get_block_item_count", &CDNS::CdnsExporter::get_block_item_count)
        .def("get_block_qr_count", &CDNS::CdnsExporter::get_block_qr_count)
        .def("get_block_aec_count", &CDNS::CdnsExporter::get_block_aec_count)
        .def("get_block_mm_count", &CDNS::CdnsExporter::get_block_mm_count)
        .def("get_blocks_written_count", &CDNS::CdnsExporter::get_blocks_written_count)
        .def("add_block_parameters", &CDNS::CdnsExporter::add_block_parameters)
        .def("set_active_block_parameters", &CDNS::CdnsExporter::set_active_block_parameters)
        .def("get_active_block_parameters", &CDNS::CdnsExporter::get_active_block_parameters)
        .def("get_active_block_parameters_ref", &CDNS::CdnsExporter::get_active_block_parameters_ref,
            py::return_value_policy::reference_internal);

    py::class_<CDNS::CdnsReader>(m, "CdnsReader")
        .def(py::init<std::ifstream&>())
        .def(py::init<std::istream&>())
        .def(py::init<std::istringstream&>())
        .def("read_block", [](CDNS::CdnsReader& self) {
            bool end = false;
            auto ret = self.read_block(end);
            return std::make_tuple(std::move(ret), end);
        })
        .def_readwrite("m_file_preamble", &CDNS::CdnsReader::m_file_preamble);
}
