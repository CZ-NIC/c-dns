#include <smmintrin.h>
#include "block_table.h"

uint32_t block_table_calculate_hash(block_table_t *bt, const char *data, size_t size)
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

    return (~ret) & (bt->capacity - 1);
}

int block_table_init(block_table_t *bt, size_t capacity)
{
    bt->capacity = capacity;
    bt->size = 0U;
    bt->oldest = NULL;
    bt->newest = NULL;
    bt->table = (block_table_item_t*)malloc(capacity * sizeof(block_table_item_t));

    if (!bt)
        return B_ERROR;
    else
        return B_SUCCESS;    
}

int block_table_insert(block_table_t *bt, void *item, size_t size, size_t *index)
{
    uint32_t hash = block_table_calculate_hash(bt, (const char*)item, size);
    block_table_item_t *tmp = bt->table + hash;

    if (tmp->item) {
        return B_SUCCESS;
    }

    tmp->index = bt->size;
    tmp->item = item;
    tmp->prev = bt->newest;
    tmp->next = NULL;
    bt->newest->next = tmp;
    bt->newest = tmp;
    bt->size++;
    *index = tmp->index;

    return B_SUCCESS;
}

void block_table_destroy(block_table_t *bt)
{
    block_table_item_t *tmp, *item = bt->oldest;
    while (item) {
        tmp = item->next;
        free(item);
        item = tmp;
    }

    bt->capacity = 0U;
    bt->size = 0U;
    bt->oldest = NULL;
    bt->newest = NULL;
    free(bt->table);
}