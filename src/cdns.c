#include "cdns.h"

int cdns_init(cdns_ctx_t *ctx, const unsigned int block_size)
{
	if (ctx == NULL) {
		return E_ERROR;
	}
	assert(block_size);

	//opts
	ctx->block_size = block_size;

	//ctx
	memset(&(ctx->stats), 0, sizeof(cdns_block_statistics_t));

	return E_SUCCESS;
}

int cdns_push(cdns_ctx_t *ctx, const cdns_query_response_t *qr)
{
	if (ctx == NULL) {
		return E_ERROR;
	}
	if (qr == NULL) {
		return E_ERROR;
	}


	//TODO - most important part => add Q/R to block
	// need to be thread-safe

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.1.1 **/
static int _cdns_init_fp_bp_sp_storage_hints(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	uint32_t query_response_hints_val 
			= TIME_OFFSET_H 
			| CLIENT_PORT_H 
			| TRANSACTION_ID_H 
			| CLIENT_HOPLIMIT_H 
			| RESPONSE_DELAY_H
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
			
	struct cbor_pair query_response_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSE_HINTS )),
		.value	= cbor_move(cbor_build_uint32( htobe32(query_response_hints_val) ))
	};

	uint32_t query_response_signature_hints_val
			= SERVER_PORT_H
			| QR_TRANSPORT_FLAGS_H
			| QR_TYPE_H
			| QR_SIG_FLAGS_H
			| QUERY_OPCODE_H
			| QR_DNS_FLAGS_H
			| QUERY_RCODE_H
			| QUERY_QDCOUNT_H
			| QUERY_ANCOUNT_H
			| QUERY_NSCOUNT_H
			| QUERY_ARCOUNT_H
			| QUERY_EDNS_VERSION_H
			| QUERY_UDP_SIZE_H
			| RESPONSE_RCODE_H
			;
	struct cbor_pair query_response_signature_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSE_SIGNATURE_HINTS )),
		.value	= cbor_move(cbor_build_uint32( htobe32(query_response_signature_hints_val) ))
	};

	uint8_t rr_hints_val
			= TTL_H
			;
	struct cbor_pair rr_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR_HINTS )),
		.value	= cbor_move(cbor_build_uint8( rr_hints_val ))
	};

	uint8_t other_data_hints_val
			= MALFORMED_MESSAGES_H
			| ADDRESS_EVENT_COUNTS_H
			;
	struct cbor_pair other_data_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)OTHER_DATA_HINTS )),
		.value	= cbor_move(cbor_build_uint8( other_data_hints_val ))
	};

	cbor_map_add(root, query_response_hints);
	cbor_map_add(root, query_response_signature_hints);
	cbor_map_add(root, rr_hints);
	cbor_map_add(root, other_data_hints);
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.1 **/
static int _cdns_init_fp_bp_storage_parameters(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair ticks_per_second = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)TICKS_PER_SECOND )),
		.value	= cbor_move(cbor_build_uint64( htobe64(CLOCKS_PER_SEC) ))
	};

	struct cbor_pair max_block_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MAX_BLOCK_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->block_size) ))
	};

	cbor_item_t *storage_hints_map = cbor_new_definite_map(STORAGE_HINTS_SIZE);
	_cdns_init_fp_bp_sp_storage_hints(ctx, storage_hints_map);

	struct cbor_pair storage_hints = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)STORAGE_HINTS )),
		.value	= cbor_move( storage_hints_map )
	};

	cbor_item_t *opcodes_array = cbor_new_indefinite_array(); //TODO

	struct cbor_pair opcodes = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)OPCODES )),
		.value	= cbor_move( opcodes_array )
	};

	cbor_item_t *rr_types_array = cbor_new_indefinite_array(); //TODO

	struct cbor_pair rr_types = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR_TYPES )),
		.value	= cbor_move( rr_types_array )
	};

	cbor_map_add(root, ticks_per_second);
	cbor_map_add(root, max_block_items);
	cbor_map_add(root, storage_hints);
	cbor_map_add(root, opcodes);
	cbor_map_add(root, rr_types);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1 **/
