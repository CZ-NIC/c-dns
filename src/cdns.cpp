#include "cdns.h"

int CDNS::cdns_push(CdnsBlock& ctx, const QueryResponse& qr)
{

    //TODO - most important part => add Q/R to block
    // need to be thread-safe

    return E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.1.1 **/
static int _cdns_init_fp_bp_sp_storage_hints(const CDNS::FilePreamble& ctx, const int idx, cbor_item_t* root)
{
    assert(root);

    struct cbor_pair query_response_hints  = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageHintsMapIndex::query_response_hints) )),
        .value	= cbor_move(cbor_build_uint32( ctx.m_block_parameters[idx].storage_parameters.storage_hints.query_response_hints ))
    };

    struct cbor_pair query_response_signature_hints  = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageHintsMapIndex::query_response_signature_hints) )),
        .value	= cbor_move(cbor_build_uint32( ctx.m_block_parameters[idx].storage_parameters.storage_hints.query_response_signature_hints ))
    };

    struct cbor_pair rr_hints  = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageHintsMapIndex::rr_hints) )),
        .value	= cbor_move(cbor_build_uint8( ctx.m_block_parameters[idx].storage_parameters.storage_hints.rr_hints ))
    };

    struct cbor_pair other_data_hints  = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageHintsMapIndex::other_data_hints) )),
        .value	= cbor_move(cbor_build_uint8( ctx.m_block_parameters[idx].storage_parameters.storage_hints.other_data_hints ))
    };

    cbor_map_add(root, query_response_hints);
    cbor_map_add(root, query_response_signature_hints);
    cbor_map_add(root, rr_hints);
    cbor_map_add(root, other_data_hints);

    return 0;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.1 **/
