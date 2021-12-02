#ifndef __HUFFDECOMPRESS_H__
#define __HUFFDECOMPRESS_H__

#include <stdio.h>

typedef enum HuffmanDecompressStatus {
    HUFFMAN_DECOMPRESS_SUCCESS = 0,
    HUFFMAN_DECOMPRESS_FAILURE = 1,
    HUFFMAN_DECOMPRESS_INVALID_ARGUMENT = 2,
    HUFFMAN_DECOMPRESS_NO_CONTENT_WRITTEN = 3
} huff_decompress_status_t;

typedef struct huff_decompressed_file {
    FILE *compressed_file;
    FILE *decompressed_file;
    size_t compressed_file_size;
    size_t decompressed_file_size;
    huff_decompress_status_t decompression_status;
} huff_decompressed_t;

huff_decompressed_t decompress_file(FILE *compressed_file, FILE *decompressed_file);
huff_decompressed_t decompress_file_from_path(const char *compressed_file_path, const char *decompressed_file_path);

#endif