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

int block_table_init(block_table_t *bt, size_t capacity)
{
    if (!bt)
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

int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index)
{
    if (!bt || !item || !index)
        return B_ERROR;

    uint32_t hash = block_table_calculate_hash(bt, (const char*)item, size);
    block_table_item_t *tmp = bt->table + hash;
    block_table_item_t *bucket_end = NULL;

    // check if this item isn't already in block table
    if (tmp->value) {
        if ((tmp->value_size == size) && (memcmp(item, tmp->value, size) == 0)) {
            *index = tmp->index;
            return B_DUPLICATE;
        }

        bucket_end = tmp;
        tmp = tmp->next_bucket;
        while (tmp) {
            if ((tmp->value_size == size) && (memcmp(item, tmp->value, size) == 0)) {
                *index = tmp->index;
                return B_DUPLICATE;
            }

            bucket_end = tmp;
            tmp = tmp->next_bucket;
        }
    }

    // allocate new block table item if new value isn't first in the bucket
    if (!tmp) {
        tmp = (block_table_item_t*)calloc(1, sizeof(block_table_item_t));
        if (!tmp)
            return B_ERROR;
    }

    // set new block table item
    tmp->index = bt->size;
    tmp->value_size = size;
    tmp->value = item;
    tmp->prev = bt->newest;
    tmp->next = NULL;
    tmp->next_bucket = NULL;
    tmp->prev_bucket = bucket_end;
    if (bucket_end)
        bucket_end->next_bucket = tmp;

    // update block table's parameters
    if (!bt->oldest)
        bt->oldest = tmp;
    if (bt->newest)
        bt->newest->next = tmp;
    bt->newest = tmp;
    bt->size++;

    // set item's index for return
    *index = tmp->index;

    return B_SUCCESS;
}

void block_table_destroy(block_table_t *bt)
{
    if (!bt)
        return;

    block_table_item_t *tmp, *item = bt->oldest;
    while (item) {
        free(item->value);
        tmp = item;
        item = item->next;
        if (tmp->prev_bucket) // item isn't first in bucket and was allocated separately from hash table
            free(tmp);
    }

    bt->capacity = 0U;
    bt->size = 0U;
    bt->oldest = NULL;
    bt->newest = NULL;
    free(bt->table);
}