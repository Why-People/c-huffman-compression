#ifndef __UTILS_H__
#define __UTILS_H__

#include "consts.h"
#include "huffnode.h"
#include "huffcode.h"
#include <stdlib.h>

huff_node_t *build_huffman_tree_from_histogram(size_t histogram[ALPHABET_SIZE]);
huff_node_t *build_huffman_tree_from_tree_dump(size_t tree_size, const char *tree_dump);

huff_code_t **build_huffman_code_table_from_tree(huff_node_t *root);

#endif