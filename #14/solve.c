#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

#define QUEUE_SIZE 8

typedef struct _Queue 
{
    int top, rear;
    unsigned size, capacity;
    char **message;
} Queue;

/* Declarations */
Queue *queue;
char words[256][8] = {"test", "haha", "nu", "queue", "salut", "idk", "buna", "pa"};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* 
    * @brief - Initialize a new queue. 
    * @param[in] - unsigned capacity - the parameter that stores the capacity of the queue
    * @return - returns the initialized queue
*/
Queue *init(unsigned capacity) 
{
    Queue *queue = (Queue *) malloc (sizeof(Queue));

    queue->capacity = capacity;
    queue->rear = capacity - 1; 
    queue->size = 0;
    queue->top = 0;
    queue->message = (char **) malloc (sizeof(char *) * capacity);

    return queue;
}

/* 
    * @brief - Check if the queue is empty.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns false/true
*/
bool is_empty(Queue *queue) 
{
    return queue->size == 0;
}

/* 
    * @brief - Check if the queue is full.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns false/true
*/
bool is_full(Queue *queue) 
{
    return queue->size == queue->capacity;
}

/* 
    * @brief - Add an element to queue.
    * @param[in] - char *message is the item that is going to be added into the queue
    * @param[in/out] - struct Queue *queue is the modified queue
*/ 
void push(Queue *queue, char *message) 
{
    if (is_full(queue)) return;

    queue->rear = (queue->rear + 1) % queue->capacity;

    queue->message[queue->rear] = (char *) malloc (sizeof(message));
    strcpy(queue->message[queue->rear], message);

    queue->size++; 
} 

/* 
    * @brief - Remove top element from queue and return it.
    * @param[in/out] - struct Queue *queue is the modified queue
    * @return - returns the removed element or a NULL value in case of an error
*/
char *pop(Queue *queue) 
{
    if (is_empty(queue)) return NULL;

    char *item = (char *) malloc (sizeof(queue->message[queue->top]));
    strcpy(item, queue->message[queue->top]);

    queue->top = (queue->top + 1) % queue->capacity; 
    queue->size--;

    return item;
}

/* 
    * @brief - Get the top element.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns the index of the top element or a negative value in case of an error
*/
int front(Queue *queue) 
{
    if (is_empty(queue)) return INT32_MIN; 

    return queue->top;
}

/* 
    * @brief - Get the rear element.
    * @param[in] - struct Queue *queue is the queue
    * @return - returns the index of the rear element or a negative value in case of an error
*/
int get_rear(Queue *queue) 
{
    if (is_empty(queue)) return INT32_MIN;

    return queue->rear;
}

/*
    * @brief - This is the producer function that adds elements to the queue
    * @param[in] - void *args stores the value that is decremented to add n values to the queue
    * @return - returns NULL
*/
void *sendMessage(void *args) 
{
    pthread_mutex_lock(&lock);
    int *noOfItems = (int *) args;

    while (*noOfItems) 
    {
        push(queue, words[(*noOfItems) % QUEUE_SIZE]);
        printf("Producer pushed to queue: %s\n", words[(*noOfItems) % QUEUE_SIZE]);
        (*noOfItems)--;
    }

    printf("\n------------------------------------\n\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}

/* 
    * @brief - Consumer function that reads the elements from the queue and clears it
    * @return - returns NULL
*/
void *receiveMessage(void *args) 
{
    pthread_mutex_lock(&lock);

    while (!is_empty(queue)) 
    {
        printf("Consumer poped from queue: %s\n", pop(queue));
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() 
{

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
