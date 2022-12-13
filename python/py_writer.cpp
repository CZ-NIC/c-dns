/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "writer.h"
#include "py_common.h"

namespace py = pybind11;

void init_writer(py::module& m)
{
    py::enum_<CDNS::CborOutputCompression>(m, "CborOutputCompression")
        .value("NO_COMPRESSION", CDNS::CborOutputCompression::NO_COMPRESSION)
        .value("GZIP", CDNS::CborOutputCompression::GZIP)
        .value("XZ", CDNS::CborOutputCompression::XZ)
        .export_values();

    py::register_exception<CDNS::CborOutputException>(m, "CborOutputException");

    py::class_<CDNS::Writer<std::string>>(m, "StringWriter")
        .def(py::init<const std::string&, const std::string>())
        .def("write", &CDNS::Writer<std::string>::write)
        .def("rotate_output", &CDNS::Writer<std::string>::rotate_output);

    py::class_<CDNS::Writer<int>>(m, "IntWriter")
        .def(py::init<const int&, const std::string>())
        .def("write", &CDNS::Writer<int>::write)
        .def("rotate_output", &CDNS::Writer<int>::rotate_output);

    py::class_<CDNS::CborOutputWriter>(m, "CborOutputWriter")
        .def(py::init<const std::string&>())
        .def(py::init<const int&>())
        .def("write", &CDNS::CborOutputWriter::write)
        .def("rotate_output", [](CDNS::CborOutputWriter& self, int arg) {
            return self.rotate_output(arg);
        })
        .def("rotate_output", [](CDNS::CborOutputWriter& self, std::string arg) {
            return self.rotate_output(arg);
        });

    py::class_<CDNS::GzipCborOutputWriter>(m, "GzipCborOutputWriter")
        .def(py::init<const std::string&>())
        .def(py::init<const int&>())
        .def("write", &CDNS::GzipCborOutputWriter::write)
        .def("rotate_output", [](CDNS::GzipCborOutputWriter& self, int arg) {
            return self.rotate_output(arg);
        })
        .def("rotate_output", [](CDNS::GzipCborOutputWriter& self, std::string arg) {
            return self.rotate_output(arg);
        });

    py::class_<CDNS::XzCborOutputWriter>(m, "XzCborOutputWriter")
        .def(py::init<const std::string&>())
        .def(py::init<const int&>())
        .def("write", &CDNS::XzCborOutputWriter::write)
        .def("rotate_output", [](CDNS::XzCborOutputWriter& self, int arg) {
            return self.rotate_output(arg);
        })
        .def("rotate_output", [](CDNS::XzCborOutputWriter& self, std::string arg) {
            return self.rotate_output(arg);
        });
}
