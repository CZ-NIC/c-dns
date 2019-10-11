#include "cdns.h"

int cdns_init(cdns_ctx_t *ctx,
		const unsigned int block_size,
		const uint32_t query_response_hints,
		const uint32_t query_response_signature_hints,
		const uint8_t rr_hints,
		const uint8_t other_data_hints)
{
	if (ctx == NULL) {
		return E_ERROR;
	}
	if (block_size == 0) {
		return E_ERROR;
	}

	//TODO for future, init as alloc
	//		Other solution would be store it locally in library and pass descriptor

	static const uint8_t opcodes[] = {
		(uint8_t)OP_QUERY,
		(uint8_t)OP_IQUERY,
		(uint8_t)OP_STATUS,
		(uint8_t)OP_NOTIFY,
		(uint8_t)OP_UPDATE,
		(uint8_t)OP_DSO
	};

	static const uint16_t rr_types[] = {
		(uint16_t)A,
    	(uint16_t)NS,
    	(uint16_t)MD,
    	(uint16_t)MF,
    	(uint16_t)CNAME,
    	(uint16_t)SOA,
    	(uint16_t)MB,
    	(uint16_t)MG,
    	(uint16_t)MR,
    	(uint16_t)NULL_R,
    	(uint16_t)WKS,
    	(uint16_t)PTR,
    	(uint16_t)HINFO,
    	(uint16_t)MINFO,
    	(uint16_t)MX,
    	(uint16_t)TXT,
    	(uint16_t)RP,
    	(uint16_t)AFSDB,
    	(uint16_t)X25,
    	(uint16_t)ISDN,
    	(uint16_t)RT,
    	(uint16_t)NSAP,
    	(uint16_t)NSAP_PTR,
    	(uint16_t)SIG,
    	(uint16_t)KEY,
    	(uint16_t)PX,
    	(uint16_t)GPOS,
    	(uint16_t)AAAA,
    	(uint16_t)LOC,
    	(uint16_t)NXT,
    	(uint16_t)EID,
    	(uint16_t)NIMLOC,
    	(uint16_t)SRV,
    	(uint16_t)ATMA,
    	(uint16_t)NAPTR,
    	(uint16_t)KX,
    	(uint16_t)CERTIFICATE,
    	(uint16_t)A6,
    	(uint16_t)DNAM,
    	(uint16_t)SINK,
    	(uint16_t)OPT,
    	(uint16_t)APL,
    	(uint16_t)DS,
    	(uint16_t)SSHFP,
    	(uint16_t)IPSECKEY,
    	(uint16_t)RRSIG,
    	(uint16_t)NSEC,
    	(uint16_t)DNSKEY,
    	(uint16_t)DHCID,
    	(uint16_t)NSEC3,
    	(uint16_t)NSEC3PARAM,
    	(uint16_t)TLSA,
    	(uint16_t)HIP,
    	(uint16_t)NINFO,
    	(uint16_t)RKEY,
    	(uint16_t)TALINK,
    	(uint16_t)CDS,
    	(uint16_t)SPF,
    	(uint16_t)UINFO,
    	(uint16_t)UID,
    	(uint16_t)GID,
    	(uint16_t)UNSPEC,
    	(uint16_t)NID,
    	(uint16_t)L32,
    	(uint16_t)L64,
    	(uint16_t)LP,
    	(uint16_t)EU148,
    	(uint16_t)EUI64,
    	(uint16_t)TKEY,
    	(uint16_t)TSIG,
    	(uint16_t)IXFR,
    	(uint16_t)AXFR,
    	(uint16_t)MAILB,
    	(uint16_t)MAILA,
    	(uint16_t)TYPE_ANY,
    	(uint16_t)URI,
    	(uint16_t)CAA,
    	(uint16_t)TA,
    	(uint16_t)DLV
	};

	ctx->options.block_parameters_size = 1U;
	ctx->options.block_parameters = (cdns_block_parameters_t *)calloc(ctx->options.block_parameters_size, sizeof(cdns_block_parameters_t));
	ctx->options.block_parameters[0].block_size = block_size;
	ctx->options.block_parameters[0].query_response_hints = query_response_hints;
	ctx->options.block_parameters[0].query_response_signature_hints = query_response_signature_hints;
	ctx->options.block_parameters[0].rr_hints = rr_hints;
	ctx->options.block_parameters[0].other_data_hints = other_data_hints;

	ctx->options.block_parameters[0].opcodes_size = sizeof(opcodes) / sizeof(uint8_t);
	ctx->options.block_parameters[0].opcodes = (uint8_t *)calloc(ctx->options.block_parameters[0].opcodes_size, sizeof(uint8_t));
	memcpy(ctx->options.block_parameters[0].opcodes, opcodes, sizeof(opcodes));
	
	ctx->options.block_parameters[0].rr_types_size = sizeof(rr_types) / sizeof(uint16_t);
	ctx->options.block_parameters[0].rr_types = (uint16_t *)calloc(ctx->options.block_parameters[0].rr_types_size, sizeof(uint16_t));
	memcpy(ctx->options.block_parameters[0].rr_types, rr_types, sizeof(rr_types));

	// storage
	memset(&(ctx->storage.stats), 0, sizeof(cdns_block_statistics_t));

	return E_SUCCESS;
}

