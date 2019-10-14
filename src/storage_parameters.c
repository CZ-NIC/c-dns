#include "storage_parameters.h"

cdns_storage_parameters_t *create_storage_parameters(
        const uint64_t ticks_per_second,
        const uint32_t max_block_items,
        const uint32_t query_response,
        const uint32_t query_response_signature_hints,
        const uint8_t rr_hints,
        const uint8_t other_data_hints,
        const uint8_t *opcodes,
        const uint16_t *rr_types
    )
{
    if (opcodes == NULL) {
        return NULL;
    }
    if (rr_types == NULL) {
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
    
    //TODO - alloc and fill
    output->opcodes = opcodes;
    //memcpy(output->opcodes, opcodes, sizeof(opcodes));
    
    //TODO - alloc and fill
    output->rr_types = rr_types;
    //memcpy(output->rr_types, rr_types, sizeof(rr_types));

    return output;
}

void delete_storage_parameters(cdns_storage_parameters_t **storage_parameters) {
    //free((*storage_parameters)->opcodes);
    //free((*storage_parameters)->rr_types);
    free(*storage_parameters);
    storage_parameters = NULL;
}