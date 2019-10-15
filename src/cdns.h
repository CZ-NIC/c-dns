#pragma once

#define _DEFAULT_SOURCE

#include <cbor.h>
#include <time.h>
#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "format_specification.h"
#include "block_parameters.h"
#include "dns.h"

#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_PRIVATE 0

/* Hints */

typedef enum
{
    TIME_OFFSET_H                   = 1 << 0,
    CLIENT_ADDRESS_INDEX_H          = 1 << 1,
    CLIENT_PORT_H                   = 1 << 2,
    TRANSACTION_ID_H                = 1 << 3,
    QR_SIGNATURE_INDEX_H            = 1 << 4,
    CLIENT_HOPLIMIT_H               = 1 << 5,
    RESPONSE_DELAY_H                = 1 << 6,
    QUERY_NAME_INDEX_H              = 1 << 7,
    QUERY_SIZE_H                    = 1 << 8,
    RESPONSE_SIZE_H                 = 1 << 9,
    RESPONSE_PROCESSING_DATA_H      = 1 << 10,
    QUERY_QUESTION_SECTIONS_H       = 1 << 11,
    QUERY_ANSWER_SECTIONS_H         = 1 << 12,
    QUERY_AUTHORITY_SECTIONS_H      = 1 << 13,
    QUERY_ADDITIONAL_SECTIONS_H     = 1 << 14,
    RESPONSE_ANSWER_SECTIONS_H      = 1 << 15,
    RESPONSE_AUTHORITY_SECTIONS_H   = 1 << 16,
    RESPONSE_ADDITIONAL_SECTIONS_H  = 1 << 17
} cdns_queryresponse_hints_t;

typedef enum
{
    SERVER_ADDRESS_INDEX_H  = 1 << 0,
    SERVER_PORT_H           = 1 << 1,
    QR_TRANSPORT_FLAGS_H    = 1 << 2,
    QR_TYPE_H               = 1 << 3,
    QR_SIG_FLAGS_H          = 1 << 4,
    QUERY_OPCODE_H          = 1 << 5,
    QR_DNS_FLAGS_H          = 1 << 6,
    QUERY_RCODE_H           = 1 << 7,
    QUERY_CLASSTYPE_INDEX_H = 1 << 8,
    QUERY_QDCOUNT_H         = 1 << 9,
    QUERY_ANCOUNT_H         = 1 << 10,
    QUERY_NSCOUNT_H         = 1 << 11,
    QUERY_ARCOUNT_H         = 1 << 12,
    QUERY_EDNS_VERSION_H    = 1 << 13,
    QUERY_UDP_SIZE_H        = 1 << 14,
    QUERY_OPT_RDATA_INDEX_H = 1 << 15,
    RESPONSE_RCODE_H        = 1 << 16
} cdns_queryresponsesignature_hints_t;

typedef enum
{
    TTL_H           = 1 << 0,
    RDATA_INDEX_H   = 1 << 1
} cdns_rr_hints_t;

typedef enum
{
    MALFORMED_MESSAGES_H    = 1 << 0,
    ADDRESS_EVENT_COUNTS_H  = 1 << 1
} cdns_otherdata_hints_t;

/* Block Preambule */

typedef struct
{
    unsigned int processed_messages;
    unsigned int qr_data_items;
    unsigned int unmatched_queries;
    unsigned int unmatched_responses;
    unsigned int discarded_opcode;
    unsigned int malformed_items;
} cdns_block_statistics_t;

typedef struct
{
    uint16_t rrtype;
    uint16_t rrclass;
} cdns_classtype_t;

typedef struct
{
    struct sockaddr_storage *server_address_p; //ORIGIN 'unsigned int server_address_index;'
    uint16_t server_port;
    uint16_t qr_transport_flags;
    uint8_t qr_type; //TODO not sure about the 8-bit size
    uint8_t qr_sig_flags;
    uint8_t query_opcode; //TODO trim to 4 bits???
    uint16_t qr_dns_flags;
    uint16_t query_rcode;
    cdns_classtype_t *query_classtype_p; //ORIGIN 'unsigned int query_classtype_index;'
    uint16_t query_qdcount;
    uint16_t query_ancount;
    uint16_t query_nscount;
    uint16_t query_arcount;
    uint8_t query_ends_version; 
    uint16_t query_udp_size; // 64KiB ought to be enough for anybody (does it?)
    unsigned int query_opt_rdata_index;
    uint16_t response_rcode;
} cdns_query_response_signature_t;

// Block
typedef struct
{
    unsigned int bailiwick_index;
    unsigned int processing_flags;
} cdns_response_processing_data_t;

typedef struct
{
    unsigned int question_index;
    unsigned int answer_index;
    unsigned int authority_index;
    unsigned int additional_index;
} cdns_query_response_extended_t;

typedef struct
{
    uint64_t time_offset;
    struct sockaddr_storage *client_address_p; //ORIGIN 'unsigned int client_address_index;'
    uint16_t client_port;
    uint16_t transaction_id; //TODO should be 16-bit but I'm not sure
    cdns_query_response_signature_t *qr_signature; //ORIGIN 'unsigned int qr_signature_index;'
    uint8_t client_hoplimit;
    int64_t response_delay;
    uint8_t *query_name_p; //ORIGIN 'unsigned int query_name_index;'
    uint16_t query_size;
    uint16_t response_size;
    cdns_response_processing_data_t *response_processing_data_p; //TODO
    cdns_query_response_extended_t *query_extended_p; //TODO
    cdns_query_response_extended_t *response_extended_p; //TODO
} cdns_query_response_t;

typedef struct
{
    uint8_t ae_type;
    uint8_t ae_code;
    uint8_t ae_transport_flags;
    unsigned int ae_address_index;
    unsigned int ae_count;
} cdns_address_event_count_t;

typedef struct
{
    uint64_t time_offset;
    unsigned int client_address_index;
    uint16_t client_port;
    unsigned int message_data_index;
} cdns_malformed_message_t;

typedef struct
{
    cdns_block_parameters_t *block_parameters_array;
    size_t block_parameters_array_size;
} cdns_options_t;

typedef struct
{
    cdns_block_statistics_t stats;
    cdns_query_response_t *queries_responses; //Array
} cdns_storage_t;

typedef struct
{
    cdns_options_t options;
    cdns_storage_t storage;
} cdns_ctx_t;

/* Other */

typedef enum
{
    E_SUCCESS = 0,
    E_ERROR   = -1
} cdns_ret_t;

/* Functions */

int cdns_init(cdns_ctx_t *ctx,
        cdns_block_parameters_t *block_parameters_array,
		size_t block_parameters_array_size
    );

int cdns_deinit(cdns_ctx_t *ctx);

int cdns_push(cdns_ctx_t *ctx, const cdns_query_response_t *qr);

int cdns_serialize_file_preamble(const cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);

int cdns_serialize_block(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);

int cdns_serialize_end(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);
