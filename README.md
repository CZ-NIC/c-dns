# C-DNS Library

Library that converts DNS transmission to [C-DNS](https://tools.ietf.org/html/rfc8618), the compacted DNS Packet Capture format.

## Dependencies

This project has the following dependencies:

* [CMake >= 3.5] (https://cmake.org/)
* [Boost] (https://www.boost.org/)
* [zlib] (https://www.zlib.net/)
* [XZ Utils] (https://tukaani.org/xz/)

## Build

Basic build instructions using CMake.
```shell
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
make
make install
```
If you don't want to build the test suite with the library, you can omit `-DBUILD_TESTS` option.

To generate Doxygen documentation run `make doc`.

## Basic Usage

To use the C-DNS library you only have to include the `<cdns/cdns.h>` header file.
```cpp
#include <cdns/cdns.h>
...
// Create C-DNS file
CDNS::FilePreamble fp;
CDNS::CdnsExporter* exporter = new CDNS::CdnsExporter(fp, "output.out", CDNS::CborOutputCompression::NO_COMPRESSION);
CDNS::GenericQueryResponse qr;

qr.client_port = 1234;
exporter->buffer_qr(qr);
exporter->write_block();
delete exporter;

// Read C-DNS file
std::ifstream ifs("output.out", std::ifstream::binary);
CDNS::CdnsReader reader = new CDNS::CdnsReader(ifs);

bool end = false;
while (true) {
    CDNS::CdnsBlockRead block = reader->read_block(end);
    if (end)
        break;

    while (true) {
        CDNS::GenericQueryResponse gqr = block.read_generic_qr(end);
        if (end)
            break;
        ...
    }
}
delete reader;
```
