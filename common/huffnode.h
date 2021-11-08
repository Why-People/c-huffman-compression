#ifndef __HUFFNODE_H__
#define __HUFFNODE_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct HuffmanNode {
    char symbol;
    int freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} huff_node_t;

huff_node_t *huff_node_init(char symbol, int freq);
void huff_node_destroy(huff_node_t **node);
void huff_node_destroy_tree(huff_node_t **root);

bool huff_node_is_leaf(huff_node_t *node);
huff_node_t *huff_node_join(huff_node_t *left, huff_node_t *right);

#endif