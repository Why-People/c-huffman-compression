#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdlib.h>
#include <stdint.h>

typedef struct Header {
    uint32_t magic;
    uint16_t tree_size;
    size_t original_file_size;
} huff_header_t;

#endif