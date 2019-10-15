#include "block_parameters.h"

cdns_block_parameters_t *create_block_parameters(cdns_storage_parameters_t *storage_parameters, cdns_collection_parameters_t *collection_parameters)
{
    if (storage_parameters == NULL) {
        return NULL;
    }

    cdns_block_parameters_t *output = (cdns_block_parameters_t *)malloc(sizeof(cdns_block_parameters_t));
    if (output == NULL) {
        return NULL;
    }

    output->storage_parameters = storage_parameters;
    output->collection_parameters = collection_parameters;

    return output;
}

void delete_block_parameters(cdns_block_parameters_t **block_parameters)
{
    if (*block_parameters == NULL) {
        return;
    }

    free(*block_parameters);
    block_parameters = NULL;
}