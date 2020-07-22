#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

#define QUEUE_SIZE 10
#define THREADS 10

typedef struct _Queue 
{
    int top, rear;
    unsigned size, capacity;
    int *arr;
} Queue;

/* Declarations */
Queue *queue;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t consumer;

static int v = 0;

/* Initialize queue */
Queue *init(unsigned capacity) 
{
    Queue *queue = (Queue *) malloc (sizeof(Queue));

    queue->capacity = capacity;
    queue->rear = capacity - 1; 
    queue->size = 0;
    queue->top = 0;
    queue->arr = (int *) malloc (sizeof(int) * capacity);

    return queue;
}

/* Check if the queue is empty */
bool isEmpty(Queue *queue) 
{
    return queue->size == 0;
}

/* Check if the queue is full */
bool isFull(Queue *queue) 
{
    return queue->size == queue->capacity;
}

/* Add an item to the queue */
void push(Queue *queue, int value) 
{

    pthread_mutex_lock(&lock2);

    if (isFull(queue)) return;

    queue->rear = (queue->rear + 1) % queue->capacity;

    queue->arr[queue->rear] = value;

    queue->size++; 

    pthread_mutex_unlock(&lock2);
} 

/* Remove an item from queue */
int pop(Queue *queue) 
{

    pthread_mutex_lock(&lock2);
    
    if (isEmpty(queue)) return INT32_MIN;

    int item = queue->arr[queue->top];

    queue->top = (queue->top + 1) % queue->capacity; 
    queue->size--;

    pthread_mutex_unlock(&lock2);

    return item;
}

/* Get top item from queue */
int getTop(Queue *queue) 
{
    if (isEmpty(queue)) return INT32_MIN; 

    return queue->arr[queue->top];
}

/* Get rear item from queue */
int getRear(Queue *queue) 
{
    if (isEmpty(queue)) return INT32_MIN; 

    return queue->arr[queue->rear];
}

/* Allocate memory to queue */
void initializeEngine() 
{
    queue = init(QUEUE_SIZE);
}

void *produce(void *args) 
{
    int *value = (int *) args;

    pthread_mutex_lock(&lock);

    printf("Thread id: %lu - value: %d\n", pthread_self(), *value);

    push(queue, *value);

    (*value)++;

    pthread_mutex_unlock(&lock);

    return NULL;
}

void *onConsume(void *args) 
{

    pthread_mutex_lock(&lock);

    while (!isEmpty(queue)) 
    {
        printf("Consumer popped from queue value: %d\n", pop(queue));
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

void startEngine() 
{
    pthread_create(&consumer, NULL, onConsume, NULL);
}

void stopEngine() 
{
    pthread_join(consumer, NULL);
}

void destroyEngine() 
{
    free(queue);
}

int main() 
{

    pthread_t producers[THREADS];

    initializeEngine();

    int i;
    
    for (i = 0; i < THREADS; i++) 
    {
        pthread_create(&producers[i], NULL, produce, &v);
    }
    
    for (i = 0; i < THREADS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    startEngine();

    stopEngine();
    destroyEngine();

    return 0;
}
