#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

#define QUEUE_SIZE 8

typedef struct _Queue 
{
    int top;
    int rear;
    unsigned int size;
    unsigned int capacity;
    char **message;
} Queue;

/* Declarations */
Queue *queue;
char words[256][8] = {"test", "haha", "nu", "queue", "salut", "idk", "buna", "pa"};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/** 
* @brief     - Initialize a new queue. 
* @param[in] - capacity - the parameter that stores the capacity of the queue
* @return    - returns the initialized queue
*/
Queue *init(unsigned int capacity) 
{
    Queue *queue = (Queue *) malloc (sizeof(Queue));

    queue->capacity = capacity;
    queue->rear = capacity - 1; 
    queue->size = 0;
    queue->top = 0;
    queue->message = (char **) malloc (sizeof(char *) * capacity);

    return queue;
}

/** 
* @brief     - Check if the queue is empty.
* @param[in] - Queue *queue - is the queue
* @return    - returns false/true
*/
bool is_empty(Queue *queue) 
{
    int ret_val = 0;

    if (NULL != queue) 
    {
        ret_val = queue->size == 0; 
    } 
    else 
    {
        ret_val = -1;
    }

    return ret_val;
}

/** 
* @brief     - Check if the queue is full.
* @param[in] - Queue *queue - is the queue
* @return    - returns false/true
*/
bool is_full(Queue *queue) 
{
    int ret_val = 0;

    if (NULL != queue)
    {
        ret_val = queue->size == queue->capacity;
    }
    else 
    {
        ret_val = -1;
    }

    return ret_val;
}

/** 
* @brief         - Add an element to queue.
* @param[in]     - char *message - is the item that is going to be added into the queue
* @param[in/out] - struct Queue *queue is the modified queue
*/ 
void push(Queue *queue, char *message) 
{
    if (NULL != queue)
    {
        if (true == is_full(queue)) 
        {
            printf("The queue is full.");
        }
        else 
        {
            queue->rear = (queue->rear + 1) % queue->capacity;

            queue->message[queue->rear] = (char *) malloc (sizeof(message));
            strcpy(queue->message[queue->rear], message);

            queue->size++; 
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }
} 

/** 
* @brief         - Remove top element from queue and return it.
* @param[in/out] - Queue *queue - is the modified queue
* @return        - returns the removed element or a NULL value in case of an error
*/
char *pop(Queue *queue) 
{
    char *result = NULL;
    if (NULL != queue) 
    {
        if (true == is_empty(queue))
        {
            result = NULL;
        }
        else 
        {   
            char *item = (char *) malloc (sizeof(queue->message[queue->top]));
            strcpy(item, queue->message[queue->top]);
            queue->top = (queue->top + 1) % queue->capacity; 
            queue->size--;

            strcpy(result, item);
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }

    return result;
}

/** 
* @brief     - Get the top element.
* @param[in] - Queue *queue - is the queue
* @return    - returns the index of the top element or a negative value in case of an error
*/
int front(Queue *queue) 
{
    int result = 0;

    if (NULL != queue) 
    {
        if (true == is_empty(queue)) 
        {
            result = INT32_MIN;
            printf("The queue is empty.");
        }
        else {
            result = queue->top;
        }
    }       
    else 
    {
        printf("The queue is NULL");
        result = INT32_MIN;
    }

    return result;
}

/** 
* @brief     - Get the rear element.
* @param[in] - Queue *queue - is the queue
* @return    - returns the index of the rear element or a negative value in case of an error
*/
int get_rear(Queue *queue) 
{
    int result = 0;
    
    if (NULL != queue) 
    {
        if (true == is_empty(queue)) 
        {
            result = INT32_MIN;
            printf("The queue is empty.");
        }
        else
        {
            result = queue->rear;
        }
    }
    else 
    {
        printf("The queue is NULL.");
        result = INT32_MIN;
    }

    return result;
}

/**
* @brief - This is the producer function that adds elements to the queue
* @param[in] - void *args stores the value that is decremented to add n values to the queue
* @return - returns NULL
*/
void *sendMessage(void *args) 
{
    pthread_mutex_lock(&lock);
    int *no_of_items = (int *) args;

    while (0 != *no_of_items) 
    {
        push(queue, words[(*no_of_items) % QUEUE_SIZE]);
        printf("Producer pushed to queue: %s\n", words[(*no_of_items) % QUEUE_SIZE]);
        (*no_of_items)--;
    }

    printf("\n------------------------------------\n\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}

/** 
* @brief - Consumer function that reads the elements from the queue and clears it
* @return - returns NULL
*/
void *receiveMessage(void *args) 
{
    pthread_mutex_lock(&lock);

    while (false == is_empty(queue)) 
    {
        printf("Consumer poped from queue: %s\n", pop(queue));
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() 
{
    queue = init(QUEUE_SIZE);

    if (NULL != queue) 
    {    
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

        if (NULL != queue) 
        {
            free(queue);
            queue != NULL;
        }   
    }

    return 0;
}
