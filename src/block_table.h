#pragma once

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Return codes for block table functions
 */
typedef enum {
    B_SUCCESS = 0,
    B_DUPLICATE, // the same item is already in block table
    B_ERROR = -1
} block_table_ret_t;

/**
 * @brief Block table item storing pointer to value and index stating position of item in the block table
 */
typedef struct block_table_item {
    size_t index;
    size_t value_size;
    void *value;
    struct block_table_item *next;
    struct block_table_item *prev;
    struct block_table_item *next_bucket;
    struct block_table_item *prev_bucket;
} block_table_item_t;

/**
 * @brief Block table implemented as hash table with stored items connected by doubly-linked list
 */
typedef struct {
    size_t capacity; // maximum number of items in hash table
    size_t size; // current number of items in hash table
    block_table_item_t *oldest;
    block_table_item_t *newest;
    block_table_item_t *table;
} block_table_t;

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
 * @return block_table_ret_t error code, B_SUCCESS on successful insertion
 */
int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index);

/**
 * @brief Free all memory allocated by block table and free all values in stored items
 * @param bt Block table to free
 */
void block_table_destroy(block_table_t *bt);
