#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef __MinHeap_header
#define __MinHeap_header

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

HeapNode* createNode(int priority, char character){
    HeapNode* p = (HeapNode*)malloc(sizeof(HeapNode));
    p->priority = priority;
    p->character = character;
    p->left = NULL;
    p->right = NULL;
    return p;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex){
    return 2 * nodeIndex;
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap* heap, int nodeIndex){
    return 2 * nodeIndex + 1;
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex){
    return nodeIndex/2;
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex];
}

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex].priority;
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
char idAt(MinHeap* heap, int nodeIndex){
    return heap->arr[nodeIndex].character;
}

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
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

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex){
    swap(heap, parentIdx(heap, nodeIndex), nodeIndex);
}

/* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode* extractMin(MinHeap* heap){
    HeapNode* q = malloc(sizeof(HeapNode));
    *q = heap->arr[1];
    swap(heap, 1, heap->size);
    heap->arr[heap->size].priority = -1;
    heap->arr[heap->size].character = -1;
    heap->size = heap->size-1;
    int v = 1;
    int left;
    int right;
    int c;
    while (true) {
        left = leftIdx(heap, v);
        right = rightIdx(heap, v);
        c = v;

        if (left <= heap->size && heap->arr[left].priority < heap->arr[c].priority) {
            c = left;
        }
        if (right <= heap->size && heap->arr[right].priority < heap->arr[c].priority) {
            c = right;
        }
        if (c == v) {
            break;
        }
        swap(heap, v, c);
        v = c;
    }
  return q;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap* heap, HeapNode* node) {
    if (heap->size >= heap->capacity) {
        printf("Heap is full!\n");
        return;
    }

    heap->size++;
    int i = heap->size;
    heap->arr[i] = *node;

    // Bubble up
    while (i > 1 && heap->arr[i].priority < heap->arr[i / 2].priority) {
        swap(heap, i, i / 2);
        i = i / 2;
    }
}


/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap* heap, int id){
    return heap->arr[indexOf(heap, id)].priority;
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap* heap, int id, int newPriority){
    if (newPriority >= getPriority(heap, id)){
        return false;
    }
    heap->arr[indexOf(heap, id)].priority = newPriority;
    while (heap->arr[parentIdx(heap, indexOf(heap, id))].priority > newPriority){
        bubbleUp(heap, indexOf(heap, id));
    }
    return true;
}

/* Prints the contents of this heap, including size, capacity, full index
 * map, and, for each non-empty element of the heap array, that node's ID and
 * priority. */



/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap* newHeap(int capacity){
    MinHeap *new_heap = malloc(sizeof(MinHeap));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->arr = calloc(capacity+1, sizeof(HeapNode));
    return new_heap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap){
    free(heap->arr);
    free(heap);
}

#endif
