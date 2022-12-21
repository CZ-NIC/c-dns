/**
 * Copyright © 2022 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <boost/optional.hpp>
#include <boost/none.hpp>

namespace py = pybind11;

using boost::optional;

namespace PYBIND11_NAMESPACE { namespace detail {
    template <typename T>
    struct type_caster<boost::optional<T>> : optional_caster<boost::optional<T>> {};
}}
