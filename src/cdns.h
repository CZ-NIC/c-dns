#pragma once

#define _DEFAULT_SOURCE

#include <cbor.h>
#include <time.h>
#include <endian.h>
#include <string.h>
#include <sys/socket.h>

#include "format_specification.h"

#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_PRIVATE 0

// Block Statistics
typedef struct {
    unsigned int processed_messages;
    unsigned int qr_data_items;
    unsigned int unmatched_queries;
    unsigned int unmatched_responses;
    unsigned int discarded_opcode;
    unsigned int malformed_items;
} cdns_block_statistics_t;

// Block Tables
typedef struct {
    uint16_t rrtype;
    uint16_t rrclass;
} cdns_classtype_t;

typedef struct {
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
typedef struct {
    uint64_t time_offset;
    struct sockaddr_storage *client_address_p; //ORIGIN 'unsigned int client_address_index;'
    uint16_t client_port;
    uint64_t transaction_id; //TODO restrict byte-size (maybe)
    cdns_query_response_signature_t *qr_signature; //ORIGIN 'unsigned int qr_signature_index;'
    uint8_t client_hoplimit; //TODO change byte-size (maybe)
    int64_t response_delay;
    const char * query_name_p; //ORIGIN 'unsigned int query_name_index;'
    uint16_t query_size;
    uint16_t response_size;
    //TODO
    //response-processing-data;
    //query-extended;
    //response-extended;
} cdns_query_response_t;

typedef struct {
    uint8_t ae_type;
    uint8_t ae_code;
    uint8_t ae_transport_flags;
    unsigned int ae_address_index;
    unsigned int ae_count;
} cdns_address_event_count_t;

typedef struct {
    uint64_t time_offset;
    unsigned int client_address_index;
    uint16_t client_port;
    unsigned int message_data_index;
} cdns_malformed_message_t;

typedef struct {
    //opts
    unsigned int block_size;

    //ctx
    cdns_block_statistics_t stats;
    cdns_query_response_t *queries_responses; //TODO make it array
} cdns_ctx_t;

typedef enum { //TODO
    E_SUCCESS               = 0,
    E_ERROR                 = -1
} cdns_ret_t;


int cdns_init(cdns_ctx_t *ctx, unsigned int block_size);

int cdns_push(cdns_ctx_t *ctx, cdns_query_response_t *qr);

int cdns_serialize_file_preamble(const cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);

int cdns_serialize_block(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);

int cdns_serialize_end(cdns_ctx_t *ctx, unsigned char **buff, size_t *buff_size);
