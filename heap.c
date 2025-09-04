#include "heap.h"

HeapNode* createNode(int priority, char character){
    HeapNode* p = (HeapNode*)malloc(sizeof(HeapNode));
    p->priority = priority;
    p->character = character;
    p->left = NULL;
    p->right = NULL;
    return p;
}

int leftIdx(MinHeap* heap, int nodeIndex){
    return 2 * nodeIndex;
}

int rightIdx(MinHeap* heap, int nodeIndex){
    return 2 * nodeIndex + 1;
}

int parentIdx(MinHeap* heap, int nodeIndex){
    return nodeIndex/2;
}

int indexOf(MinHeap* heap, int id){
    return id;
}

void swap(MinHeap* heap, int index1, int index2){
    if (index1 <= heap->capacity && index2 <= heap->capacity){
        if (index1 > 0 && index2 > 0){
            HeapNode p = heap->arr[index2];
            heap->arr[index2] = heap->arr[index1];
            heap->arr[index1] = p;
        }
    }
}

void bubbleUp(MinHeap* heap, int nodeIndex){
    swap(heap, parentIdx(heap, nodeIndex), nodeIndex);
}

HeapNode* extractMin(MinHeap* heap){
    HeapNode* q = malloc(sizeof(HeapNode));
    *q = heap->arr[1]; // Store min
    swap(heap, 1, heap->size); // Delete min from heap
    heap->arr[heap->size].priority = -1;
    heap->arr[heap->size].character = -1;
    heap->size = heap->size-1;
    int v = 1;
    int left;
    int right;
    int c;
    while (true) { // Loop through heap to search for successor
        left = leftIdx(heap, v);
        right = rightIdx(heap, v);
        c = v;
        if (left <= heap->size && heap->arr[left].priority < heap->arr[c].priority) {
            c = left;
        }
        if (right <= heap->size && heap->arr[right].priority < heap->arr[c].priority) {
            c = right;
        }
        if (c == v) { // Successor found
            break;
        }
        swap(heap, v, c);
        v = c;
    }
  return q; // Return min
}

void insert(MinHeap* heap, HeapNode* node) {
    if (heap->size >= heap->capacity) {
        printf("Heap is full!\n");
        return;
    }

    heap->size++;
    int i = heap->size;
    heap->arr[i] = *node;
    free(node);

    while (i > 1 && heap->arr[i].priority < heap->arr[i / 2].priority) {
        swap(heap, i, i / 2); // Bubble up
        i = i / 2;
    }
}

int getPriority(MinHeap* heap, int id){
    return heap->arr[indexOf(heap, id)].priority;
}

MinHeap* newHeap(int capacity){
    MinHeap *new_heap = malloc(sizeof(MinHeap));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->arr = calloc(capacity+1, sizeof(HeapNode));
    return new_heap;
}

void deleteHeap(MinHeap* heap){
    free(heap->arr);
    free(heap);
}

void deleteTree(HeapNode *root){
    if (root == NULL){
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}