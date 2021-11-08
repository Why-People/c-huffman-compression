#include "huffcode.h"
#include "common/bv.h"
#include "common/consts.h"

#define CODE_ALLOC_SIZE (ALPHABET_SIZE / 8)

struct HuffmanCode {
    bv_t *vec;
    size_t len;
};

huff_code_t *huff_code_init() {
    huff_code_t *code = malloc(sizeof(huff_code_t));
    code->vec = bv_init(CODE_ALLOC_SIZE);
    code->len = 0;
    return code;
}

void huff_code_destroy(huff_code_t **code) {
    bv_destroy(&(*code)->vec);
    free(*code);
    *code = NULL;
}

size_t huff_code_len(huff_code_t *code) {
    return code->len;
}

void huff_code_push_bit(huff_code_t *code, int bit) {
    bv_set_bit(code->vec, code->len);
    code->len++;
}

int huff_code_pop_bit(huff_code_t *code) {
    int bit = bv_get_bit(code->vec, code->len - 1);
    code->len--;
    return bit;
}