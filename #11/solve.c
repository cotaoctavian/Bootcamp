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
    unsigned int capacity;
    unsigned int size;
    int *arr;
} Queue;

/* Declarations */
Queue *queue;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t consumer;
static int v = 0;

/** 
* @brief     - Initialize a new queue. 
* @param[in] - unsigned capacity - the parameter that stores the capacity of the queue
* @return    - returns the initialized queue
*/
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

/** 
* @brief     - Check if the queue is empty.
* @param[in] - struct Queue *queue - is the queue
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
* @param[in] - struct Queue *queue - is the queue
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
* @param[in]     - uint32_t value - is the item that is going to be added into the queue
* @param[in/out] - struct Queue *queue is the modified queue
*/ 
void push(Queue *queue, int value) 
{
    if (NULL != queue) 
    {
        pthread_mutex_lock(&lock2);

        if (0 == is_full(queue)) 
        {
            queue->rear = (queue->rear + 1) % queue->capacity;

            queue->arr[queue->rear] = value;

            queue->size++; 
        }

        pthread_mutex_unlock(&lock2);
    }
    else 
    {
        printf("The queue is NULL.");
    }
} 

/** 
* @brief         - Remove top element from queue and return it.
* @param[in/out] - struct Queue *queue - is the modified queue
* @return        - returns the removed element or a negative value in case of an error
*/
int pop(Queue *queue) 
{   
    int result = 0;
    if (NULL != queue)
    {
        pthread_mutex_lock(&lock2);
        
        if (0 != is_empty(queue)) 
        {
            result = INT32_MIN;
        }

        result = queue->arr[queue->top];

        queue->top = (queue->top + 1) % queue->capacity; 
        queue->size--;

        pthread_mutex_unlock(&lock2);
    }
    else 
    {
        printf("The queue is NULL.");
        result = INT32_MIN;
    }
    
    return result;
}

/** 
* @brief     - Get the top element.
* @param[in] - struct Queue *queue - is the queue
* @return    - returns the top element or a negative value in case of an error
*/
int front(Queue *queue) 
{       
    int result = 0;
    
    if (NULL != queue)
    {
        if (0 != is_empty(queue)) 
        {
            result = INT32_MIN;
        }  
        else 
        {
            result = queue->arr[queue->top];
        }
    }
    else 
    {
        result = INT32_MIN;
    }

    return result;
}

/** 
* @brief     - Get the rear element.
* @param[in] - struct Queue *queue - is the queue
* @return    - returns the rear element or a negative value in case of an error
*/
int get_rear(Queue *queue) 
{   
    int result = 0;

    if (NULL != queue)
    {
        if(0 != is_empty(queue)) 
        {
            result = INT32_MIN;
        }
        else 
        { 
            result = queue->arr[queue->rear];
        }
    }
    else
    {
        result = INT32_MIN;
    }
    
    return result;
}

/** 
* @brief - This function allocates memory to the queue
*/
void initializeEngine() 
{
    queue = init(QUEUE_SIZE);
}

/**
* @brief     - Deallocate the memory for the struct. 
* @param[in] - Queue *queue is the queue.
*/
void deinitialize(Queue *queue) 
{
    if(NULL != queue) 
    {   
        free(queue);
        queue = NULL;
    }
    else 
    {
        printf("The queue is NULL.");
    }
}

/** 
* @brief     - This function is used by producer threads to print their id and the value passed as an argument, also incrementing it and push it to the queue.
* @param[in] - void *args is the value passed to the function to increment it
* @return    - return NULL
*/
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

/** 
* @brief     - This function is used by consumer thread to print the values from the queue and clear it.
* @param[in] - void *args - is empty
* @return    - return NULL
*/
void *on_consume(void *args) 
{
    pthread_mutex_lock(&lock);

    while (0 == is_empty(queue)) 
    {
        printf("Consumer popped from queue value: %d\n", pop(queue));
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/** 
* @brief - This function starts the consumer thread.
*/
void start_engine() 
{
    pthread_create(&consumer, NULL, on_consume, NULL);
}

/** 
* @brief - This function stops the consumer thread.
*/
void stop_engine() 
{
    pthread_join(consumer, NULL);
}

/**
* @brief - This function deallocate the memory of the queue
*/
void destroy_engine() 
{
    if (NULL != queue) 
    { 
        free(queue);
        queue = NULL;
    }
    else 
    {
        printf("The queue is NULL.");
    }
}

int main() 
{
    int i = 0;

    pthread_t producers[THREADS];

    initializeEngine();

    if (NULL != queue) 
    {
        for (i = 0; i < THREADS; i++) 
        {
            pthread_create(&producers[i], NULL, produce, &v);
        }
        
        for (i = 0; i < THREADS; i++)
        {
            pthread_join(producers[i], NULL);
        }

        start_engine();

        stop_engine();
        destroy_engine();
    }
    else 
    {
        printf("The queue is NULL.");
    }
    
    return 0;
}
