#include "huffnode.h"

#define NODE_JOIN_SYMBOL '$'

#include <stdlib.h>

huff_node_t *huff_node_init(char symbol, int freq) {
    huff_node_t *node = malloc(sizeof(huff_node_t));
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void huff_node_destroy_tree(huff_node_t **root) {
    if ((*root)->left != NULL) {
        huff_node_destroy_tree(&(*root)->left);
    }
    if ((*root)->right != NULL) {
        huff_node_destroy_tree(&(*root)->right);
    }
    free(*root);
}

bool huff_node_is_leaf(huff_node_t *node) {
    return node != NULL && node->left == NULL && node->right == NULL;
}

huff_node_t *huff_node_join(huff_node_t *left, huff_node_t *right) {
    huff_node_t *node = huff_node_init(NODE_JOIN_SYMBOL, left->freq + right->freq);
    node->left = left;
    node->right = right;
    return node;
}