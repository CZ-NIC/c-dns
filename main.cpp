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

    const uint32_t qr_hints =
        CDNS::QueryResponseHintsMask::time_offset |
        CDNS::QueryResponseHintsMask::client_address_index |
        CDNS::QueryResponseHintsMask::client_port |
        CDNS::QueryResponseHintsMask::transaction_id |
        CDNS::QueryResponseHintsMask::qr_signature_index |
        CDNS::QueryResponseHintsMask::client_hoplimit |
        CDNS::QueryResponseHintsMask::response_delay |
        CDNS::QueryResponseHintsMask::query_name_index |
        CDNS::QueryResponseHintsMask::query_size |
        CDNS::QueryResponseHintsMask::response_size |
        CDNS::QueryResponseHintsMask:: response_processing_data |
        CDNS::QueryResponseHintsMask::query_question_sections |
        CDNS::QueryResponseHintsMask::query_answer_sections |
        CDNS::QueryResponseHintsMask::query_authority_sections |
        CDNS::QueryResponseHintsMask::query_additional_sections |
        CDNS::QueryResponseHintsMask::response_answer_sections |
        CDNS::QueryResponseHintsMask::response_authority_sections |
        CDNS::QueryResponseHintsMask::response_additional_sections;

    const uint32_t qr_signature_hints =
        CDNS::QueryResponseSignatureHintsMask::server_address_index |
        CDNS::QueryResponseSignatureHintsMask::server_port |
        CDNS::QueryResponseSignatureHintsMask::qr_transport_flags |
        CDNS::QueryResponseSignatureHintsMask::qr_type |
        CDNS::QueryResponseSignatureHintsMask::qr_sig_flags |
        CDNS::QueryResponseSignatureHintsMask::query_opcode |
        CDNS::QueryResponseSignatureHintsMask::qr_dns_flags |
        CDNS::QueryResponseSignatureHintsMask::query_rcode |
        CDNS::QueryResponseSignatureHintsMask::query_classtype_index |
        CDNS::QueryResponseSignatureHintsMask::query_qdcount |
        CDNS::QueryResponseSignatureHintsMask::query_ancount |
        CDNS::QueryResponseSignatureHintsMask::query_nscount |
        CDNS::QueryResponseSignatureHintsMask::query_arcount |
        CDNS::QueryResponseSignatureHintsMask::query_edns_version |
        CDNS::QueryResponseSignatureHintsMask::query_udp_size |
        CDNS::QueryResponseSignatureHintsMask::query_opt_rdata_index |
        CDNS::QueryResponseSignatureHintsMask::response_rcode;

    const uint8_t rr_hints =
        CDNS::RrHintsMask::ttl |
        CDNS::RrHintsMask::rdata_index;

    const uint8_t other_data_hints =
        CDNS::OtherDataHintsMask::malformed_messages |
        CDNS::OtherDataHintsMask::address_event_counts;

    CDNS::StorageParameters storage_parameter;
    storage_parameter.storage_hints.query_response_hints = qr_hints;
    storage_parameter.storage_hints.query_response_signature_hints = qr_signature_hints;
    storage_parameter.storage_hints.rr_hints = rr_hints;
    storage_parameter.storage_hints.other_data_hints = other_data_hints;

    CDNS::BlockParameters block_parameter;
    block_parameter.storage_parameters = storage_parameter;
    std::vector<CDNS::BlockParameters> block_parameters = {block_parameter};

    CDNS::FilePreamble cdns_h(block_parameters);

    // Pick one - both works
    // #1
    //unsigned char *buff = NULL;
    //size_t buff_len = 0UL;
    // #2
    unsigned char* buff = static_cast<unsigned char *>(calloc(1024UL, 1UL));
    std::size_t buff_len = 1024UL;

    cdns_serialize_file_preamble(cdns_h, &buff, &buff_len);
    if(!buff) {
        return EXIT_FAILURE;
    }
    CDNS_TEST_PRINT(buff, buff_len);

    CDNS::QueryResponse query = {
        CDNS::Timestamp(0, 0),
        0,
        53,
        1,
        0,
        0xFF,
        0,
        0,
        10UL,
        0xFFFF,
        {},
        {},
        {}
    };

    CDNS::CdnsBlock cdns_block(block_parameters[0], 0);
    cdns_push(cdns_block, query);

    buff_len = 1024UL;
    cdns_serialize_block(cdns_block, &buff, &buff_len);
    if(!buff) {
        return EXIT_FAILURE;
    }
    CDNS_TEST_PRINT(buff, buff_len);

    buff_len = 1024UL;
    cdns_serialize_end(cdns_block, &buff, &buff_len);
    if(!buff) {
        return EXIT_FAILURE;
    }
    CDNS_TEST_PRINT(buff, buff_len);

    fprintf(stdout, "\n");
    fflush(stdout);

    free(buff);

    // Try Writer
    CDNS::BaseCborOutputWriter* writer = new CDNS::XzCborOutputWriter("test");
    writer->write("Hello", std::strlen("Hello"));
    writer->write(" world!", std::strlen(" world!"));
    delete writer;

    return EXIT_SUCCESS;
}
