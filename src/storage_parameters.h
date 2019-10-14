#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint32_t query_response;
    uint32_t query_response_signature_hints;
    uint8_t rr_hints;
    uint8_t other_data_hints;
} cdns_storage_hints_t;

typedef struct {
    uint64_t ticks_per_second;
    uint32_t max_block_items;
    cdns_storage_hints_t storage_hints;
    const uint8_t *opcodes;
    const uint16_t *rr_types;
} cdns_storage_parameters_t;

cdns_storage_parameters_t *create_storage_parameters(
        const uint64_t ticks_per_second,
        const uint32_t max_block_items,
        const uint32_t query_response,
        const uint32_t query_response_signature_hints,
        const uint8_t rr_hints,
        const uint8_t other_data_hints,
        const uint8_t *opcodes,
        const size_t opcodes_size,
        const uint16_t *rr_types,
        const size_t rr_types_size
    );

void delete_storage_parameters(cdns_storage_parameters_t **storage_parameters);