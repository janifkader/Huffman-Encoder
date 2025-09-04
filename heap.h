#ifndef _HEAP_H
#define _HEAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct heap_node {
  int priority; 
  char character;
  struct heap_node* left;
  struct heap_node* right;
} HeapNode;

typedef struct min_heap {
  int size;
  int capacity;
  HeapNode* arr;
} MinHeap;


/*
 * Creates node for Minheap.
 * priority: node priority (frequency of the char in our case).
 * character: The character associated with the node.
 * return: The newly created HeapNode
 */

HeapNode* createNode(int priority, char character);

/*
 * Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.
 * heap: The mineap.
 * nodeIndex: The index of the node whose left child we look for.
 * return: index of left child.
 */
int leftIdx(MinHeap* heap, int nodeIndex);

/*
 * Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.
 * heap: The mineap.
 * nodeIndex: The index of the node whose right child we look for.
 * return: index of right child.
 */
int rightIdx(MinHeap* heap, int nodeIndex);


/* 
 * Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.
 * heap: The mineap.
 * nodeIndex: The index of the node whose parent we look for.
 * return: index of parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex);

/* 
 * Returns index of node with ID 'id' in minheap 'heap'.
 * heap: The minheap.
 * id: The character whose node we look for.
 * return: The index of the node associated with the character.
 */
int indexOf(MinHeap* heap, int id);

/*
 * Swaps the two elements in the minheap.
 * heap: The minheap.
 * index1: The first node's index to be swapped.
 * index2: The second node's index to be swapped.
 * return: void.
 */
void swap(MinHeap* heap, int index1, int index2);

/* 
 * Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap.
 * heap: The minheap.
 * nodeIndex: The index of the node to be bubbled up
 * return: void.
 */
void bubbleUp(MinHeap* heap, int nodeIndex);

/* 
 * Removes and returns the node with minimum priority in minheap 'heap'.
 * heap: The minheap.
 * return: The minimum node in the heap.
 */
HeapNode* extractMin(MinHeap* heap);

/* 
 * Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * heap: The minheap.
 * node: The node to be inserted into the minheap.
 * return: void.
 */
void insert(MinHeap* heap, HeapNode* node);

/* 
 * Returns priority of the node with ID 'id' in 'heap'.
 * heap: The minheap.
 * id: The character associated with the node whose priority
 * we are checking.
 * return: priority of the node associated with the character.
 */
int getPriority(MinHeap* heap, int id);

/* 
 * Returns a newly created empty minheap with initial capacity 'capacity'.
 * capacity: The total capacity of the minheap.
 * return: The new minheap.
 */
MinHeap* newHeap(int capacity);

/* 
 * Frees all memory allocated for minheap 'heap'.
 * heap: The minheap.
 * return: void.
 */
void deleteHeap(MinHeap* heap);

/* 
 * Frees all memory allocated for nodes in the Huffman Tree.
 * tree: The root node in the tree.
 * return: void.
 */
void deleteTree(HeapNode* tree);

#endif