int cdns_deinit(cdns_ctx_t *ctx)
{
	for(size_t idx = 0; idx < ctx->options.block_parameters_size; ++idx) {
		free(ctx->options.block_parameters[idx].opcodes);
		free(ctx->options.block_parameters[idx].rr_types);
	}

	free(ctx->options.block_parameters);
	ctx->options.block_parameters = NULL;

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
static int _cdns_init_fp_bp_sp_storage_hints(const cdns_ctx_t *ctx, const int idx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair query_response_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSE_HINTS )),
		.value	= cbor_move(cbor_build_uint32( ctx->options.block_parameters[idx].query_response_hints ))
	};

	struct cbor_pair query_response_signature_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSE_SIGNATURE_HINTS )),
		.value	= cbor_move(cbor_build_uint32( ctx->options.block_parameters[idx].query_response_signature_hints ))
	};

	struct cbor_pair rr_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR_HINTS )),
		.value	= cbor_move(cbor_build_uint8( ctx->options.block_parameters[idx].rr_hints ))
	};

	struct cbor_pair other_data_hints  = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)OTHER_DATA_HINTS )),
		.value	= cbor_move(cbor_build_uint8( ctx->options.block_parameters[idx].other_data_hints ))
	};

	cbor_map_add(root, query_response_hints);
	cbor_map_add(root, query_response_signature_hints);
	cbor_map_add(root, rr_hints);
	cbor_map_add(root, other_data_hints);
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.1 **/
static int _cdns_init_fp_bp_storage_parameters(const cdns_ctx_t *ctx, const int idx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair ticks_per_second = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)TICKS_PER_SECOND )),
		.value	= cbor_move(cbor_build_uint64( CLOCKS_PER_SEC ))
	};

	struct cbor_pair max_block_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MAX_BLOCK_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( ctx->options.block_parameters[idx].block_size ))
	};

	cbor_item_t *storage_hints_map = cbor_new_definite_map(STORAGE_HINTS_SIZE);
	_cdns_init_fp_bp_sp_storage_hints(ctx, idx, storage_hints_map);

	struct cbor_pair storage_hints = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)STORAGE_HINTS )),
		.value	= cbor_move( storage_hints_map )
	};

	const size_t opcodes_size = ctx->options.block_parameters[idx].opcodes_size;
	cbor_item_t *opcodes_array = cbor_new_definite_array( opcodes_size );
	for(size_t op_idx = 0; op_idx < opcodes_size; ++op_idx) {
		cbor_array_push(opcodes_array, cbor_move(cbor_build_uint8( ctx->options.block_parameters[idx].opcodes[op_idx] )));
	}

	struct cbor_pair opcodes = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)OPCODES )),
		.value	= cbor_move( opcodes_array )
	};

	const size_t rr_types_size = ctx->options.block_parameters[idx].rr_types_size;	
	cbor_item_t *rr_types_array = cbor_new_definite_array( rr_types_size );
	for(size_t rr_idx = 0; rr_idx < rr_types_size; ++rr_idx) {
		cbor_array_set(rr_types_array, rr_idx, cbor_move(cbor_build_uint16( ctx->options.block_parameters[idx].rr_types[rr_idx] )));
	}

	struct cbor_pair rr_types = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR_TYPES )),
		.value	= cbor_move( rr_types_array )
	};

	//TODO optionals

	cbor_map_add(root, ticks_per_second);
	cbor_map_add(root, max_block_items);
	cbor_map_add(root, storage_hints);
	cbor_map_add(root, opcodes);
	cbor_map_add(root, rr_types);
	//TODO optionals

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.2 **/
static int _cdns_init_fp_bp_collection_parameters(const cdns_ctx_t *ctx, const int idx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	struct cbor_pair query_timeout = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_TIMEOUT )),
		.value	= cbor_move(cbor_build_uint32( 0 ))
	};

	struct cbor_pair promisc = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)PROMISC )),
		.value	= cbor_move(cbor_build_bool( true ))
	};

	cbor_map_add(root, query_timeout);
	cbor_map_add(root, promisc);

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1 **/
static int _cdns_init_fp_block_parameters(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	for(size_t idx = 0; idx < ctx->options.block_parameters_size; ++idx) {
		cbor_item_t *block_parameters_map = cbor_new_definite_map(BLOCK_PARAMETERS_SIZE);

		cbor_item_t *storage_parameters_map = cbor_new_definite_map(STORAGE_PARAMETERS_SIZE);
		_cdns_init_fp_bp_storage_parameters(ctx, idx, storage_parameters_map);

		struct cbor_pair storage_parameters = {
			.key	= cbor_move(cbor_build_uint8( (uint8_t)STORAGE_PARAMETERS )),
			.value	= cbor_move( storage_parameters_map )
		};

		cbor_item_t *collection_parameters_map = cbor_new_definite_map(COLLECTION_PARAMETERS_SIZE);
		_cdns_init_fp_bp_collection_parameters(ctx, idx, collection_parameters_map);
		
		struct cbor_pair collection_parameters = {
			.key	= cbor_move(cbor_build_uint8( (uint8_t)COLLECTION_PARAMETERS )),
			.value	= cbor_move( collection_parameters_map )
		};

		cbor_map_add(block_parameters_map, storage_parameters);
		cbor_map_add(block_parameters_map, collection_parameters);

		cbor_array_push(root, cbor_move(block_parameters_map));
	}

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

	struct cbor_pair private_version = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)PRIVATE_VERSION )),
		.value	= cbor_move(cbor_build_uint8( (uint8_t)VERSION_PRIVATE ))
	};

	cbor_item_t *block_parameters_array = cbor_new_definite_array(ctx->options.block_parameters_size);
	_cdns_init_fp_block_parameters(ctx, block_parameters_array);
	
	struct cbor_pair block_parameters = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_PARAMETERS )),
		.value	= cbor_move( block_parameters_array )
	};

	cbor_map_add(root, major_version);
	cbor_map_add(root, minor_version);
	cbor_map_add(root, private_version);
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

    cbor_item_t *root = cbor_new_definite_array(FILE_SIZE);
    if(!root) {
        return E_ERROR;
    }
    
	cbor_array_set(root, FILE_TYPE_ID, cbor_move(cbor_build_string("C-DNS")));

	cbor_item_t *file_preamble = cbor_new_definite_map( FILE_PREAMBLE_SIZE );
	_cdns_init_file_preamble(ctx, file_preamble);
	cbor_array_set(root, FILE_PREAMBLE, cbor_move(file_preamble));

	cbor_item_t *block_array = cbor_new_indefinite_array(); //TODO better would be definite array
	// Thanks to definite array we will not need function `cdns_serialize_end()`
	// Current solution is here because of `libcbor` feature that counting number of elements in array to keep file format valid even when API is not used correctly
	cbor_array_set(root, FILE_BLOCKS, cbor_move(block_array));

	size_t len = 0;
	if (*buff) {
		len = cbor_serialize(root, *buff, *buff_size);
	} else {
		len = cbor_serialize_alloc(root, buff, buff_size);
	}
	--len; //Remove end of indefinite array (for append in future - read TODO comment above)

	*buff_size = len;

	cbor_decref(&root);

	return E_SUCCESS;
}

