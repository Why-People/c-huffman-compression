#include "utils.h"
#include "bv.h"
#include "huffminheap.h"
#include "huffstack.h"
#include <stdio.h>

static huff_min_heap_t *_build_min_heap_from_histogram(size_t histogram[ALPHABET_SIZE]) {
    huff_min_heap_t *heap = huff_min_heap_init();
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if(histogram[i] <= 0) continue;
        huff_node_t *node = huff_node_init((char) i, histogram[i]);
        huff_min_heap_insert(heap, node);
    }
    return heap;
}

huff_node_t *build_huffman_tree_from_histogram(size_t histogram[ALPHABET_SIZE]) {
    huff_min_heap_t *heap = _build_min_heap_from_histogram(histogram);

    while(huff_min_heap_size(heap) > 1) {
        huff_node_t *left = huff_min_heap_extract_min(heap);
        huff_node_t *right = huff_min_heap_extract_min(heap);
        huff_node_t *parent = huff_node_join(left, right);
        huff_min_heap_insert(heap, parent);
    }

    huff_node_t *root = huff_min_heap_extract_min(heap);
    huff_min_heap_destroy(&heap);
    return root;
}

huff_node_t *build_huffman_tree_from_tree_dump(size_t tree_size, const char *tree_dump) {
    huff_stack_t *stack = huff_stack_init();

    for(int i = 0; i < tree_size; i++) {
        if(tree_dump[i] == 'L') {
            huff_node_t *node = huff_node_init(tree_dump[++i], 0);
            huff_stack_push(stack, node);
        } else if(tree_dump[i] == 'I') {
            huff_node_t *right = huff_stack_pop(stack);
            huff_node_t *left = huff_stack_pop(stack);

            // TODO: Handle this more gracefully in the context of a decompression status field
            if(left == NULL || right == NULL) {
                fprintf(stderr, "Invalid tree dump\n");
                exit(1);
            }

            huff_node_t *parent = huff_node_join(left, right);
            huff_stack_push(stack, parent);
        }
    }

    huff_node_t *root = huff_stack_pop(stack);
    huff_stack_destroy(&stack);
    return root;
}

static void _build_code_table_helper(huff_node_t *root, huff_code_t *code, huff_code_t **code_table) {
    if(huff_node_is_leaf(root)) {
        code_table[(int) root->symbol] = huff_code_copy(code);
        return;
    }

    huff_code_push_bit(code, 0);
    _build_code_table_helper(root->left, code, code_table);
    huff_code_pop_bit(code);

    huff_code_push_bit(code, 1);
    _build_code_table_helper(root->right, code, code_table);
    huff_code_pop_bit(code);
}

huff_code_t **build_huffman_code_table_from_tree(huff_node_t *root) {
    huff_code_t **huffman_code_table = calloc(ALPHABET_SIZE, sizeof(huff_code_t *));
    huff_code_t *code = huff_code_init();
    _build_code_table_helper(root, code, huffman_code_table);
    huff_code_destroy(&code);
    return huffman_code_table;
}