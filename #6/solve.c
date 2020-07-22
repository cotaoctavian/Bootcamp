#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define CAPACITY 15

struct Queue {
    int top, size;
    int *array;

    unsigned capacity;
};

// Initialize a new queue.
struct Queue* initialize(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    
    queue->capacity = capacity;
    queue->size = 0;
    queue->array = (int *)malloc(capacity * sizeof(int));
    queue->top = 0;

    return queue;
}

// Check if the queue is empty.
bool isEmpty(struct Queue* queue) {
    return queue->size == 0;
}

// Check if the queue is full.
bool isFull(struct Queue* queue) {
    return queue->size == queue->capacity;
}

// Add an element to queue 
void push(struct Queue* queue, uint32_t value) {
    // Check if the size of the queue is lower than the capacity of it.
    if(isFull(queue)) return;

    queue->array[queue->size] = value;
    queue->size += 1;
}

// Remove top element from queue and return it.
int pop(struct Queue* queue) {
    if(isEmpty(queue)) {
        printf("The queue is empty. Invalid operation.");
        return INT32_MIN;
    } else {
        int el = queue->array[queue->top];

        int i;
        for(i = 0; i <= queue->size - 1; i++) {
            queue->array[i] = queue->array[i + 1];
        }

        queue->size -= 1;
        
        return el;
    }
}

// Get the top element.
int front(struct Queue* queue) {
    if(isEmpty(queue)) return INT32_MIN;
    return queue->array[queue->top];
}

// Print the elements of the queue
void printQueue(struct Queue* queue) {
    int i;

    printf("The queue values are: ");
    for(i = 0; i <= queue->size - 1; i++) {
        printf("%d ", queue->array[i]);
    }

    printf("\n");
}


int main() {

    struct Queue* queue = initialize(CAPACITY);

    push(queue, 6);
    push(queue, 67);
    push(queue, 21);
    push(queue, 3);

    printQueue(queue);
    printf("The front element is: %d\n", front(queue));

    printf("The removed item is: %d\n", pop(queue));
    printQueue(queue);

    printf("The front element is: %d\n", front(queue));

    return 0;
}