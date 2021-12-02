#include "huffminheap.h"
#include "consts.h"

struct HuffmanMinHeap {
    huff_node_t **nodes;
    size_t size, cap;
};

huff_min_heap_t *huff_min_heap_init(void) {
    huff_min_heap_t *h = malloc(sizeof(huff_min_heap_t));
    h->nodes = malloc(sizeof(huff_node_t *) * ALPHABET_SIZE);
    h->size = 0;
    h->cap = ALPHABET_SIZE;
    return h;
}

void huff_min_heap_destroy(huff_min_heap_t **h) {
    if (h == NULL || *h == NULL) return;
    for (size_t i = 0; i < (*h)->size; i++) {
        huff_node_destroy(&(*h)->nodes[i]);
    }
    free((*h)->nodes);
    free(*h);
}

size_t huff_min_heap_size(huff_min_heap_t *h) {
    return h->size;
}

static void _heapify(huff_min_heap_t *h, size_t i) {
    size_t l = 2 * i + 1, r = 2 * i + 2, smallest = i;
    if (l < h->size && h->nodes[l]->freq < h->nodes[smallest]->freq) {
        smallest = l;
    }
    if (r < h->size && h->nodes[r]->freq < h->nodes[smallest]->freq) {
        smallest = r;
    }
    if (smallest != i) {
        huff_node_t *tmp = h->nodes[i];
        h->nodes[i] = h->nodes[smallest];
        h->nodes[smallest] = tmp;
        _heapify(h, smallest);
    }
}

void huff_min_heap_insert(huff_min_heap_t *h, huff_node_t *node) {
    size_t i = ++h->size - 1;
    while (i > 0 && node->freq < h->nodes[(i - 1) / 2]->freq) {
        h->nodes[i] = h->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h->nodes[i] = node;
}

huff_node_t *huff_min_heap_extract_min(huff_min_heap_t *h) {
    if (h->size == 0) return NULL;
    huff_node_t *min = h->nodes[0];
    h->nodes[0] = h->nodes[h->size - 1];
    h->nodes[h->size - 1] = NULL;
    h->size--;
    _heapify(h, 0);
    return min;
}