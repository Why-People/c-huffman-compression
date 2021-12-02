#include "bv.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct BitVector {
    uint8_t *data;
    size_t cap;
};

bv_t *bv_init(size_t cap) {
    bv_t *bv = malloc(sizeof(bv_t));
    bv->data = calloc(cap, sizeof(uint8_t));
    bv->cap = cap;
    return bv;
}

void bv_destroy(bv_t **bv) {
    if(bv == NULL || *bv == NULL) return;
    free((*bv)->data);
    free(*bv);
    *bv = NULL;
}

bv_t *bv_from_raw_str_data(const char *data, size_t len) {
    bv_t *bv = bv_init(len);
    memcpy(bv->data, data, len);
    return bv;
}

size_t bv_cap(bv_t *bv) {
    return bv->cap;
}

void bv_set_bit(bv_t *bv, int index) {
    bv->data[index / BIT_SIZE] |= 1 << (index % BIT_SIZE);
}

void bv_clr_bit(bv_t *bv, int index) {
    bv->data[index / BIT_SIZE] &= ~(1 << (index % BIT_SIZE));
}

int bv_get_bit(bv_t *bv, int index) {
    return bv->data[index / BIT_SIZE] & (1 << index % BIT_SIZE) ? 1 : 0;
}

void bv_reset(bv_t *bv) {
    memset(bv->data, 0, bv->cap);
}

void bv_set_bits_from_raw_data(bv_t *bv, void *data, size_t len) {
    memcpy(bv->data, data, len);
}

char *bv_raw_data_as_str(bv_t *bv) {
    char *str = calloc(bv->cap, sizeof(char));
    for (int i = 0; i < bv->cap; i++) {
        str[i] = bv->data[i];
    }
    return str;
}

void bv_log(bv_t *bv) {
    for(int i = 0; i < bv->cap * BIT_SIZE; i++) {
        printf("%d", bv_get_bit(bv, i));
    }
    printf("\n");
}
