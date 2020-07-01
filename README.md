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

To generate Doxygen documentation run `make doc`. Doxygen documentation for current release can be found [here](https://knot.pages.nic.cz/c-dns/).

# Installation from packages
Packages for Debian 10 and 9 and Ubuntu 20.04, 18.04 and 16.04 are available from
[OBS (openSUSE Build Service)](https://build.opensuse.org/project/show/home:CZ-NIC:dns-probe).

First you need to add the OBS repository for given distribution to your system's repository list and download the repository's signing key:

##### Debian 10
```shell
sudo echo 'deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Debian_10/ /' > /etc/apt/sources.list.d/dns-probe.list
wget -nv http://download.opensuse.org/repositories/home:CZ-NIC:dns-probe/Debian_10/Release.key -O Release.key
```

##### Debian 9
```shell
sudo echo 'deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Debian_9.0/ /' > /etc/apt/sources.list.d/dns-probe.list
wget -nv http://download.opensuse.org/repositories/home:CZ-NIC:dns-probe/Debian_9.0/Release.key -O Release.key
```

##### Ubuntu 20.04
```shell
sudo echo 'deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/xUbuntu_20.04/ /' > /etc/apt/sources.list.d/dns-probe.list
wget -nv http://download.opensuse.org/repositories/home:CZ-NIC:dns-probe/xUbuntu_20.04/Release.key -O Release.key
```

##### Ubuntu 18.04
```shell
sudo echo 'deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/xUbuntu_18.04/ /' > /etc/apt/sources.list.d/dns-probe.list
wget -nv http://download.opensuse.org/repositories/home:CZ-NIC:dns-probe/xUbuntu_18.04/Release.key -O Release.key
```

##### Ubuntu 16.04
```shell
sudo echo 'deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/xUbuntu_16.04/ /' > /etc/apt/sources.list.d/dns-probe.list
wget -nv http://download.opensuse.org/repositories/home:CZ-NIC:dns-probe/xUbuntu_16.04/Release.key -O Release.key
```

Now you need to add the signing key to your system, update the repository list and then you can finally install the C-DNS library:

```shell
sudo apt-key add - < Release.key
sudo apt-get update
sudo apt-get install libcdns0 libcdns-dev
```

The C-DNS library is separated into two packages. `libcdns0` package installs the shared library and `libcdns-dev` package installs
development headers.

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
