/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "timestamp.h"
#include "py_common.h"

namespace py = pybind11;

void init_timestamp(py::module& m)
{
    py::class_<CDNS::Timestamp>(m, "Timestamp")
        .def(py::init())
        .def(py::init<uint64_t, uint64_t>())
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def("get_time_offset", &CDNS::Timestamp::get_time_offset)
        .def("add_time_offset", &CDNS::Timestamp::add_time_offset)
        .def("write", &CDNS::Timestamp::write)
        .def("read", &CDNS::Timestamp::read)
        .def("reset", &CDNS::Timestamp::reset)
        .def_readwrite("m_secs", &CDNS::Timestamp::m_secs)
        .def_readwrite("m_ticks", &CDNS::Timestamp::m_ticks);
}