static int _cdns_init_fp_block_parameters(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	cbor_item_t *block_parameters_map = cbor_new_definite_map(BLOCK_PARAMETERS_SIZE);

	cbor_item_t *storage_parameters_map = cbor_new_definite_map(STORAGE_PARAMETERS_SIZE);
	_cdns_init_fp_bp_storage_parameters(ctx, storage_parameters_map);

	struct cbor_pair storage_parameters = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)STORAGE_PARAMETERS )),
		.value	= cbor_move( storage_parameters_map )
	};

	cbor_map_add(block_parameters_map, storage_parameters);
	//root[COLLECTION_PARAMETERS] is optional => todo

	cbor_array_set(root, 0UL, cbor_move(block_parameters_map));

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1 **/
static int _cdns_init_file_preamble(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair major_version = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MAJOR_FORMAT_VERSION )),
		.value	= cbor_move(cbor_build_uint8( (uint8_t)VERSION_MAJOR ))
	};

	struct cbor_pair minor_version = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MINOR_FORMAT_VERSION )),
		.value	= cbor_move(cbor_build_uint8( (uint8_t)VERSION_MINOR ))
	};

	/**OPTIONAL**/ struct cbor_pair private_version = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)PRIVATE_VERSION )),
		.value	= cbor_move(cbor_build_uint8( (uint8_t)VERSION_PRIVATE ))
	};


	/** One is MANDATORY (default), rest is OPTIONAL **/
	cbor_item_t *block_parameters_array = cbor_new_definite_array(1UL);
	_cdns_init_fp_block_parameters(ctx, block_parameters_array);
	
	struct cbor_pair block_parameters = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_PARAMETERS )),
		.value	= cbor_move( block_parameters_array )
	};

	cbor_map_add(root, major_version);
	cbor_map_add(root, minor_version);
	/**OPTIONAL**/ cbor_map_add(root, private_version);
	cbor_map_add(root, block_parameters);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3 **/
int cdns_serialize_file_preamble(const cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size)
{
	if(ctx == NULL) {
		return E_ERROR;
	}
	if(buff_size == NULL) {
		return E_ERROR;
	}

    cbor_item_t *root = cbor_new_definite_array(FILE_SIZE); //0x83
    if(!root) {
        return E_ERROR;
    }
    
	cbor_array_set(root, FILE_TYPE_ID, cbor_move(cbor_build_string("C-DNS")));

	cbor_item_t *file_preamble = cbor_new_definite_map( FILE_PREAMBLE_SIZE );
	_cdns_init_file_preamble(ctx, file_preamble);
	cbor_array_set(root, FILE_PREAMBLE, cbor_move(file_preamble));

	cbor_item_t *block_array = cbor_new_indefinite_array();
	cbor_array_set(root, FILE_BLOCKS, cbor_move(block_array));

	size_t len = 0;
	if (*buff) {
		len = cbor_serialize(root, *buff, *buff_size);
	} else {
		len = cbor_serialize_alloc(root, buff, buff_size);
	}
	--len; //Remove end of indefinite array (for append in future)

	*buff_size = len;

	cbor_decref(&root);

	return E_SUCCESS;
}

static int _cdns_init_bp_earliest_time(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	cbor_array_set(root, EPOCH_TIME, cbor_move(cbor_build_uint64( htobe64(time(NULL)) )));
	cbor_array_set(root, TICKS, cbor_move(cbor_build_uint64( htobe64(0UL) )));

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.1 **/
static int _cdns_init_block_preamble(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(root);

	cbor_item_t *earliest_time_array = cbor_new_definite_array(EARLIEST_TIME_SIZE);
	_cdns_init_bp_earliest_time(ctx, earliest_time_array);

	struct cbor_pair earliest_time = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)EARLIEST_TIME )),
		.value	= cbor_move( earliest_time_array )
	};

	struct cbor_pair block_parameters_index = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_PARAMETERS_INDEX )),
		.value	= cbor_move(cbor_build_uint32( htobe32(0) )) //TODO - 0 is default value
	};

	cbor_map_add(root, earliest_time);
	cbor_map_add(root, block_parameters_index);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.2 **/
static int _cdns_init_block_statistics(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair processed_messages = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)PROCESSED_MESSAGES )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.processed_messages) ))
	};

	struct cbor_pair qr_data_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QR_DATA_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.qr_data_items) ))
	};

	struct cbor_pair unmatched_queries = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)UNMATCHED_QUERIES )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.unmatched_queries) ))
	};

	struct cbor_pair unmatched_responses = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)UNMATCHED_RESPONSES )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.unmatched_responses) ))
	};

	struct cbor_pair discarded_opcode = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)DISCARDED_OPCODE )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.discarded_opcode) ))
	};

	struct cbor_pair malformed_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MALFORMED_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->stats.malformed_items) ))
	};

	cbor_map_add(root, processed_messages);
	cbor_map_add(root, qr_data_items);
	cbor_map_add(root, unmatched_queries);
	cbor_map_add(root, unmatched_responses);
	cbor_map_add(root, discarded_opcode);
	cbor_map_add(root, malformed_items);

	return E_SUCCESS;
}

