/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include <pybind11/functional.h>
#include "cdns_decoder.h"
#include "py_common.h"

namespace py = pybind11;

void init_cdns_decoder(py::module& m)
{
    py::class_<std::istringstream>(m, "Istringstream")
        .def(py::init<const std::string&>());

    py::register_exception<CDNS::CdnsDecoderException>(m, "CdnsDecoderException");
    py::register_exception<CDNS::CdnsDecoderEnd>(m, "CdnsDecoderEnd");

    py::class_<CDNS::CdnsDecoder>(m, "CdnsDecoder")
        .def(py::init<std::ifstream&>())
        .def(py::init<std::istream&>())
        .def(py::init<std::istringstream&>())
        .def("peek_type", &CDNS::CdnsDecoder::peek_type)
        .def("read_unsigned", &CDNS::CdnsDecoder::read_unsigned)
        .def("read_negative", &CDNS::CdnsDecoder::read_negative)
        .def("read_integer", &CDNS::CdnsDecoder::read_integer)
        .def("read_bool", &CDNS::CdnsDecoder::read_bool)
        .def("read_bytestring", [](CDNS::CdnsDecoder& self) {
            auto res = self.read_bytestring();
            return py::bytes(res);
        })
        .def("read_textstring", &CDNS::CdnsDecoder::read_textstring)
        .def("read_array_start", [](CDNS::CdnsDecoder& self) {
            bool indef = false;
            auto res = self.read_array_start(indef);
            return std::make_tuple(res, indef);
        })
        .def("read_array", &CDNS::CdnsDecoder::read_array)
        .def("read_map_start", [](CDNS::CdnsDecoder& self) {
            bool indef = false;
            auto res = self.read_map_start(indef);
            return std::make_tuple(res, indef);
        })
        .def("read_break", &CDNS::CdnsDecoder::read_break)
        .def("skip_item", &CDNS::CdnsDecoder::skip_item);
}
