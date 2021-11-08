#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdio.h>

// Compression interface
FILE *compress_file(FILE *in);
FILE *compress_file_from_path(const char *in_path);

// Decompression interface
FILE *decompress_file(FILE *in);
FILE *decompress_file_from_path(const char *in_path);


#endif