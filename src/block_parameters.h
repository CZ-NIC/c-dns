#pragma once

#include "storage_parameters.h"

typedef struct {
    cdns_storage_parameters_t *storage_parameters;
    void *collection_parameters;
} cdns_block_parameters_t;

cdns_block_parameters_t *create_block_parameters(cdns_storage_parameters_t *storage_parameters);

void delete_block_parameters(cdns_block_parameters_t **block_parameters);