static int _cdns_init_fp_bp_storage_parameters(const CDNS::FilePreamble& ctx, const int idx, cbor_item_t* root)
{
    assert(root);

    struct cbor_pair ticks_per_second = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageParametersMapIndex::ticks_per_second) )),
        .value	= cbor_move(cbor_build_uint64( ctx.m_block_parameters[idx].storage_parameters.ticks_per_second ))
    };

    struct cbor_pair max_block_items = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageParametersMapIndex::max_block_items) )),
        .value	= cbor_move(cbor_build_uint32( ctx.m_block_parameters[idx].storage_parameters.max_block_items ))
    };

    cbor_item_t* storage_hints_map = cbor_new_definite_map(CDNS::get_map_index(CDNS::StorageHintsMapIndex::storage_hints_size));
    _cdns_init_fp_bp_sp_storage_hints(ctx, idx, storage_hints_map);

    struct cbor_pair storage_hints = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageParametersMapIndex::storage_hints) )),
        .value	= cbor_move( storage_hints_map )
    };

    const size_t opcodes_size = ctx.m_block_parameters[idx].storage_parameters.opcodes.size();
    cbor_item_t* opcodes_array = cbor_new_definite_array( opcodes_size );
    for(size_t op_idx = 0; op_idx < opcodes_size; ++op_idx) {
        cbor_array_push(opcodes_array, cbor_move(cbor_build_uint8( static_cast<uint8_t>(ctx.m_block_parameters[idx].storage_parameters.opcodes[op_idx]) )));
    }

    struct cbor_pair opcodes = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageParametersMapIndex::opcodes) )),
        .value	= cbor_move( opcodes_array )
    };

    const size_t rr_types_size = ctx.m_block_parameters[idx].storage_parameters.rr_types.size();
    cbor_item_t* rr_types_array = cbor_new_definite_array( rr_types_size );
    for(size_t rr_idx = 0; rr_idx < rr_types_size; ++rr_idx) {
        cbor_array_push(rr_types_array, cbor_move(cbor_build_uint16( static_cast<uint16_t>(ctx.m_block_parameters[idx].storage_parameters.rr_types[rr_idx]) )));
    }

    struct cbor_pair rr_types = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::StorageParametersMapIndex::rr_types) )),
        .value	= cbor_move( rr_types_array )
    };

    //TODO optionals
    cbor_map_add(root, ticks_per_second);
    cbor_map_add(root, max_block_items);
    cbor_map_add(root, storage_hints);
    cbor_map_add(root, opcodes);
    cbor_map_add(root, rr_types);
    //TODO optionals

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1.2 **/
static int _cdns_init_fp_bp_collection_parameters(const CDNS::FilePreamble& ctx, const int idx, cbor_item_t* root)
{
    assert(root);

    struct cbor_pair query_timeout = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::CollectionParametersMapIndex::query_timeout) )),
        .value	= cbor_move(cbor_build_uint32( 0 ))
    };

    struct cbor_pair promisc = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::CollectionParametersMapIndex::promisc) )),
        .value	= cbor_move(cbor_build_bool( true ))
    };

    cbor_map_add(root, query_timeout);
    cbor_map_add(root, promisc);

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1.1 **/
static int _cdns_init_fp_block_parameters(const CDNS::FilePreamble& ctx, cbor_item_t* root)
{
    assert(root);

    for(size_t idx = 0; idx < ctx.m_block_parameters.size(); ++idx) {
        cbor_item_t* block_parameters_map = cbor_new_definite_map(CDNS::get_map_index(CDNS::BlockParametersMapIndex::block_parameters_size));

        cbor_item_t* storage_parameters_map = cbor_new_definite_map(CDNS::get_map_index(CDNS::StorageParametersMapIndex::storage_parameters_size));
        _cdns_init_fp_bp_storage_parameters(ctx, idx, storage_parameters_map);

        struct cbor_pair storage_parameters = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockParametersMapIndex::storage_parameters) )),
            .value	= cbor_move( storage_parameters_map )
        };

        cbor_map_add(block_parameters_map, storage_parameters);

        if (ctx.m_block_parameters[idx].collection_parameters) {

            cbor_item_t* collection_parameters_map = cbor_new_definite_map(CDNS::get_map_index(CDNS::CollectionParametersMapIndex::collection_parameters_size));
            _cdns_init_fp_bp_collection_parameters(ctx, idx, collection_parameters_map);

            struct cbor_pair collection_parameters = {
                .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockParametersMapIndex::collection_parameters) )),
                .value	= cbor_move( collection_parameters_map )
            };

            cbor_map_add(block_parameters_map, collection_parameters);
        }

        cbor_array_push(root, cbor_move(block_parameters_map));
    }

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.1 **/
static int _cdns_init_file_preamble(const CDNS::FilePreamble& ctx, cbor_item_t* root)
{
    assert(root);

    struct cbor_pair major_version = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::FilePreambleMapIndex::major_format_version) )),
        .value	= cbor_move(cbor_build_uint8( CDNS::VERSION_MAJOR ))
    };

    struct cbor_pair minor_version = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::FilePreambleMapIndex::minor_format_version) )),
        .value	= cbor_move(cbor_build_uint8( CDNS::VERSION_MINOR ))
    };

    struct cbor_pair private_version = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::FilePreambleMapIndex::private_version) )),
        .value	= cbor_move(cbor_build_uint8( CDNS::VERSION_PRIVATE ))
    };

    cbor_item_t *block_parameters_array = cbor_new_definite_array(ctx.m_block_parameters.size());
    _cdns_init_fp_block_parameters(ctx, block_parameters_array);

    struct cbor_pair block_parameters = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::FilePreambleMapIndex::block_parameters) )),
        .value	= cbor_move( block_parameters_array )
    };

    cbor_map_add(root, major_version);
    cbor_map_add(root, minor_version);
    cbor_map_add(root, private_version);
    cbor_map_add(root, block_parameters);

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3 **/
int CDNS::cdns_serialize_file_preamble(const FilePreamble& ctx, unsigned char** buff, size_t* buff_size)
{
    if(buff_size == nullptr) {
        return E_ERROR;
    }

    cbor_item_t* root = cbor_new_definite_array(get_map_index(FileMapIndex::file_size));
    if(!root) {
        return E_ERROR;
    }
    
    cbor_array_set(root, get_map_index(FileMapIndex::file_type_id), cbor_move(cbor_build_string("C-DNS")));

    cbor_item_t* file_preamble = cbor_new_definite_map( get_map_index(FilePreambleMapIndex::file_preamble_size) );
    _cdns_init_file_preamble(ctx, file_preamble);
    cbor_array_set(root, get_map_index(FileMapIndex::file_preamble), cbor_move(file_preamble));

    cbor_item_t* block_array = cbor_new_indefinite_array(); //TODO better would be definite array
    // Thanks to definite array we will not need function `cdns_serialize_end()`
    // Current solution is here because of `libcbor` feature that counting number of elements in array to keep file format valid even when API is not used correctly
    cbor_array_set(root, get_map_index(FileMapIndex::file_block), cbor_move(block_array));

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

static int _cdns_init_bp_earliest_time(const CDNS::CdnsBlock& ctx, cbor_item_t* root)
{
    assert(root);

    cbor_array_set(root, 0, cbor_move(cbor_build_uint64( 0UL )));
    cbor_array_set(root, 1, cbor_move(cbor_build_uint64( 0UL )));

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.1 **/
static int _cdns_init_block_preamble(const CDNS::CdnsBlock& ctx, cbor_item_t* root)
{
    assert(root);

    cbor_item_t* earliest_time_array = cbor_new_definite_array(2);
    _cdns_init_bp_earliest_time(ctx, earliest_time_array);

    struct cbor_pair earliest_time = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockPreambleMapIndex::earliest_time) )),
        .value	= cbor_move( earliest_time_array )
    };

    struct cbor_pair block_parameters_index = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockPreambleMapIndex::block_parameters_index) )),
        .value	= cbor_move(cbor_build_uint32( 0 )) //TODO - 0 is default value
    };

    cbor_map_add(root, earliest_time);
    cbor_map_add(root, block_parameters_index);

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.2 **/
static int _cdns_init_block_statistics(const CDNS::CdnsBlock& ctx, cbor_item_t* root)
{
    assert(root);

    if (ctx.m_block_statistics) {

        struct cbor_pair processed_messages = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::processed_messages) )),
            .value	= cbor_move(cbor_build_uint32( ctx.m_block_statistics.value().processed_messages.value() ))
        };

        struct cbor_pair qr_data_items = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::qr_data_items) )),
            .value	= cbor_move(cbor_build_uint32( ctx.m_block_statistics.value().qr_data_items.value() ))
        };

        struct cbor_pair unmatched_queries = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_queries) )),
            .value	= cbor_move(cbor_build_uint32( htobe32(ctx.m_block_statistics.value().unmatched_queries.value()) ))
        };

        struct cbor_pair unmatched_responses = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::unmatched_responses) )),
            .value	= cbor_move(cbor_build_uint32( ctx.m_block_statistics.value().unmatched_responses.value() ))
        };

        struct cbor_pair discarded_opcode = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::discarded_opcode) )),
            .value	= cbor_move(cbor_build_uint32( ctx.m_block_statistics.value().discarded_opcode.value() ))
        };

        struct cbor_pair malformed_items = {
            .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockStatisticsMapIndex::malformed_items) )),
            .value	= cbor_move(cbor_build_uint32( ctx.m_block_statistics.value().malformed_items.value() ))
        };

        cbor_map_add(root, processed_messages);
        cbor_map_add(root, qr_data_items);
        cbor_map_add(root, unmatched_queries);
        cbor_map_add(root, unmatched_responses);
        cbor_map_add(root, discarded_opcode);
        cbor_map_add(root, malformed_items);
    }

    return CDNS::E_SUCCESS;
}

