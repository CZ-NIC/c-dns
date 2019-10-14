#include "storage_parameters.h"

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
    )
{
    if (opcodes == NULL || !opcodes_size) {
        return NULL;
    }
    if (rr_types == NULL || !rr_types_size) {
        return NULL;
    }

    cdns_storage_parameters_t *output = (cdns_storage_parameters_t *)malloc(sizeof(cdns_storage_parameters_t));
    if (output == NULL) {
        return NULL;
    }

    output->ticks_per_second = ticks_per_second;
    output->max_block_items = max_block_items;
    output->storage_hints.query_response = query_response;
    output->storage_hints.query_response_signature_hints = query_response_signature_hints;
    output->storage_hints.rr_hints = rr_hints;
    output->storage_hints.other_data_hints = other_data_hints;
    
    output->opcodes = (uint8_t *)calloc(opcodes_size, sizeof(uint8_t));
    if (output->opcodes == NULL) {
        free(output);
        return NULL;
    }
    memcpy(output->opcodes, opcodes, opcodes_size * sizeof(uint8_t));
    
    output->rr_types = (uint16_t *)calloc(rr_types_size, sizeof(uint16_t));
    if (output->rr_types == NULL) {
        free(output->opcodes);
        free(output);
        return NULL;
    }
    memcpy(output->rr_types, rr_types, rr_types_size * sizeof(uint16_t));

    return output;
}

void delete_storage_parameters(cdns_storage_parameters_t **storage_parameters) {
    free((*storage_parameters)->opcodes);
    free((*storage_parameters)->rr_types);
    free(*storage_parameters);
    storage_parameters = NULL;
}