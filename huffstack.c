#include "huffstack.h"
#include "consts.h"

struct HuffmanStack {
    huff_node_t **nodes;
    size_t top;
};

huff_stack_t *huff_stack_init(void) {
    huff_stack_t *stack = malloc(sizeof(huff_stack_t));
    stack->nodes = calloc(ALPHABET_SIZE, sizeof(huff_node_t *));
    stack->top = 0;
    return stack;
}

void huff_stack_destroy(huff_stack_t **stack) {
    if (stack == NULL || *stack == NULL) return;
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        if ((*stack)->nodes[i] != NULL) {
            huff_node_destroy(&((*stack)->nodes[i]));
        }
    }
    free((*stack)->nodes);
    free(*stack);
    *stack = NULL;
}

void huff_stack_push(huff_stack_t *stack, huff_node_t *node) {
    if(stack->top == ALPHABET_SIZE) return;
    stack->nodes[stack->top++] = node;
}

huff_node_t *huff_stack_pop(huff_stack_t *stack) {
    if(stack->top == 0) return NULL;
    return stack->nodes[--stack->top];
}