static int _cdns_init_bt_ip_address(const CDNS::CdnsBlock& ctx, cbor_item_t* root)
{
    assert(root);

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2.3 **/
static int _cdns_init_block_tables(const CDNS::CdnsBlock& ctx, cbor_item_t* root)
{
    assert(root);

    cbor_item_t* ip_address_array = cbor_new_definite_array( 1UL /* TODO */);
    _cdns_init_bt_ip_address(ctx, ip_address_array);

    struct cbor_pair ip_address = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::ip_address) )),
        .value	= cbor_move( ip_address_array )
    };

    cbor_item_t* classtype_array = cbor_new_definite_array( 1UL /* TODO*/);
    //TODO init

    struct cbor_pair classtype = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::classtype) )),
        .value	= cbor_move( classtype_array )
    };

    cbor_item_t* name_rdata_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair name_rdata = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::name_rdata) )),
        .value	= cbor_move( name_rdata_array )
    };

    cbor_item_t* qr_siq_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair qr_sig = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::qr_sig) )),
        .value	= cbor_move( qr_siq_array )
    };

    cbor_item_t* qlist_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair qlist = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::qlist) )),
        .value	= cbor_move( qlist_array )
    };

    cbor_item_t* qrr_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair qrr = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::qrr) )),
        .value	= cbor_move( qrr_array )
    };

    cbor_item_t* rrlist_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair rrlist = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::rrlist) )),
        .value	= cbor_move( rrlist_array )
    };

    cbor_item_t* rr_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair rr = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::rr) )),
        .value	= cbor_move( rr_array )
    };

    cbor_item_t* malformed_message_data_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair malformed_message_data = {
        .key	= cbor_move(cbor_build_uint8( CDNS::get_map_index(CDNS::BlockTablesMapIndex::malformed_message_data) )),
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

    return CDNS::E_SUCCESS;
}

