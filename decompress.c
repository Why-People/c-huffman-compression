#include "huffdecompress.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    char *inpath = argv[1];
    char *outpath = argv[2];

    huff_decompressed_t decompressed = decompress_file_from_path(inpath, outpath);
    fclose(decompressed.compressed_file);
    fclose(decompressed.decompressed_file);

    printf("Compressed file size: %zu\n", decompressed.compressed_file_size);
    printf("Decompressed file size: %zu\n", decompressed.decompressed_file_size);
    printf("Compression ratio: %f\n", (float) decompressed.decompressed_file_size / (float) decompressed.compressed_file_size);
    return 0;
}