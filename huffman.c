#include "huffman.h"

void getFrequency(int* frequencies, const char *filename){
    FILE *file = fopen(filename, "r"); // Open file to read
    if (!file){
        perror("File error");
        return;
    }
    char c;
    while ((c = fgetc(file)) != EOF){ // Gets each character in the file
        if (c >= 0 && c < 256) {
            frequencies[c]++; // Increases the frequency at the char's index
        }
    }
    fclose(file); // Close the file
}

HeapNode* buildTree(int* freqs, int cap){
    MinHeap* heap = newHeap(cap); // Create new heap
    for (int i = 0; i < 256; i++){ // Loop through all characters
        if (*(freqs + i) > 0){ // Check if the character is in the file
            insert(heap, createNode(*(freqs + i), i)); // Creates node for the character
        }
    }
    while (heap->size > 1) { // Loop until only 1 node is in the heap to create the Huffman tree
        HeapNode *left = extractMin(heap); // Extract the two smallest nodes
        HeapNode *right = extractMin(heap);
        HeapNode *newNode = createNode('\0', left->priority + right->priority); // Create a new node with their sum
        newNode->left = left; // Make the two smallest nodes the children of the new node
        newNode->right = right;
        insert(heap, newNode); // Reinsert the node into the heap
    }
    HeapNode* min = extractMin(heap); // Extract the final remaining node
    deleteHeap(heap); // Delete the heap
    return min;
}

void getCode(HeapNode* root, char* code, char** codes, int len){
    if (root == NULL){ // Base case for recursion
        return;
    }
    if (root->left == NULL && root->right == NULL){
        code[len] = '\0'; // Ends the code with the null character
        codes[(unsigned char)root->character] = strdup(code); // Add it to the list of codes
        return;
    }
    code[len] = '0'; // Generaes the code based on the Huffman tree
    getCode(root->left, code, codes, len+1);
    code[len] = '1';
    getCode(root->right, code, codes, len+1);
}

void writeBit(FILE* out, unsigned char bit, unsigned char* byteBuffer, int* bitCount) {
    *byteBuffer = (*byteBuffer << 1) | (bit & 1); // Adds the bit to the buffer
    (*bitCount)++; // Adds one the bit count

    if (*bitCount == 8) { // Checks if we have enough bits for a byte
        fwrite(byteBuffer, 1, 1, out); // Write the buffer to the output
        *bitCount = 0; // Resets the bit count
        *byteBuffer = 0; // Resets the buffer
    }
}

void flushBits(FILE* out, unsigned char* byteBuffer, int* bitCount) {
    if (*bitCount > 0) { // Checks if there are any bits remaining
        *byteBuffer <<= (8 - *bitCount);
        fwrite(byteBuffer, 1, 1, out); // Writes the remaining bits
    }
}

void compressFile(char* input, char* output, char** codes, int* freqs){
    FILE* inputFile = fopen(input, "r"); // Open the input file for reading
    FILE* outputFile = fopen(output, "wb"); // Open the output file for writing

    fwrite(freqs, sizeof(int), 256, outputFile); // Write the frequencies of each char to the output file

    char byteBuffer = 0;
    int bitCount = 0;

    int c = 0;
    while ((c = fgetc(inputFile)) != EOF) { // Gets each character from the input file
        char* code = codes[(unsigned char)c]; // Retrieves the associated code
        for (int i = 0; code[i]; i++) {
            writeBit(outputFile, code[i] - '0', &byteBuffer, &bitCount); // Writes each bit of the code to the output file
        }
    }

    flushBits(outputFile, &byteBuffer, &bitCount); // Write any remaining bits
    fclose(inputFile); // Close both files
    fclose(outputFile);
}

void decompress(const char* input, const char* output, int cap) {
    FILE* in = fopen(input, "rb"); // Open the input file for reading
    if (!in) {
        perror("Input file error");
        exit(1);
    }

    FILE* out = fopen(output, "w"); // Open the output file for writing
    if (!out) {
        perror("Output file error");
        exit(1);
    }

    int freqs[256] = {0}; // Initialize frequencies table
    fread(freqs, sizeof(int), 256, in); // Read the frequencies from the .huff file

    HeapNode* root = buildTree(freqs, cap); // Build a new huffman tree
    HeapNode* curr = root;

    int byte;
    while ((byte = fgetc(in)) != EOF) {
        for (int i = 7; i >= 0; i--) { // Decode the codes based on our tree
            int bit = (byte >> i) & 1;
            if (bit == 0) {
                curr = curr->left;
            } 
            else {
                curr = curr->right;
            }
            if (curr->left == NULL && curr->right == NULL) {
                fputc(curr->character, out);
                curr = root;
            }
        }
    }

    fclose(in); // Close the input file
    fclose(out); // Close the output file
}

