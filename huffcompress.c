#include "huffcompress.h"
#include "header.h"
#include "consts.h"
#include "huffnode.h"
#include <string.h>
#include <sys/stat.h>

huff_compressed_t compress_file_from_path(const char* in_path, const char *out_path) {
    FILE *infile = fopen(in_path, "rb");
    FILE *outfile = fopen(out_path, "wb");
    return compress_file(infile, outfile);
}

huff_compressed_t compress_file(FILE *in, FILE *out) {
    // Needed for the compressed file header and output data type
    struct stat infile_statbuf;
    fstat(fileno(in), &infile_statbuf);

    // Set initial values for the return data type
    huff_compressed_t compressed;
    compressed.original_file = in;
    compressed.original_file_size = infile_statbuf.st_size;
    compressed.compressed_file = out;
    compressed.compressed_file_size = 0; // This is incrmented whenever a byte is written to the output file

    // Invalid File pointers and therefore invalid arguments
    if(in == NULL || out == NULL) {
        compressed.compression_status = HUFFMAN_COMPRESS_INVALID_ARGUMENT;
        return compressed;
    }

    int unique_symbols = 0; // Used for calculating the tree size for the header
    char read_buf[BLOCK_SIZE] = { 0 };
    size_t histogram[ALPHABET_SIZE] = { 0 };
    size_t bytes_read; // Used for both file passes

    // First pass through the file, count the character frequencies
    while((bytes_read = fread(read_buf, sizeof(char), BLOCK_SIZE, in)) > 0) {
        for(int i = 0; i < bytes_read; i++) {
            histogram[read_buf[i]]++;
            unique_symbols += (int) (histogram[read_buf[i]] == 1);
        }
    }

    huff_node_t *huffman_tree_root = build_huffman_tree_from_histogram(histogram);

    // Reset the file pointer to the beginning of the file in preparation for the second pass
    fseek(in, 0, SEEK_SET);

    // Build and write the ompressed file header
    huff_header_t header;
    header.magic = MAGIC_NUMBER;
    header.tree_size = (3 * unique_symbols) - 1;
    header.original_file_size = infile_statbuf.st_size;
    fwrite(&header, sizeof(huff_header_t), 1, out);
    compressed.compressed_file_size += sizeof(huff_header_t) + header.tree_size;

    // Write the huffman tree to the output file
    huff_node_write_tree(huffman_tree_root, out);

    // An empty file will yield an empty huffman tree, and also an empty output file minus the header
    if(huffman_tree_root == NULL) {
        compressed.compression_status = HUFFMAN_COMPRESS_SUCCESS;
        return compressed;
    }

    huff_code_t **huffman_code_table = build_huffman_code_table_from_tree(huffman_tree_root);

    // Allows for easy concatenation of huffman codes
    bv_t *code_buffer = bv_init(BLOCK_SIZE);
    size_t bits_appended = 0; // Keeps track of how many bits have been appended to the code buffer

    // Second pass through the file, write the huffman codes to the output file
    while((bytes_read = fread(read_buf, sizeof(char), BLOCK_SIZE, in)) > 0) {
        // Map each byte to its huffman code and append the bits from the code to the code buffer
        // Once the code buffer is full, write its raw data to the output file
        for(int i = 0; i < bytes_read; i++) {
            huff_code_t *code = huffman_code_table[read_buf[i]];
            bv_t *code_vec = huff_code_to_bv(code);
            size_t code_len = huff_code_len(code);

            // Append the bits from the code to the code buffer
            for(int j = 0; j < code_len; j++) {
                int curr_bit = bv_get_bit(code_vec, j);
                if(curr_bit) {
                    bv_set_bit(code_buffer, bits_appended);
                }
                bits_appended++;

                // If we've appended the maximum number of bits that our vector can hold, write the buffer to the output file
                if(bits_appended == BLOCK_SIZE * BIT_SIZE) {
                    // Convert the data in the buffer to a raw string and write it to the output file
                    char *raw_code_buffer_data = bv_raw_data_as_str(code_buffer);
                    compressed.compressed_file_size += fwrite(raw_code_buffer_data, sizeof(char), BLOCK_SIZE, out);
                    free(raw_code_buffer_data); // bv_raw_data_as_str returns a copy of the internal data field of the buffer
                    
                    bv_reset(code_buffer); // Reset the code buffer and start appending bits again from the beginning
                    bits_appended = 0; // Reset this number since we're starting over
                }
            }
        }
    }

    // There may still be bits in the code buffer that haven't been written to the output file, so we need to write them
    
    // First, Pad the code buffer with zeros to the next byte boundary
    while(bits_appended % BIT_SIZE != 0) {
        bits_appended++;
    }

    // Then, convert the data in the buffer to a raw string and write it to the output file
    char *raw_code_buffer_data = bv_raw_data_as_str(code_buffer);
    compressed.compressed_file_size += fwrite(raw_code_buffer_data, sizeof(char), bits_appended / BIT_SIZE, out);
    free(raw_code_buffer_data); // bv_raw_data_as_str returns a copy of the internal data field of the buffer
    bv_destroy(&code_buffer); // Destroy the code buffer

    huff_node_destroy_tree(&huffman_tree_root); // Free the memory used by the huffman tree

    // Free each of the codes in the huffman code table
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        huff_code_destroy(&huffman_code_table[i]);
    }

    free(huffman_code_table); // Free the memory used by the huffman code table

    compressed.compression_status = HUFFMAN_COMPRESS_SUCCESS;
    return compressed;
}