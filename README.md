# C-DNS Daemon

WORK IN PROCESS - STILL IN DEVELOPMENT

Daemon that converts DNS transmission to C-DNS file format.

## Build

Basic instructions for build. This is not deploy process, it's intended for testing.

    mkdir build
    cd build
    cmake ..
    make
    cd ..
    ./link_and_run_test.sh
    ./a.out

## TODO-List

**High priority**

* [ ] Enhance library interface
* [ ] Finalize file and block preamble
* [ ] Threaded implementation and thread-safety

**Medium priotity**

* [ ] CBOR (*libcbor*) performance improve
* [ ] Replace `cmake` with autotools (`automake`)

**Low priority**

* [ ] Deploy

# Development info

## Input format (DNSTap)

**Query**

    +----------+--------------------+
    |          | type               |
    |          | socket_family      |
    |          | socket_protocol    |
    |          | query_address      |
    | message  | query_port         |
    |          | query_message      |
    |          | query_time_sec     |
    |          | query_time_nsec    |
    +----------+--------------------+

**Response**

    +----------+--------------------+
    |          | type               |
    |          | socket_family      |
    |          | socket_protocol    |
    |          | query_address      |
    | message  | query_port         |
    |          | response_message   |
    |          | response_time_sec  |
    |          | response_time_nsec |
    +----------+--------------------+

**Values info**

    message.type := [ Query / Response ]
    message.socket_family := [ IPv4 / IPv6 ]
    message.socket_protocol := [ UDP / TCP ]
    message.[ query / response ]_message := DNS message in wire format