static int _cdns_init_bp_earliest_time(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	cbor_array_set(root, EPOCH_TIME, cbor_move(cbor_build_uint64( time(NULL) )));
	cbor_array_set(root, TICKS, cbor_move(cbor_build_uint64( 0UL )));

	return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.1 **/
static int _cdns_init_block_preamble(const cdns_ctx_t *ctx, cbor_item_t *root)
{
	assert(ctx);
	assert(root);

	cbor_item_t *earliest_time_array = cbor_new_definite_array(EARLIEST_TIME_SIZE);
	_cdns_init_bp_earliest_time(ctx, earliest_time_array);

	struct cbor_pair earliest_time = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)EARLIEST_TIME )),
		.value	= cbor_move( earliest_time_array )
	};

	struct cbor_pair block_parameters_index = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)BLOCK_PARAMETERS_INDEX )),
		.value	= cbor_move(cbor_build_uint32( 0 )) //TODO - 0 is default value
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
		.value	= cbor_move(cbor_build_uint32( ctx->storage.stats.processed_messages ))
	};

	struct cbor_pair qr_data_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QR_DATA_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( ctx->storage.stats.qr_data_items ))
	};

	struct cbor_pair unmatched_queries = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)UNMATCHED_QUERIES )),
		.value	= cbor_move(cbor_build_uint32( htobe32(ctx->storage.stats.unmatched_queries) ))
	};

	struct cbor_pair unmatched_responses = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)UNMATCHED_RESPONSES )),
		.value	= cbor_move(cbor_build_uint32( ctx->storage.stats.unmatched_responses ))
	};

	struct cbor_pair discarded_opcode = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)DISCARDED_OPCODE )),
		.value	= cbor_move(cbor_build_uint32( ctx->storage.stats.discarded_opcode ))
	};

	struct cbor_pair malformed_items = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)MALFORMED_ITEMS )),
		.value	= cbor_move(cbor_build_uint32( ctx->storage.stats.malformed_items ))
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

	cbor_item_t *ip_address_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	_cdns_init_bt_ip_address(ctx, ip_address_array);

	struct cbor_pair ip_address = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)IP_ADDRESS )),
		.value	= cbor_move( ip_address_array )
	};

	cbor_item_t *classtype_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair classtype = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)CLASSTYPE )),
		.value	= cbor_move( classtype_array )
	};

	cbor_item_t *name_rdata_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair name_rdata = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)NAME_RDATA )),
		.value	= cbor_move( name_rdata_array )
	};

	cbor_item_t *qr_siq_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair qr_sig = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QR_SIG )),
		.value	= cbor_move( qr_siq_array )
	};

	cbor_item_t *qlist_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair qlist = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QLIST )),
		.value	= cbor_move( qlist_array )
	};

	cbor_item_t *qrr_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair qrr = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QRR )),
		.value	= cbor_move( qrr_array )
	};

	cbor_item_t *rrlist_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair rrlist = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RRLIST )),
		.value	= cbor_move( rrlist_array )
	};

	cbor_item_t *rr_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair rr = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)RR )),
		.value	= cbor_move( rr_array )
	};

	cbor_item_t *malformed_message_data_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
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

	cbor_item_t *query_responses_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair query_responses = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)QUERY_RESPONSES )),
		.value	= cbor_move( query_responses_array )
	};

	cbor_item_t *address_event_counts_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
	//TODO init

	struct cbor_pair address_event_counts = {
		.key	= cbor_move(cbor_build_uint8( (uint8_t)ADDRESS_EVENT_COUNTS )),
		.value	= cbor_move( address_event_counts_array )
	};

	cbor_item_t *malformed_messages_array = cbor_new_definite_array(ctx->options.block_parameters[0].block_size);
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

	//TODO clear all counters
	memset(&(ctx->storage.stats), 0, sizeof(cdns_block_statistics_t));
	
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
