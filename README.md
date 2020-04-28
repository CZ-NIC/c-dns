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
CDNS::FilePreamble fp;
CDNS::CdnsExporter exporter(fp, "output.out", CDNS::CborOutputCompression::XZ);
CDNS::QueryResponse qr;

qr.client_port = 1234;
exporter.buffer_qr(qr);
exporter.write_block();
```

## TODO-List

* [ ] Implement C-DNS decoder
* [ ] Enhance library interface
