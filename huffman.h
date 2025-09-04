#ifndef _HUFF_H
#define _HUFF_H

#include "heap.h"

/*
 * Records the frequency of every character in a given filename.
 * frequencies: A zeroed array which will store the frequency of each char.
 * filename: The name of the file to be inspected.
 * return: void.
 */
void getFrequency(int* frequencies, const char *filename);


/*
 * Builds a minheap with the character frequencies
 * freqs: An array storing the frequency of each char.
 * cap: Max number of elements in a tree.
 * return: Minimum node in the heap.
 */
HeapNode* buildTree(int* freqs, int cap);

/*
 * Develop a code for each character based on frequency.
 * root: The root node in the minheap.
 * code: Empty array to hold the code of a given character.
 * codes: Empty array to hold the codes for all characters.
 * len: The length of the code.
 * return: void.
 */
void getCode(HeapNode* root, char* code, char** codes, int len);

/*
 * Writes a bit to a buffer until a byte, which is written to a file.
 * out: The file to write to.
 * bit: The bit to be written.
 * byteBuffer: The buffer to hold the byte.
 * bitCount: the amount of bits written.
 * return: void.
 */
void writeBit(FILE* out, unsigned char bit, unsigned char* byteBuffer, int* bitCount);

/*
 * Flushes the remaining bits, which don't form a byte.
 * out: The file to write to.
 * byteBuffer: The buffer to hold the byte.
 * bitCount: the amount of bits written.
 * return: void.
 */
void flushBits(FILE* out, unsigned char* byteBuffer, int* bitCount);

/*
 * Compress a given file with huffman encoding.
 * input: The input filename.
 * output: The output filename.
 * codes: The array containing the codes for all characters.
 * freqs: The array containing the frequencies for all characters.
 * return: void.
 */
void compressFile(char* input, char* output, char** codes, int* freqs);

/*
 * Decompress a .huff file to reveal its contents.
 * input: The input filename.
 * output: The output filename.
 * cap: Max number of elements to be stored in the minheap.
 * return: void.
 */
void decompress(const char* input, const char* output, int cap);

/* 
 * Free the memory associated with the Huffman codes.
 * code: The memory associated with each individual code.
 * codes: The memory associated with the array containing the codes.
 * return: void.
 */
void deleteCodes(char* code, char** codes);

#endif



