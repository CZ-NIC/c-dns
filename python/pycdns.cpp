#include <pybind11/pybind11.h>
#include <boost/optional.hpp>
#include "py_common.h"

namespace py = pybind11;

void init_format_specification(py::module&);
void init_dns(py::module&);
void init_writer(py::module&);
void init_cdns_encoder(py::module&);
void init_cdns_decoder(py::module&);
void init_timestamp(py::module&);
void init_file_preamble(py::module&);
void init_block_table(py::module&);
void init_block(py::module&);
void init_interface(py::module&);
void init_cdns(py::module&);

PYBIND11_MODULE(pycdns, m)
{
    init_format_specification(m);
    init_dns(m);
    init_writer(m);
    init_cdns_encoder(m);
    init_cdns_decoder(m);
    init_timestamp(m);
    init_file_preamble(m);
    init_block_table(m);
    init_block(m);
    init_interface(m);
    init_cdns(m);
}
