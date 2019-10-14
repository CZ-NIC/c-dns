#pragma once

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Return codes for block table functions
 */
typedef enum {
    B_SUCCESS = 0,
    B_ERROR = -1
} block_table_ret_t;

/**
 * @brief Block table item storing pointer to value and index stating position of item in the block table
 */
typedef struct block_table_item {
    size_t index;
    void *item;
    struct block_table_item *next;
    struct block_table_item *prev;
} block_table_item_t;

/**
 * @brief Block table implemented as hash table with stored items connected by doubly-linked list
 */
typedef struct {
    size_t capacity;
    size_t size;
    block_table_item_t *oldest;
    block_table_item_t *newest;
    block_table_item_t *table;
} block_table_t;

/**
 * @brief Calculate hash for item's key
 * @param bt Pointer to block table
 * @param data Pointer to start of item's key
 * @param size Size of item's key in bytes
 * @return Hash for item's key
 */
uint32_t block_table_calculate_hash(block_table_t *bt, const char *data, size_t size);

/**
 * @brief Initialize block table for 'capacity' items
 * @param bt Block table to initialize
 * @param capacity Maximum number of items in block table
 * @return B_SUCCESS on successful initialization, B_ERROR otherwise
 */
int block_table_init(block_table_t *bt, size_t capacity);

/**
 * @brief Insert new item into block table
 * @param bt Block table to insert to
 * @param item New item to insert to block table
 * @param size Size of the new item in bytes
 * @param index Pointer where item's index will be stored if insertion is successful
 * @return B_SUCCESS on successful insertion, B_ERROR otherwise
 */
int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index);

/**
 * @brief Free all memory allocated by block table and free all values in stored items
 * @param bt Block table to free
 */
void block_table_destroy(block_table_t *bt);
