#ifndef __BV_H__
#define __BV_H__

#include <stdlib.h>

#define BIT_SIZE 8
#define WORD_SIZE(len) (len / BIT_SIZE + (len % BIT_SIZE ? 1 : 0))

typedef struct BitVector bv_t;

bv_t *bv_init(size_t cap);
void bv_destroy(bv_t **bv);

void bv_set_bit(bv_t *bv, int index);
void bv_clr_bit(bv_t *bv, int index);
int bv_get_bit(bv_t *bv, int index);

size_t bv_len(bv_t *bv);
size_t bv_cap(bv_t *bv);

void bv_reset(bv_t *bv);
void bv_set_bits_from_raw_data(bv_t *bv, void *data, size_t len);
char *bv_raw_data_as_str(bv_t *bv);

void bv_log(bv_t *bv);

#endif