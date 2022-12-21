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
#include "block_table.h"
#include "block.h"
#include "py_common.h"

namespace py = pybind11;

template<typename T, typename K = T>
void declare_block_table(py::module& m, std::string typestr) {
    using Class = CDNS::BlockTable<T, K>;
    std::string pyclass_name = std::string("BlockTable") + typestr;
    py::class_<Class>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
        .def(py::init<>())
        .def("find", [](Class& self, const K& key) {
            CDNS::index_t index = 0;
            auto ret = self.find(key, index);
            return std::make_tuple(ret, index);
        })
        .def("add_value", py::overload_cast<const T&>(&Class::add_value))
        .def("add", &Class::add)
        .def("clear", &Class::clear)
        .def("__getitem__", &Class::operator[], py::return_value_policy::reference_internal)
        .def("size", &Class::size)
        .def("begin", &Class::begin)
        .def("end", &Class::end);
}

void init_block_table(py::module& m)
{
    declare_block_table<CDNS::StringItem>(m, "StringItem");
    declare_block_table<CDNS::ClassType>(m, "ClassType");
    declare_block_table<CDNS::QueryResponseSignature>(m, "QueryResponseSignature");
    declare_block_table<CDNS::IndexListItem>(m, "IndexListItem");
    declare_block_table<CDNS::Question>(m, "Question");
    declare_block_table<CDNS::RR>(m, "RR");
    declare_block_table<CDNS::MalformedMessageData>(m, "MalformedMessageData");
}
