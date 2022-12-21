# C-DNS Library

Library that converts DNS transmission to [C-DNS](https://tools.ietf.org/html/rfc8618), the compacted DNS Packet Capture format.

## Dependencies

This project has the following dependencies:

* [CMake >= 3.5] (https://cmake.org/)
* [Boost] (https://www.boost.org/)
* [zlib] (https://www.zlib.net/)
* [XZ Utils] (https://tukaani.org/xz/)

Optional:
* [GoogleTest] (https://github.com/google/googletest)
* [pybind11] (https://github.com/pybind/pybind11)

## Build

Basic build instructions using CMake.
```shell
mkdir build
cd build
cmake -DBUILD_PYTHON_BINDINGS=ON -DBUILD_TESTS=ON ..
make
make install
```
If you don't want to build the Python bindings, you can omit `-DBUILD_PYTHON_BINDINGS` option.
If you don't want to build the test suite with the library, you can omit `-DBUILD_TESTS` option.

To generate Doxygen documentation run `make doc`. Doxygen documentation for current release can be found [here](https://knot.pages.nic.cz/c-dns/).

# Installation from packages
Packages for Debian 11, 10, 9; Ubuntu 22.04, 20.04, 18.04, 16.04; Fedora 36, 35, 34, Rawhide;
EPEL 8 and Arch are available from [OBS (openSUSE Build Service)](https://build.opensuse.org/project/show/home:CZ-NIC:dns-probe).

Python bindings are, for the moment, available only when building the library from sources. Linux packages for Python bindings will be available soon.

#### Debian/Ubuntu
```shell
sudo apt-get update
sudo apt-get install -y lsb-release curl gpg

DISTRO=$(lsb_release -i -s)
RELEASE=$(lsb_release -r -s)
if [[ $DISTRO == "Ubuntu" ]]; then DISTRO="xUbuntu"; fi
if [[ $DISTRO == "Debian" && "$RELEASE" =~ ^9\..*$ ]]; then RELEASE="9.0"; fi

echo "deb http://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/${DISTRO}_${RELEASE}/ /" | sudo tee /etc/apt/sources.list.d/dns-probe.list
curl -fsSL https://download.opensuse.org/repositories/home:CZ-NIC:/dns-probe/${DISTRO}_${RELEASE}/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/dns-probe.gpg > /dev/null
sudo apt-get update
sudo apt-get install libcdns1 libcdns-dev
```

#### Fedora
```shell
sudo dnf config-manager --add-repo https://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Fedora_$(cut -d: -f5 /etc/system-release-cpe | cut -d. -f1)/home:CZ-NIC:dns-probe.repo
sudo dnf install libcdns libcdns-devel
```

#### Fedora Rawhide
```shell
sudo dnf config-manager --add-repo https://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Fedora_Rawhide/home:CZ-NIC:dns-probe.repo
sudo dnf install libcdns libcdns-devel
```

#### EPEL 8
```shell
cd /etc/yum.repos.d/
sudo wget https://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Fedora_EPEL_8_CentOS/home:CZ-NIC:dns-probe.repo
sudo yum install libcdns libcdns-devel
```

#### Arch
```shell
echo "[home_CZ-NIC_dns-probe_Arch]" | sudo tee -a /etc/pacman.conf
echo "Server = https://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Arch/$(uname -m)" | sudo tee -a /etc/pacman.conf

key=$(curl -fsSL https://download.opensuse.org/repositories/home:/CZ-NIC:/dns-probe/Arch/$(uname -m)/home_CZ-NIC_dns-probe_Arch.key)
fingerprint=$(gpg --quiet --with-colons --import-options show-only --import --fingerprint <<< "${key}" | awk -F: '$1 == "fpr" { print $10 }')

sudo pacman-key --init
sudo pacman-key --add - <<< "${key}"
sudo pacman-key --lsign-key "${fingerprint}"

sudo pacman -Sy home_CZ-NIC_dns-probe_Arch/c-dns
```

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
