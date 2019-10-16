#include <smmintrin.h>
#include <string.h>
#include "block_table.h"

/**
 * @brief Calculate hash for item's key
 * @param bt Pointer to block table
 * @param data Pointer to start of item's key
 * @param size Size of item's key in bytes
 * @return Hash for item's key
 */
static uint32_t block_table_calculate_hash(block_table_t *bt, const char *data, size_t size)
{
    const char *end = data + size;
    uint32_t ret = ~0U;

    for ( ; data < end; ) {
        // At least 8 bytes of data left
        if (data + 8 <= end) {
            ret = _mm_crc32_u64(ret, *((const uint64_t*)data));
            data += 8;
        }
        // At least 4 bytes of data left
        else if (data + 4 <= end) {
            ret = _mm_crc32_u32(ret, *((const uint32_t*)data));
            data += 4;
        }
        // At least 2 bytes of data left
        else if (data + 2 <= end) {
            ret = _mm_crc32_u16(ret, *((const uint16_t*)data));
            data += 2;
        }
        // 1 byte of data left
        else {
            ret = _mm_crc32_u8(ret, *((const uint8_t*)data));
            data += 1;
        }
    }

    return (~ret) % bt->capacity;
}

block_table_t* block_table_alloc()
{
    return (block_table_t*)calloc(1, sizeof(block_table_t));
}

int block_table_init(block_table_t *bt, size_t capacity)
{
    if (!bt || capacity == 0)
        return B_ERROR;

    bt->capacity = capacity;
    bt->size = 0U;
    bt->oldest = NULL;
    bt->newest = NULL;
    bt->table = (block_table_item_t*)calloc(capacity, sizeof(block_table_item_t));

    if (!bt->table)
        return B_ERROR;
    else
        return B_SUCCESS;    
}

block_table_t* block_table_create(size_t capacity)
{
    // Allocate new block table
    block_table_t* bt = block_table_alloc();
    if (!bt)
        return NULL;

    // Initialize new block table for 'capacity' items
    int ret = block_table_init(bt, capacity);
    if (ret != B_SUCCESS) {
        free(bt);
        return NULL;
    }

    return bt;
}

bool block_table_find(block_table_t *bt, void *item, size_t size, block_table_item_t **hint)
{
    if (!bt || !item)
        return false;

    // calculate item's hash
    uint32_t hash = block_table_calculate_hash(bt, (const char*)item, size);
    block_table_item_t *tmp = bt->table + hash;
    block_table_item_t *bucket_end = NULL;

    if (tmp->value) {
        // Check if item is already stored at the beginning of the bucket
        if ((tmp->value_size == size) && (memcmp(item, tmp->value, size) == 0)) {
            *hint = tmp;
            return true;
        }

        bucket_end = tmp;
        tmp = tmp->next_bucket;
        while(tmp) {
            // Check if item is already stored somewhere inside the bucket
            if ((tmp->value_size == size) && (memcmp(item, tmp->value, size) == 0)) {
                *hint = tmp;
                return true;
            }

            bucket_end = tmp;
            tmp = tmp->next_bucket;
        }

        *hint = bucket_end;
        return false;
    }

    *hint = tmp;
    return false;
}

int block_table_insert_hint(block_table_t *bt, void *item, size_t size, size_t *index, block_table_item_t *hint)
{
    if (!bt || !item || !index || !hint)
        return B_ERROR;

    if (bt->size == bt->capacity)
        return B_ERROR;

    // if hint points to the last item in non-empty bucket, allocate new item at the end of the bucket
    block_table_item_t *tmp = hint;
    if (hint->value) {
        hint = (block_table_item_t*)calloc(1, sizeof(block_table_item_t));
        if (!hint)
            return B_ERROR;
    }

    // set new block table item
    hint->next_bucket = NULL;
    hint->prev_bucket = tmp->value ? tmp : NULL;
    if (tmp->value)
        tmp->next_bucket = hint;
    hint->index = bt->size;
    hint->value_size = size;
    hint->value = item;
    hint->prev = bt->newest;
    hint->next = NULL;

    // update block table's parameters
    if (!bt->oldest)
        bt->oldest = hint;
    if (bt->newest)
        bt->newest->next = hint;
    bt->newest = hint;
    bt->size++;

    // set item's index for return
    *index = hint->index;

    return B_SUCCESS;
}

int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index)
{
    if (!bt || !item || !index)
        return B_ERROR;

    block_table_item_t *hint = NULL;

    // try to find if the item isn't already in block table
    bool found = block_table_find(bt, item, size, &hint);
    if (found)
        return B_DUPLICATE;

    if (!hint)
        return B_ERROR;

    return block_table_insert_hint(bt, item, size, index, hint);
}

void block_table_discard(block_table_t *bt)
{
    if (!bt)
        return;

    // free all values currently stored in block table's items
    block_table_item_t *tmp, *item = bt->oldest;
    while (item) {
        if (item->value)
            free(item->value);
        tmp = item;
        item = item->next;
        if (tmp->prev_bucket) // item isn't first in bucket and was allocated separately from hash table
            free(tmp);
    }

    // free block table's allocated items
    bt->capacity = 0U;
    bt->size = 0U;
    bt->oldest = NULL;
    bt->newest = NULL;
    if (bt->table)
        free(bt->table);
}

void block_table_dealloc(block_table_t *bt)
{
    if (bt)
        free(bt);
}

void block_table_destroy(block_table_t *bt)
{
    if (!bt)
        return;

    block_table_discard(bt);
    block_table_dealloc(bt);
}