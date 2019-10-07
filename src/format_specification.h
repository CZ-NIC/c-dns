#pragma once


// C-DNS keys

typedef enum {
    FILE_TYPE_ID = 0,
    FILE_PREAMBLE = 1,
    FILE_BLOCKS = 2,

    FILE_SIZE
} cdns_file_key_t;

typedef enum {
    MAJOR_FORMAT_VERSION    = 0,
    MINOR_FORMAT_VERSION    = 1,
    PRIVATE_VERSION         = 2,
    BLOCK_PARAMETERS        = 3,

    FILE_PREAMBLE_SIZE
} cnds_filepreamble_key_t;

typedef enum {
    STORAGE_PARAMETERS = 0,
    COLLECTION_PARAMETERS = 1,

    BLOCK_PARAMETERS_SIZE
} cdns_blockparameters_ket_t;

typedef enum {
    TICKS_PER_SECOND            = 0,
    MAX_BLOCK_ITEMS             = 1,
    STORAGE_HINTS               = 2,
    OPCODES                     = 3,
    RR_TYPES                    = 4,
    STORAGE_FLAGS               = 5,
    CLIENT_ADDRESS_PREFIX_IPV4  = 6,
    CLIENT_ADDRESS_PREFIX_IPV6  = 7,
    SERVER_ADDRESS_PREFIX_IPV4  = 8,
    SERVER_ADDRESS_PREFIX_IPV6  = 9,
    SAMPLING_METHOD             = 10,
    ANONYMIZATION_METHOD        = 11,

    STORAGE_PARAMETERS_SIZE
} cdns_storageparameters_key_t;

typedef enum {
    QUERY_RESPONSE_HINTS            = 0,
    QUERY_RESPONSE_SIGNATURE_HINTS  = 1,
    RR_HINTS                        = 2,
    OTHER_DATA_HINTS                = 3,

    STORAGE_HINTS_SIZE
} cdns_storagehints_key_t;

typedef enum {
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

typedef enum {
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

typedef enum {
    TTL_H           = 1 << 0,
    RDATA_INDEX_H   = 1 << 1
} cdns_rr_hints_t;

typedef enum {
    MALFORMED_MESSAGES_H    = 1 << 0,
    ADDRESS_EVENT_COUNTS_H  = 1 << 1
} cdns_otherdata_hints_t;

//TODO in header is stored more info, watch RFC-8618, i was just lazy implements it for PoC

typedef enum {
    BLOCK_PREAMBLE          = 0,
    BLOCK_STATISTICS        = 1,
    BLOCK_TABLES            = 2,
    QUERY_RESPONSES         = 3,
    ADDRESS_EVENT_COUNTS    = 4,
    MALFORMED_MESSAGES      = 5,

    BLOCK_SIZE
} cdns_block_key_t;


typedef enum {
    EARLIEST_TIME           = 0,
    BLOCK_PARAMETERS_INDEX  = 1,

    BLOCK_PREAMBLE_SIZE
} cdns_blockpreamble_key_t;

typedef enum {
    EPOCH_TIME  = 0,
    TICKS       = 1,

    EARLIEST_TIME_SIZE
} cdns_earliesttime_key_t;

//TODO end implementation of block header

typedef enum {
    PROCESSED_MESSAGES  = 0,
    QR_DATA_ITEMS       = 1,
    UNMATCHED_QUERIES   = 2,
    UNMATCHED_RESPONSES = 3,
    DISCARDED_OPCODE    = 4,
    MALFORMED_ITEMS     = 5,

    BLOCK_STATISTICS_SIZE
} cdns_blockstatistics_key_t;

typedef enum {
    IP_ADDRESS              = 0,
    CLASSTYPE               = 1,
    NAME_RDATA              = 2,
    QR_SIG                  = 3,
    QLIST                   = 4,
    QRR                     = 5,
    RRLIST                  = 6,
    RR                      = 7,
    MALFORMED_MESSAGE_DATA  = 8,

    BLOCK_TABLES_SIZE
} cdns_blocktables_key_t;

typedef enum {
    TYPE    = 0,
    CLASS   = 1,

    CLASS_TYPE_SIZE
} cdns_classtype_key_t;

typedef enum {
    SERVER_ADDRESS_INDEX    = 0,
    SERVER_PORT             = 1,
    QR_TRANSPORT_FLAGS      = 2,
    QR_TYPE                 = 3,
    QR_SIG_FLAGS            = 4,
    QUERY_OPCODE            = 5,
    QR_DNS_FLAGS            = 6,
    QUERY_RCODE             = 7,
    QUERY_CLASSTYPE_INDEX   = 8,
    QUERY_QDCOUNT           = 9,
    QUERY_ANCOUNT           = 10,
    QUERY_NSCOUNT           = 11,
    QUERY_ARCOUNT           = 12,
    QUERY_EDNS_VERSION      = 13,
    QUERY_UDP_SIZE          = 14,
    QUERY_OPT_RDATA_INDEX   = 15,
    RESPONSE_RCODE          = 16,

    QUERY_RESPONSE_SIGNATURE_SIZE
} cdns_queryresponsesignature_key_t;