/**	https://tools.ietf.org/html/rfc8618#section-7.3.2 **/
int CDNS::cdns_serialize_block(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size)
{
    if (buff_size == nullptr) {
        return E_ERROR;
    }


    cbor_item_t* root = cbor_new_definite_map( get_map_index(BlockMapIndex::block_size) );

    cbor_item_t* block_preamble_map = cbor_new_definite_map(get_map_index(BlockPreambleMapIndex::block_preamble_size));
    _cdns_init_block_preamble(ctx, block_preamble_map);

    struct cbor_pair block_preamble = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::block_preamble) )),
        .value	= cbor_move( block_preamble_map )
    };

    cbor_item_t* block_statistics_map = cbor_new_definite_map(get_map_index(BlockStatisticsMapIndex::block_statistics_size));
    _cdns_init_block_statistics(ctx, block_statistics_map);

    struct cbor_pair block_statistics = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::block_statistics) )),
        .value	= cbor_move( block_statistics_map )
    };

    cbor_item_t* block_tables_map = cbor_new_definite_map(get_map_index(BlockTablesMapIndex::block_tables_size));
    _cdns_init_block_tables(ctx, block_tables_map);

    struct cbor_pair block_tables = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::block_tables) )),
        .value	= cbor_move( block_tables_map )
    };

    cbor_item_t* query_responses_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair query_responses = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::query_responses) )),
        .value	= cbor_move( query_responses_array )
    };

    cbor_item_t* address_event_counts_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair address_event_counts = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::address_event_counts) )),
        .value	= cbor_move( address_event_counts_array )
    };

    cbor_item_t* malformed_messages_array = cbor_new_definite_array( 1UL /* TODO */);
    //TODO init

    struct cbor_pair malformed_messages = {
        .key	= cbor_move(cbor_build_uint8( get_map_index(BlockMapIndex::malformed_messages) )),
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
    memset(&(ctx.m_block_statistics), 0, sizeof(BlockStatistics));

    return E_SUCCESS;
}

int CDNS::cdns_serialize_end(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size)
{
    if (buff_size == nullptr) {
        return E_ERROR;
    }

    if (*buff == nullptr) {
        *buff = (unsigned char *)calloc(2UL, 1UL);
    }

    (*buff)[0] = 0xFF;
    *buff_size = 1UL;

    return E_SUCCESS;
}
