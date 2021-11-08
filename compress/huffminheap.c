#include "huffminheap.h"
#include "common/consts.h"

struct HuffmanMinHeap {
    huff_node_t **nodes;
    size_t size, cap;
};

huff_min_heap_t *huff_minheap_init(void) {
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
    if (l < h->size && h->nodes[l]->freq < h->nodes[i]->freq) {
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
    if (h->size == h->cap) {
        h->cap *= 2;
        h->nodes = realloc(h->nodes, sizeof(huff_node_t *) * h->cap);
    }
    h->nodes[h->size] = node;
    size_t i = h->size;
    while (i > 0 && h->nodes[i]->freq < h->nodes[(i - 1) / 2]->freq) {
        huff_node_t *tmp = h->nodes[i];
        h->nodes[i] = h->nodes[(i - 1) / 2];
        h->nodes[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
    h->size++;
}

huff_node_t *huff_min_heap_extract_min(huff_min_heap_t *h) {
    if (h->size == 0) return NULL;
    huff_node_t *min = h->nodes[0];
    h->nodes[0] = h->nodes[h->size - 1];
    h->size--;
    _heapify(h, 0);
    return min;
}