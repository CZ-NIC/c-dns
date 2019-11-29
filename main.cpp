#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "cdns.h"

void CDNS_TEST_PRINT(unsigned char* buff, std::size_t buff_len) {
    for (unsigned i = 0; i < buff_len; ++i) {
        fprintf(stdout, "0x%02X ", buff[i]);
    }
}

int main(int argc, char** argv)
{
    CDNS::FilePreamble fp;
    CDNS::CdnsExporter exporter(fp, "test", CDNS::CborOutputCompression::XZ);
    CDNS::GenericQueryResponse qr;
    CDNS::Timestamp ts = {
        15632486,
        200
    };
    qr.ts = &ts;
    std::string client = "192.168.0.1";
    std::string server = "8.8.8.8";
    qr.client_ip = &client;
    qr.server_ip = &server;
    exporter.buffer_qr(qr);
    exporter.write_block();

    return EXIT_SUCCESS;
}
