#ifndef _HUFF_H
#define _HUFF_H

#include "heap.h"

void getFrequency(int* frequencies, const char *filename);

HeapNode* buildTree(int* freqs, int cap);

void getCode(HeapNode* root, char* code, char** codes, int len);

void writeBit(FILE* out, unsigned char bit, unsigned char* byteBuffer, int* bitCount);

void flushBits(FILE* out, unsigned char* byteBuffer, int* bitCount);

void compressFile(char* input, char* output, char** codes, int* freqs);

void decompress(const char* inputFilename, const char* outputFilename, int cap);

#endif



