#ifndef __HUFFCODE_H__
#define __HUFFCODE_H__

#include <stdlib.h>

typedef struct HuffmanCode huff_code_t;

huff_code_t *hufff_code_init();
void huff_code_destroy(huff_code_t **code);

size_t huff_code_len(huff_code_t *code);

void huff_code_push_bit(huff_code_t *code, int bit);
int huff_code_pop_bit(huff_code_t *code);

#endif