#include "cdns.h"

#define CDNS_TEST_PRINT \
	for (int i = 0; i < buff_len; ++i) \
		fprintf(stdout, "0x%02X ", buff[i])

int main(int argc, char **argv)
{

	uint32_t qr_hints
		= TIME_OFFSET_H
    	| CLIENT_ADDRESS_INDEX_H
    	| CLIENT_PORT_H
    	| TRANSACTION_ID_H
    	| QR_SIGNATURE_INDEX_H
    	| CLIENT_HOPLIMIT_H
    	| RESPONSE_DELAY_H
    	| QUERY_NAME_INDEX_H
    	| QUERY_SIZE_H
    	| RESPONSE_SIZE_H
    	| RESPONSE_PROCESSING_DATA_H
    	| QUERY_QUESTION_SECTIONS_H
    	| QUERY_ANSWER_SECTIONS_H
    	| QUERY_AUTHORITY_SECTIONS_H
    	| QUERY_ADDITIONAL_SECTIONS_H
    	| RESPONSE_ANSWER_SECTIONS_H
    	| RESPONSE_AUTHORITY_SECTIONS_H
    	| RESPONSE_ADDITIONAL_SECTIONS_H
		;
	uint32_t qr_signature_hints
		= SERVER_ADDRESS_INDEX_H
    	| SERVER_PORT_H
    	| QR_TRANSPORT_FLAGS_H
    	| QR_TYPE_H
    	| QR_SIG_FLAGS_H
    	| QUERY_OPCODE_H
    	| QR_DNS_FLAGS_H
    	| QUERY_RCODE_H
    	| QUERY_CLASSTYPE_INDEX_H
    	| QUERY_QDCOUNT_H
    	| QUERY_ANCOUNT_H
    	| QUERY_NSCOUNT_H
    	| QUERY_ARCOUNT_H
    	| QUERY_EDNS_VERSION_H
    	| QUERY_UDP_SIZE_H
    	| QUERY_OPT_RDATA_INDEX_H
    	| RESPONSE_RCODE_H
		;

	uint8_t rr_hints
			= TTL_H
			;

	uint8_t other_data_hints
			= MALFORMED_MESSAGES_H
			| ADDRESS_EVENT_COUNTS_H
			;

	cdns_ctx_t cdns_h = {0};
	if (cdns_init(&cdns_h, 20000, qr_hints, qr_signature_hints)) {
		return EXIT_FAILURE;
	}

	// Pick one - both works
	// #1
	//unsigned char *buff = NULL;
	//size_t buff_len = 0UL;
	// #2
	unsigned char *buff = (unsigned char *)calloc(255UL, 1UL);
	size_t buff_len = 255UL;

	
	cdns_serialize_file_preamble(&cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT;

	cdns_query_response_t query = {
		.time_offset = time(NULL),
    	.client_address_p = NULL,
    	.client_port = 53,
    	.transaction_id = 1,
    	.qr_signature = NULL,
    	.client_hoplimit = 0xFF,
    	.response_delay = time(NULL),
    	.query_name_p = "www.nic.cz",
    	.query_size = 10UL,
    	.response_size = 0xFFFF,
    	.response_processing_data_p = NULL,
    	.query_extended_p = NULL,
    	.response_extended_p = NULL
	};
	cdns_push(&cdns_h, &query);

	buff_len = 255UL;
	cdns_serialize_block(&cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT;

		
	buff_len = 255UL;
	cdns_serialize_end(&cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT;

	fprintf(stdout, "\n");
	fflush(stdout);
	
	free(buff);

	return EXIT_SUCCESS;
}
