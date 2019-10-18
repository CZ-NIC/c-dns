#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
 * @brief Allocate new block table
 * @return Block table allocated with calloc
 */
block_table_t* block_table_alloc();

/**
 * @brief Initialize block table for 'capacity' items
 * @param bt Block table to initialize
 * @param capacity Maximum number of items in block table
 * @return B_SUCCESS on successful initialization, B_ERROR otherwise
 */
int block_table_init(block_table_t *bt, size_t capacity);

/**
 * @brief Allocate and initialize new block table
 * @param capacity Maximum number of item's in block table
 * @return New block table initialized to capacity items
 */
block_table_t* block_table_create(size_t capacity);

/**
 * @brief Check if item is already stored in block table and if it isn't, set hint as to where it would be stored
 * @param bt Block table to search in
 * @param item Item to search for
 * @param size Size of the item in bytes
 * @param hint Is set to where in block table the item would be stored if it isn't found.
 * If item wouldn't be first in the bucket, 'hint' points to currently last item in the bucket.
 * Item would be stored as the next in the bucket after 'hint'.
 * @return Returns true if item is found, false otherwise
 */
bool block_table_find(block_table_t *bt, void *item, size_t size, block_table_item_t **hint);

/**
 * @brief Insert item into block table to position 'hint' that was acquired by calling block_table_find()
 * function. If item isn't first in the bucket, 'hint' points to currently last item in the bucket.
 * Item will be stored as the next in the bucket after 'hint'.
 * @param bt Block table to insert to
 * @param item New item to insert into block table. If insertion is successful block tables takes
 * ownership of this item and is responsible for its deallocation
 * @param size Size of the item in bytes
 * @param index Pointer where item's index will be stored if insertion is successful
 * @param hint Hint as to where in the block table item should be stored
 * @return block_table_ret_t error code, B_SUCCESS on successful insertion
 */
int block_table_insert_hint(block_table_t *bt, void *item, size_t size, size_t *index, block_table_item_t *hint);

/**
 * @brief Insert new item into block table
 * @param bt Block table to insert to
 * @param item New item to insert into block table. If insertion is successful block table takes
 * ownership of this item and is responsible for its deallocation
 * @param size Size of the new item in bytes
 * @param index Pointer where item's index will be stored if insertion is successful
 * @return block_table_ret_t error code, B_SUCCESS on successful insertion
 */
int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index);

/**
 * @brief Free all resources managed by given block table
 * @param bt Block table whose resources to discard
 */
void block_table_discard(block_table_t *bt);

/**
 * @brief Free given block table
 * @param bt Block table to free
 */
void block_table_dealloc(block_table_t *bt);

/**
 * @brief Free all resources managed by block table and free the block table
 * @param bt Block table to free
 */
void block_table_destroy(block_table_t *bt);