static int _cdns_init_bt_ip_address(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.3 **/
static int _cdns_init_block_tables(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	cbor_item_t *ip_address_array = cbor_new_definite_array(ctx->block_size);
	_cdns_init_bt_ip_address(ctx, ip_address_array);

	struct cbor_pair ip_address = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)IP_ADDRESS )),
		.value	= cbor_move( ip_address_array )
	};

	cbor_item_t *classtype_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair classtype = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)CLASSTYPE )),
		.value	= cbor_move( classtype_array )
	};

	cbor_item_t *name_rdata_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair name_rdata = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)NAME_RDATA )),
		.value	= cbor_move( name_rdata_array )
	};

	cbor_item_t *qr_siq_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair qr_sig = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QR_SIG )),
		.value	= cbor_move( qr_siq_array )
	};

	cbor_item_t *qlist_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair qlist = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QLIST )),
		.value	= cbor_move( qlist_array )
	};

	cbor_item_t *qrr_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair qrr = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QRR )),
		.value	= cbor_move( qrr_array )
	};

	cbor_item_t *rrlist_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair rrlist = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RRLIST )),
		.value	= cbor_move( rrlist_array )
	};

	cbor_item_t *rr_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair rr = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR )),
		.value	= cbor_move( rr_array )
	};

	cbor_item_t *malformed_message_data_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair malformed_message_data = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MALFORMED_MESSAGE_DATA )),
		.value	= cbor_move( malformed_message_data_array )
	};

	cbor_map_add(root, ip_address);
	cbor_map_add(root, classtype);
	cbor_map_add(root, name_rdata);
	cbor_map_add(root, qr_sig);
	cbor_map_add(root, qlist);
	cbor_map_add(root, qrr);
	cbor_map_add(root, rrlist);
	cbor_map_add(root, rr);
	cbor_map_add(root, malformed_message_data);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2 **/
int cdns_serialize_block(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size)
{
	if (ctx == NULL) {
		return E_ERROR;
	}
	if (buff_size == NULL) {
		return E_ERROR;
	}


	cbor_item_t *root = cbor_new_definite_map( BLOCK_SIZE );

	cbor_item_t *block_preamble_map = cbor_new_definite_map(BLOCK_PREAMBLE_SIZE);
	_cdns_init_block_preamble(ctx, block_preamble_map);

	struct cbor_pair block_preamble = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_PREAMBLE )),
		.value	= cbor_move( block_preamble_map )
	};

	cbor_item_t *block_statistics_map = cbor_new_definite_map(BLOCK_STATISTICS_SIZE);
	_cdns_init_block_statistics(ctx, block_statistics_map);

	struct cbor_pair block_statistics = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_STATISTICS )),
		.value	= cbor_move( block_statistics_map )
	};

	cbor_item_t *block_tables_map = cbor_new_definite_map(BLOCK_TABLES_SIZE);
	_cdns_init_block_tables(ctx, block_tables_map);

	struct cbor_pair block_tables = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_TABLES )),
		.value	= cbor_move( block_tables_map )
	};

	cbor_item_t *query_responses_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair query_responses = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSES )),
		.value	= cbor_move( query_responses_array )
	};

	cbor_item_t *address_event_counts_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair address_event_counts = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)ADDRESS_EVENT_COUNTS )),
		.value	= cbor_move( address_event_counts_array )
	};

	cbor_item_t *malformed_messages_array = cbor_new_definite_array(ctx->block_size);
	//TODO init

	struct cbor_pair malformed_messages = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MALFORMED_MESSAGES )),
		.value	= cbor_move( malformed_messages_array )
	};

	cbor_map_add(root, block_preamble);
	cbor_map_add(root, block_statistics);
	cbor_map_add(root, block_tables);
	cbor_map_add(root, query_responses);
	cbor_map_add(root, address_event_counts);
	cbor_map_add(root, malformed_messages);
	

	size_t len = 0;
	if (*buff) {
		len = cbor_serialize(root, *buff, *buff_size);
	} else {
		len = cbor_serialize_alloc(root, buff, buff_size);
	}
	*buff_size = len;

	cbor_decref(&root);

	//TODO clear all buffers
	memset(&(ctx->stats), 0, sizeof(cdns_block_statistics_t));
	
	return E_SUCCESS;
}

int cdns_serialize_end(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size)
{
	if (ctx == NULL) {
		return E_ERROR;
	}
	if (buff_size == NULL) {
		return E_ERROR;
	}
	

	if (*buff == NULL) {
		*buff = (unsigned char *)calloc(2UL, 1UL);
	}
	
	(*buff)[0] = 0xFF;
	*buff_size = 1UL;

	return E_SUCCESS;
}
