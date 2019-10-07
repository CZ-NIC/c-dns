#include "cdns.h"

#define CDNS_TEST_PRINT \
	for (int i = 0; i < buff_len; ++i) \
		fprintf(stdout, "0x%02X ", buff[i])

int main(int argc, char **argv)
{

	cdns_ctx_t cdns_h = {0};
	if (cdns_init(&cdns_h, 10000)) {
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
