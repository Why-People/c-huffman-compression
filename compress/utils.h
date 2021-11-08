#ifndef __UTILS_H__
#define __UTILS_H__

#include "common/consts.h"
#include "common/huffnode.h"
#include "huffcode.h"
#include <stdlib.h>

huff_node_t *build_huffman_tree_from_histogram(size_t histogram[ALPHABET_SIZE]);

huff_code_t **build_huffman_code_table_from_tree(huff_node_t *root);

#endif