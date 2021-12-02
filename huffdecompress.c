#include "huffdecompress.h"
#include "huffnode.h"
#include "header.h"
#include "consts.h"
#include "bv.h"
#include "utils.h"
#include <sys/stat.h>
#include <stdio.h>

huff_decompressed_t decompress_file_from_path(const char *inpath, const char *outpath) {
    FILE *infile = fopen(inpath, "rb");
    FILE *outfile = fopen(outpath, "wb");
    return decompress_file(infile, outfile);
}

huff_decompressed_t decompress_file(FILE *in, FILE *out) {
    // Get the file size for the compressed file
    struct stat infile_statbuf;
    fstat(fileno(in), &infile_statbuf);

    // Set initial values for return data type
    huff_decompressed_t decompressed;
    decompressed.compressed_file = in;
    decompressed.compressed_file_size = infile_statbuf.st_size;
    decompressed.decompressed_file = out;
    decompressed.decompressed_file_size = 0;

    if(in == NULL || out == NULL) {
        decompressed.decompression_status = HUFFMAN_DECOMPRESS_INVALID_ARGUMENT;
        return decompressed;
    }

    // Read the header from the compressed file
    huff_header_t compressed_file_header;
    fread(&compressed_file_header, sizeof(huff_header_t), 1, in);

    // Make sure the magic number is correct
    if(compressed_file_header.magic != MAGIC_NUMBER) {
        decompressed.decompression_status = HUFFMAN_DECOMPRESS_INVALID_ARGUMENT;
        return decompressed;
    }

    // Read in the tree dump and reconstruct the huffman tree
    char tree_dump[compressed_file_header.tree_size];
    fread(tree_dump, sizeof(char), compressed_file_header.tree_size, in);
    huff_node_t *huffman_tree_root = build_huffman_tree_from_tree_dump(compressed_file_header.tree_size, tree_dump);

    char read_buf[BLOCK_SIZE]; // Read buffer
    char write_buf[BLOCK_SIZE]; // Write buffer
    bv_t *bit_buf = bv_init(BLOCK_SIZE); // Bit buffer
    int bit_buf_ptr = -1; // Keep track of the amounf of bits we've read from the bit buffer
    huff_node_t *nav = huffman_tree_root; // Navigation node to find the symbol when we read it's respective code bit by bit from the compressed file
    size_t bytes_read = 0; // Number of bytes read from the compressed file
    size_t symbols_decoded = 0; // Number of symbols decoded

    // Decompress the compressed file by reading the file bit by bit and writing the corresponding symbol to the decompressed file when we reach a
    // leaf node on the huffman tree
    while(symbols_decoded < compressed_file_header.original_file_size) {
        // If we've read all the bits from the bit buffer or haven't read any bits yet, read a new block of bits
        if(bit_buf_ptr == BLOCK_SIZE * BIT_SIZE || bit_buf_ptr == -1) {
            bytes_read = fread(read_buf, sizeof(char), BLOCK_SIZE, in);
            bv_set_bits_from_raw_data(bit_buf, read_buf, bytes_read);
            bit_buf_ptr = 0;
        }

        // Read the current bit from the bit buffer and navigate the huffman tree to find the symbol
        int curr_bit = bv_get_bit(bit_buf, bit_buf_ptr++);
        nav = curr_bit ? nav->right : nav->left;

        // If the node is not a leaf, then the node data is not a symbol, so we need to keep reading bits until we get to a leaf
        if(!huff_node_is_leaf(nav)) {
            continue;
        }

        // If we've reached a leaf node, write the symbol to the write buffer
        write_buf[symbols_decoded % BLOCK_SIZE] = nav->symbol;

        // If we've written a full block of data, write the block to the decompressed file
        if(symbols_decoded % BLOCK_SIZE == BLOCK_SIZE - 1) {
            decompressed.decompressed_file_size += fwrite(write_buf, sizeof(char), BLOCK_SIZE, out);
        }

        nav = huffman_tree_root; // Reset the navigation node to the root
        symbols_decoded++;
    }

    // Free the memory used by the bit buffer
    bv_destroy(&bit_buf);

    // Write any remaining data to the decompressed file
    decompressed.decompressed_file_size += fwrite(write_buf, sizeof(char), symbols_decoded % BLOCK_SIZE, out);
    huff_node_destroy_tree(&huffman_tree_root); // Destroy the huffman tree

    decompressed.decompression_status = HUFFMAN_DECOMPRESS_SUCCESS;
    return decompressed;
}