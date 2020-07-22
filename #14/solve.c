#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

#define QUEUE_SIZE 8

typedef struct _Queue {
    int top, rear;
    unsigned size, capacity;
    char **message;
} Queue;

/* Declarations */
Queue *queue;
char words[256][8] = {"test", "haha", "nu", "queue", "salut", "idk", "buna", "pa"};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Initialize queue */
Queue* init(unsigned capacity) {
    Queue* queue = (Queue*) malloc (sizeof(Queue));

    queue->capacity = capacity;
    queue->rear = capacity - 1; 
    queue->size = 0;
    queue->top = 0;
    queue->message = (char **) malloc (sizeof(char *) * capacity);

    return queue;
}

/* Check if the queue is empty */
bool isEmpty(Queue* queue) {
    return queue->size == 0;
}

/* Check if the queue is full */
bool isFull(Queue* queue) {
    return queue->size == queue->capacity;
}

/* Add an item to the queue */
void push(Queue* queue, char *message) {
    if(isFull(queue)) return;

    queue->rear = (queue->rear + 1) % queue->capacity;

    queue->message[queue->rear] = (char *) malloc (sizeof(message));
    strcpy(queue->message[queue->rear], message);

    queue->size++; 
} 

/* Remove an item from queue */
char* pop(Queue* queue) {
    if(isEmpty(queue)) return NULL;

    char *item = (char *) malloc (sizeof(queue->message[queue->top]));
    strcpy(item, queue->message[queue->top]);

    queue->top = (queue->top + 1) % queue->capacity; 
    queue->size--;

    return item;
}

/* Get top item from queue */
int getTop(Queue* queue) {
    if(isEmpty(queue)) return INT32_MIN; 
    return queue->top;
}

/* Get rear item from queue */
int getRear(Queue* queue) {
    if(isEmpty(queue)) return INT32_MIN; 
    return queue->rear;
}

/* Producer function */
void *sendMessage(void *args) {
    pthread_mutex_lock(&lock);
    int *noOfItems = (int *) args;

    while(*noOfItems) {
        push(queue, words[(*noOfItems) % QUEUE_SIZE]);
        printf("Producer pushed to queue: %s\n", words[(*noOfItems) % QUEUE_SIZE]);
        (*noOfItems)--;
    }

    printf("\n------------------------------------\n\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}

/* Consumer function */
void *receiveMessage(void *args) {
    pthread_mutex_lock(&lock);

    while(!isEmpty(queue)) {
        printf("Consumer poped from queue: %s\n", pop(queue));
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {

    queue = init(QUEUE_SIZE);
    pthread_t producer;
    pthread_t consumer;

    int v = 8;

    /* Create producer thread */
    pthread_create(&producer, NULL, sendMessage, &v);

    /* Create consumer thread */
    pthread_create(&consumer, NULL, receiveMessage, NULL);

    /* Terminate threads */
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    free(queue);

    return 0;
}