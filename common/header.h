#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdlib.h>
#include <stdint.h>

typedef struct Header {
    uint32_t magic;
    size_t tree_size;
    size_t file_size;
} huff_header;

#endif