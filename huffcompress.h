#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include "utils.h"
#include "huffcode.h"
#include <stdio.h>

typedef enum HuffmanCompressStatus {
    HUFFMAN_COMPRESS_SUCCESS = 0,
    HUFFMAN_COMPRESS_FAILURE = 1,
    HUFFMAN_COMPRESS_INVALID_ARGUMENT = 2,
    HUFFMAN_COMPRESS_NO_CONTENT_WRITTEN = 3
} huff_compress_status_t;

typedef struct huff_compressed_file {
    FILE *original_file;
    FILE *compressed_file;
    size_t original_file_size;
    size_t compressed_file_size;
    huff_compress_status_t compression_status;
} huff_compressed_t;

huff_compressed_t compress_file(FILE *in, FILE *out);
huff_compressed_t compress_file_from_path(const char *in_path, const char *out_path);

#endif