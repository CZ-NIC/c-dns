#pragma once

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
} cdns_blockparameters_key_t;

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
    QUERY_TIMEOUT       = 0,
    SKEW_TIMEOUT        = 1,
    SNAPLEN             = 2,
    PROMISC             = 3,
    INTERFACES          = 4,
    SERVER_ADDRESSES    = 5,
    VLAN_IDS            = 6,
    FILTER              = 7,
    GENERATOR_ID        = 8,
    HOST_ID             = 9,

    COLLECTION_PARAMETERS_SIZE
} cdns_collectionparameters_key_t;

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

typedef enum {
    NAME_INDEX      = 0,
    CLASSTYPE_INDEX = 1
} cdns_question_key_t;

typedef enum {
    //cdns_question_key_t
    TTL         = 2,
    RDATA_INDEX = 3
} cdns_rr_key_t;

typedef enum {
    //cdns_question_key_t
    MM_TRANSPORT_FLAGS  = 2,
    MM_PAYLOAD          = 3
} cdns_malformedmessagedata_key_t;

typedef enum {
    TIME_OFFSET                 = 0,
    CLIENT_ADDRESS_INDEX        = 1,
    CLIENT_PORT                 = 2,
    TRANSACTION_ID              = 3,
    QR_SIGNATURE_INDEX          = 4,
    CLIENT_HOPLIMIT             = 5,
    RESPONSE_DELAY              = 6,
    QUERY_NAME_INDEX            = 7,
    QUERY_SIZE                  = 8,
    RESPONSE_SIZE               = 9,
    RESPONSE_PROCESSING_DATA    = 10,
    QUERY_EXTENDED              = 11,
    RESPONSE_EXTENDED           = 12,

    QUERY_RESPONSE_SIZE
} cdns_queryresponse_key_t;

typedef enum {
    BAILIWICK_INDEX     = 0,
    PROCESSING_FLAGS    = 1,

    RESPONSE_PROCESSING_DATA_SIZE
} cdns_responseprocessingdata_key_t;

typedef enum {
    QUESTION_INDEX      = 0,
    ANSWER_INDEX        = 1,
    AUTHORITY_INDEX     = 2,
    ADDITIONAL_INDEX    = 3,

    QUESTION_RESPONSE_EXTENDED_SIZE
} cdns_questionresponseextended_key_t;

typedef enum {
    AE_TYPE             = 0,
    AE_CODE             = 1,
    AE_ADDRESS_INDEX    = 2,
    AE_TRANSPORT_FLAGS  = 3,
    AE_COUNT            = 4,

   ADDRESS_EVENT_COUNT_SIZE
} cdns_addresseventcount_key_t;

typedef enum {
    //cdns_question_key_t
    MESSAGE_DATA_INDEX  = 3
} cdns_malformedmessage_key_t;
