#ifndef __HUFFSTACK_H__
#define __HUFFSTACK_H__

#include "huffnode.h"

typedef struct HuffmanStack huff_stack_t;

huff_stack_t *huff_stack_init();
void huff_stack_destroy(huff_stack_t **stack);

void huff_stack_push(huff_stack_t *stack, huff_node_t *data);
huff_node_t *huff_stack_pop(huff_stack_t *stack);

#endif