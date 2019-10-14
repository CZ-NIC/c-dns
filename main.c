#include "cdns.h"

#define CDNS_TEST_PRINT \
	for (int i = 0; i < buff_len; ++i) \
		fprintf(stdout, "0x%02X ", buff[i])

int main(int argc, char **argv)
{
	const uint32_t qr_hints
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
	const uint32_t qr_signature_hints
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

	const uint8_t rr_hints
		= TTL_H
		| RDATA_INDEX_H
		;

	const uint8_t other_data_hints
		= MALFORMED_MESSAGES_H
		| ADDRESS_EVENT_COUNTS_H
		;

	const uint8_t opcodes[] = {
		OP_QUERY,
		OP_IQUERY,
		OP_STATUS,
		OP_NOTIFY,
		OP_UPDATE,
		OP_DSO
	};

	static const uint16_t rr_types[] = {
		A,
    	NS,
    	MD,
    	MF,
    	CNAME,
    	SOA,
    	MB,
    	MG,
    	MR,
    	NULL_R,
    	WKS,
    	PTR,
    	HINFO,
    	MINFO,
    	MX,
    	TXT,
    	RP,
    	AFSDB,
    	X25,
    	ISDN,
    	RT,
    	NSAP,
    	NSAP_PTR,
    	SIG,
    	KEY,
    	PX,
    	GPOS,
    	AAAA,
    	LOC,
    	NXT,
    	EID,
    	NIMLOC,
    	SRV,
    	ATMA,
    	NAPTR,
    	KX,
    	CERTIFICATE,
    	A6,
    	DNAM,
    	SINK,
    	OPT,
    	APL,
    	DS,
    	SSHFP,
    	IPSECKEY,
    	RRSIG,
    	NSEC,
    	DNSKEY,
    	DHCID,
    	NSEC3,
    	NSEC3PARAM,
    	TLSA,
    	HIP,
    	NINFO,
    	RKEY,
    	TALINK,
    	CDS,
    	SPF,
    	UINFO,
    	UID,
    	GID,
    	UNSPEC,
    	NID,
    	L32,
    	L64,
    	LP,
    	EU148,
    	EUI64,
    	TKEY,
    	TSIG,
    	IXFR,
    	AXFR,
    	MAILB,
    	MAILA,
    	TYPE_ANY,
    	URI,
    	CAA,
    	TA,
    	DLV
	};


	cdns_storage_parameters_t *storage_parameter = create_storage_parameters(
			TICKS_PER_SECOND,
			20000,
			qr_hints,
			qr_signature_hints,
			rr_hints,
			other_data_hints,
			opcodes,
			sizeof(opcodes) / sizeof(uint8_t),
			rr_types,
			sizeof(rr_types) / sizeof(uint16_t)
		);
	if (storage_parameter == NULL) {
		return EXIT_FAILURE;
	}

	cdns_ctx_t cdns_h = {0};
	if (cdns_init(&cdns_h, 20000, qr_hints, qr_signature_hints, rr_hints, other_data_hints)) {
		return EXIT_FAILURE;
	}

	// Pick one - both works
	// #1
	//unsigned char *buff = NULL;
	//size_t buff_len = 0UL;
	// #2
	unsigned char *buff = (unsigned char *)calloc(1024UL, 1UL);
	size_t buff_len = 1024UL;

	
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
    	.query_name_p = (uint8_t*)"www.nic.cz",
    	.query_size = 10UL,
    	.response_size = 0xFFFF,
    	.response_processing_data_p = NULL,
    	.query_extended_p = NULL,
    	.response_extended_p = NULL
	};
	cdns_push(&cdns_h, &query);

	buff_len = 1024UL;
	cdns_serialize_block(&cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT;

		
	buff_len = 1024UL;
	cdns_serialize_end(&cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT;

	fprintf(stdout, "\n");
	fflush(stdout);
	
	cdns_deinit(&cdns_h);
	free(buff);

	delete_storage_parameters(&storage_parameter);

	return EXIT_SUCCESS;
}
