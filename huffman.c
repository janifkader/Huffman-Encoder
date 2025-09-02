#include "huffman.h"

void getFrequency(int* frequencies, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file){
        perror("File error");
        return;
    }
    char c;
    while ((c = fgetc(file)) != EOF){
        if (c >= 0 && c < 256) {
            frequencies[c]++;
        }
    }
    fclose(file);
}

HeapNode* buildTree(int* freqs, int cap){
    MinHeap* heap = newHeap(cap);
    for (int i = 0; i < 256; i++){
        if (*(freqs + i) > 0){
            insert(heap, createNode(*(freqs + i), i));
        }
    }
    while (heap->size > 1) {
        HeapNode *left = extractMin(heap);
        HeapNode *right = extractMin(heap);
        HeapNode *newNode = createNode('\0', left->priority + right->priority);
        newNode->left = left;
        newNode->right = right;
        insert(heap, newNode);
    }
    HeapNode* min = extractMin(heap);
    deleteHeap(heap);
    return min;
}

void getCode(HeapNode* root, char* code, char** codes, int len){
    if (root == NULL){
        return;
    }
    if (root->left == NULL && root->right == NULL){
        code[len] = '\0';
        codes[(unsigned char)root->character] = strdup(code);
        return;
    }
    code[len] = '0';
    getCode(root->left, code, codes, len+1);
    code[len] = '1';
    getCode(root->right, code, codes, len+1);
}

void writeBit(FILE* out, unsigned char bit, unsigned char* byteBuffer, int* bitCount) {
    *byteBuffer = (*byteBuffer << 1) | (bit & 1);
    (*bitCount)++;

    if (*bitCount == 8) {
        fwrite(byteBuffer, 1, 1, out);
        *bitCount = 0;
        *byteBuffer = 0;
    }
}

void flushBits(FILE* out, unsigned char* byteBuffer, int* bitCount) {
    if (*bitCount > 0) {
        *byteBuffer <<= (8 - *bitCount);
        fwrite(byteBuffer, 1, 1, out);
    }
}

void compressFile(char* input, char* output, char** codes, int* freqs){
    FILE* inputFile = fopen(input, "r");
    FILE* outputFile = fopen(output, "wb");

    fwrite(freqs, sizeof(int), 256, outputFile);

    char byteBuffer = 0;
    int bitCount = 0;

    int c = 0;
    while ((c = fgetc(inputFile)) != EOF) {
        char* code = codes[(unsigned char)c];
        for (int i = 0; code[i]; i++) {
            writeBit(outputFile, code[i] - '0', &byteBuffer, &bitCount);
        }
    }

    flushBits(outputFile, &byteBuffer, &bitCount);
    fclose(inputFile);
    fclose(outputFile);
}

void decompress(const char* input, const char* output, int cap) {
    FILE* in = fopen(input, "rb");
    if (!in) {
        perror("Input file error");
        exit(1);
    }

    FILE* out = fopen(output, "w");
    if (!out) {
        perror("Output file error");
        exit(1);
    }

    int freqs[256] = {0};
    fread(freqs, sizeof(int), 256, in);

    HeapNode* root = buildTree(freqs, cap);
    HeapNode* curr = root;

    int byte;
    while ((byte = fgetc(in)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            curr = (bit == 0) ? curr->left : curr->right;

            if (curr->left == NULL && curr->right == NULL) {
                fputc(curr->character, out);
                curr = root;
            }
        }
    }

    fclose(in);
    fclose(out);
}

