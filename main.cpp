#include <iostream>
#include <vector>
#include <string>

#include "cdns.h"

void CDNS_TEST_PRINT(unsigned char *buff, std::size_t buff_len) {
	for (unsigned i = 0; i < buff_len; ++i) {
		fprintf(stdout, "0x%02X ", buff[i]);
	}
}

int main(int argc, char **argv)
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

	const std::vector<CDNS::OpCodes> opcodes {
		CDNS::OpCodes::OP_QUERY,
		CDNS::OpCodes::OP_IQUERY,
		CDNS::OpCodes::OP_STATUS,
		CDNS::OpCodes::OP_NOTIFY,
		CDNS::OpCodes::OP_UPDATE,
		CDNS::OpCodes::OP_DSO
	};

	const std::vector<CDNS::RrTypes> rr_types {
		CDNS::RrTypes::A,
    	CDNS::RrTypes::NS,
    	CDNS::RrTypes::MD,
    	CDNS::RrTypes::MF,
    	CDNS::RrTypes::CNAME,
    	CDNS::RrTypes::SOA,
    	CDNS::RrTypes::MB,
    	CDNS::RrTypes::MG,
    	CDNS::RrTypes::MR,
    	CDNS::RrTypes::NULL_R,
    	CDNS::RrTypes::WKS,
    	CDNS::RrTypes::PTR,
    	CDNS::RrTypes::HINFO,
    	CDNS::RrTypes::MINFO,
    	CDNS::RrTypes::MX,
    	CDNS::RrTypes::TXT,
    	CDNS::RrTypes::RP,
    	CDNS::RrTypes::AFSDB,
    	CDNS::RrTypes::X25,
    	CDNS::RrTypes::ISDN,
    	CDNS::RrTypes::RT,
    	CDNS::RrTypes::NSAP,
    	CDNS::RrTypes::NSAP_PTR,
    	CDNS::RrTypes::SIG,
    	CDNS::RrTypes::KEY,
    	CDNS::RrTypes::PX,
    	CDNS::RrTypes::GPOS,
    	CDNS::RrTypes::AAAA,
    	CDNS::RrTypes::LOC,
    	CDNS::RrTypes::NXT,
    	CDNS::RrTypes::EID,
    	CDNS::RrTypes::NIMLOC,
    	CDNS::RrTypes::SRV,
    	CDNS::RrTypes::ATMA,
    	CDNS::RrTypes::NAPTR,
    	CDNS::RrTypes::KX,
    	CDNS::RrTypes::CERTIFICATE,
    	CDNS::RrTypes::A6,
    	CDNS::RrTypes::DNAM,
    	CDNS::RrTypes::SINK,
    	CDNS::RrTypes::OPT,
    	CDNS::RrTypes::APL,
    	CDNS::RrTypes::DS,
    	CDNS::RrTypes::SSHFP,
    	CDNS::RrTypes::IPSECKEY,
    	CDNS::RrTypes::RRSIG,
    	CDNS::RrTypes::NSEC,
    	CDNS::RrTypes::DNSKEY,
    	CDNS::RrTypes::DHCID,
    	CDNS::RrTypes::NSEC3,
    	CDNS::RrTypes::NSEC3PARAM,
    	CDNS::RrTypes::TLSA,
    	CDNS::RrTypes::HIP,
    	CDNS::RrTypes::NINFO,
    	CDNS::RrTypes::RKEY,
    	CDNS::RrTypes::TALINK,
    	CDNS::RrTypes::CDS,
    	CDNS::RrTypes::SPF,
    	CDNS::RrTypes::UINFO,
    	CDNS::RrTypes::UID,
    	CDNS::RrTypes::GID,
    	CDNS::RrTypes::UNSPEC,
    	CDNS::RrTypes::NID,
    	CDNS::RrTypes::L32,
    	CDNS::RrTypes::L64,
    	CDNS::RrTypes::LP,
    	CDNS::RrTypes::EU148,
    	CDNS::RrTypes::EUI64,
    	CDNS::RrTypes::TKEY,
    	CDNS::RrTypes::TSIG,
    	CDNS::RrTypes::IXFR,
    	CDNS::RrTypes::AXFR,
    	CDNS::RrTypes::MAILB,
    	CDNS::RrTypes::MAILA,
    	CDNS::RrTypes::TYPE_ANY,
    	CDNS::RrTypes::URI,
    	CDNS::RrTypes::CAA,
    	CDNS::RrTypes::TA,
    	CDNS::RrTypes::DLV
	};

	CDNS::StorageParameters storage_parameter = {
		1000000,
		10000,
		{
			qr_hints,
			qr_signature_hints,
			rr_hints,
			other_data_hints
		},
		opcodes,
		rr_types,
		{},
		0,
		0,
		0,
		0,
		"",
		""
	};

	std::vector<CDNS::BlockParameters> block_parameters = {
		{
			storage_parameter,
			{}
		}
	};

	CDNS::FilePreamble cdns_h(block_parameters);

	// Pick one - both works
	// #1
	//unsigned char *buff = NULL;
	//size_t buff_len = 0UL;
	// #2
	unsigned char *buff = static_cast<unsigned char *>(calloc(1024UL, 1UL));
	std::size_t buff_len = 1024UL;
	
	cdns_serialize_file_preamble(cdns_h, &buff, &buff_len);
	if(!buff) {
		return EXIT_FAILURE;
	}
	CDNS_TEST_PRINT(buff, buff_len);

	CDNS::QueryResponse query = {
		0,
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

	CDNS::CdnsBlock cdns_block;
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

	return EXIT_SUCCESS;
}
