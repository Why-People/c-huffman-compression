#include "bv.h"
#include <stdint.h>

struct BitVector {
    uint8_t *data;
    size_t len, cap;
};

bv_t *bv_init(size_t cap) {
    bv_t *bv = malloc(sizeof(bv_t));
    size_t alloc_size = WORD_SIZE(cap);
    bv->data = calloc(alloc_size, sizeof(uint8_t));
    bv->cap = cap;
    bv->len = 0;
    return bv;
}

void bv_free(bv_t **bv) {
    free((*bv)->data);
    free(*bv);
    *bv = NULL;
}

size_t bv_len(bv_t *bv) {
    return bv->len;
}

size_t bv_cap(bv_t *bv) {
    return bv->cap;
}

void bv_set_bit(bv_t *bv, int index) {
    bv->data[bv->len / BIT_SIZE] |= 1 << (index % BIT_SIZE);
}

void bv_clr_bit(bv_t *bv, int index) {
    bv->data[bv->len / BIT_SIZE] &= ~(1 << (index % BIT_SIZE));
}

int bv_get_bit(bv_t *bv, int index) {
    return bv->data[bv->len / BIT_SIZE] & (1 << (index % BIT_SIZE) ? 1 : 0);
}


