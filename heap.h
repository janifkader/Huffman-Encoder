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

HeapNode* createNode(int priority, char character);

int leftIdx(MinHeap* heap, int nodeIndex);

int rightIdx(MinHeap* heap, int nodeIndex);

int parentIdx(MinHeap* heap, int nodeIndex);

HeapNode nodeAt(MinHeap* heap, int nodeIndex);

int priorityAt(MinHeap* heap, int nodeIndex);

char idAt(MinHeap* heap, int nodeIndex);

int indexOf(MinHeap* heap, int id);

void swap(MinHeap* heap, int index1, int index2);

void bubbleUp(MinHeap* heap, int nodeIndex);

HeapNode* extractMin(MinHeap* heap);

void insert(MinHeap* heap, HeapNode* node);

int getPriority(MinHeap* heap, int id);

bool decreasePriority(MinHeap* heap, int id, int newPriority);

MinHeap* newHeap(int capacity);

void deleteHeap(MinHeap* heap);

#endif
