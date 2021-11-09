#include "huffcompress.h"
#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Error: Invalid number of arguments.");
        return 1;
    }

    char *in_file = argv[1];
    char *out_file = argv[2];

    huff_compressed_t compressed = compress_file_from_path(in_file, out_file);
    fclose(compressed.original_file);
    fclose(compressed.compressed_file);

    printf("Original file size: %zu\n", compressed.original_file_size);
    printf("Compressed file size: %zu\n", compressed.compressed_file_size);
    printf("Compression ratio: %f\n", (float) compressed.original_file_size / (float) compressed.compressed_file_size);
    return 0;
}