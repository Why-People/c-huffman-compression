#ifndef __HUFFMINHEAP_H__
#define __HUFFMINHEAP_H__

#include "common/huffnode.h"

typedef struct HuffmanMinHeap huff_min_heap_t;

huff_min_heap_t *huff_min_heap_init(); // Capacity is always ALPHABET_SIZE (256)
void huff_min_heap_destroy(huff_min_heap_t **heap);

void huff_min_heap_insert(huff_min_heap_t *heap, huff_node_t *node);
huff_node_t *huff_min_heap_extract_min(huff_min_heap_t *heap);

size_t huff_min_heap_size(huff_min_heap_t *heap);

#endif