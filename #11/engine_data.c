/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define QUEUE_SIZE 10
#define THREADS 10

typedef struct _Queue 
{
    int top;
    int rear;
    unsigned int capacity;
    unsigned int size;
    int *arr;
} Queue;

typedef struct _ENGINE_DATA 
{   
    int index;
    pthread_t producer[THREADS];
    pthread_t consumer;
    pthread_mutex_t lock, lock2;
    Queue *queue;
} ENGINE_DATA;

/**************************************************************
 *                STATIC DECLARATION                          *
 **************************************************************/

static bool run = true;

/**************************************************************
 *                STATIC DECLARATION                          *
 **************************************************************/

static Queue *init(unsigned int capacity);
static int is_empty(Queue *queue);
static int is_full(Queue *queue);
static int pop(Queue *queue);
static void push(Queue *queue, int value);
static void *on_consume(void *args);
static void *on_produce(void *args);
static void set_run(bool value, ENGINE_DATA *engine);
static bool get_run(ENGINE_DATA *engine);

/**************************************************************
 *             STATIC FUNCTIONS DEFINITION                    *
 **************************************************************/

/** 
* @brief     Initialize a new queue. 
* @param[in] capacity - the parameter that stores the capacity of the queue
* @return    returns the initialized queue
*/
static Queue *init(unsigned int capacity) 
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
* @brief     Check if the queue is empty.
* @param[in] queue - is the queue
* @return    returns false/true
*/
static int is_empty(Queue *queue)
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
* @brief     Check if the queue is full.
* @param[in] queue - is the queue
* @return    returns false/true
*/
static int is_full(Queue *queue) 
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
* @brief         Add an element to queue.
* @param[in]     value - is the item that is going to be added into the queue
* @param[in/out] queue - is the modified queue
*/ 
static void push(Queue *queue, int value) 
{
    if (NULL != queue) 
    {
        if (0 == is_full(queue)) 
        {
            queue->rear = (queue->rear + 1) % queue->capacity;

            queue->arr[queue->rear] = value;

            queue->size++; 
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }
} 

/** 
* @brief         Remove top element from queue and return it.
* @param[in/out] queue - is the modified queue
* @return        returns the removed element or a negative value in case of an error
*/
static int pop(Queue *queue) 
{   
    int result = 0;
    
    if (NULL != queue)
    {
        if (0 != is_empty(queue)) 
        {
            result = INT32_MIN;
        }

        result = queue->arr[queue->top];

        queue->top = (queue->top + 1) % queue->capacity; 
        queue->size--;

    }
    else 
    {
        printf("The queue is NULL.");
        result = INT32_MIN;
    }
    
    return result;
}

static void set_run(bool value, ENGINE_DATA *engine) 
{
    pthread_mutex_lock(&engine->lock2);
    run = value;
    pthread_mutex_unlock(&engine->lock2);
}

static bool get_run(ENGINE_DATA *engine)
{
    bool local_run = false;

    pthread_mutex_lock(&engine->lock2);
    local_run = run;
    pthread_mutex_unlock(&engine->lock2);

    return local_run;
}

/**************************************************************
 *             PUBLIC FUNCTIONS DEFINITION                    *
 **************************************************************/

/** 
 * @brief This function allocates memory to ENGINE_DATA struct.
 */
void *initializeEngine(void)
{
    ENGINE_DATA *engine = (ENGINE_DATA *) malloc (sizeof(ENGINE_DATA));

    if (NULL != engine) 
    {
        engine->queue = init(QUEUE_SIZE);
        
        if (NULL == engine->queue)
        {
            printf("The queue is NULL.");
        }

        engine->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
        engine->lock2 = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
        engine->index = 0;
    }
    else 
    {
        printf("The engine is NULL.");
    }

    return engine;
}

/**
 * @brief     Deallocate the memory for ENGINE_DATA struct. 
 * @param[in] engine - is the struct that contains the queue & the mutex.
 */
void destroyEngine(void *engine)
{
    engine = (ENGINE_DATA *) engine;

    if (NULL != engine)
    {   
        if (NULL != ((ENGINE_DATA *) engine)->queue) 
        {   
            if (NULL != (((ENGINE_DATA *) engine)->queue->arr)) 
            {
                free(((ENGINE_DATA *) engine)->queue->arr);
                ((ENGINE_DATA *) engine)->queue->arr = NULL;
            }

            free(((ENGINE_DATA *) engine)->queue);
            ((ENGINE_DATA *) engine)->queue = NULL;

            pthread_mutex_destroy(&((ENGINE_DATA *) engine)->lock);
            pthread_mutex_destroy(&((ENGINE_DATA *) engine)->lock2);
        }
        else 
        {
            printf("The queue is NULL.");
        }

        free(engine);
        engine = NULL;
    }
    else 
    {
        printf("The engine is NULL.");
    } 
}

/** 
 * @brief     This function starts the consumer thread.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 * @return    returns 1
 */
int startEngine(void *engine)
{   
    int i = 0;

    while (i < THREADS)
    {
        pthread_create(&((ENGINE_DATA *) engine)->producer[i], NULL, on_produce, engine);
        i++;
    }

    pthread_create(&((ENGINE_DATA *) engine)->consumer, NULL, on_consume, engine);

    return 1;
}

/** 
 * @brief This function stops the consumer thread.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 */
void stopEngine(void *engine)
{
    int i = 0;
    engine = (ENGINE_DATA *) engine;
    set_run(false, (ENGINE_DATA *) engine);

    while (i < THREADS)
    {
        pthread_join(((ENGINE_DATA *) engine)->producer[i], NULL);
        i++;
    }

    pthread_join(((ENGINE_DATA *) engine)->consumer, NULL);
}

/**
 * @brief     Function that receives an integer, prints it's thread id and index value and calls push into the synchronized queue
 * @param[in] engine - is the struct that contains the queue & the mutex.
 * @param[in] index  - is the number of the thread.
 * @return    1 if success, otherwise 0
 */
int produce(void *engine, int index)
{   
    engine = (ENGINE_DATA *) engine;

    pthread_mutex_lock(&((ENGINE_DATA *) engine)->lock);
    push(((ENGINE_DATA *) engine)->queue, index);   
    pthread_mutex_unlock(&((ENGINE_DATA *) engine)->lock);

    printf("Producer with the thread id: %lu - value: %d\n", pthread_self(), index);
    
    return 1;
}

/**
 * @brief     Function that is called by a consumer thread, extracts the elements from the queue and prints them.
 * @param[in] engine - is the struct that contains the queue & the mutex.
 * @param[in] index  - is the number of the thread.
 * @return    1 if success, otherwise 0
 */
int onConsume(void *engine, int index)
{
    int value = 0;
    int response = 0;
    engine = (ENGINE_DATA *) engine;

    if (0 == is_empty(((ENGINE_DATA *) engine)->queue)) 
    {   
        pthread_mutex_lock(&((ENGINE_DATA *) engine)->lock);
        value = pop(((ENGINE_DATA *) engine)->queue);
        pthread_mutex_unlock(&((ENGINE_DATA *) engine)->lock);

        printf("Consumer with thread's number %d popped from queue value: %d\n", index, value);
        response = 1;
    }
    else
    {
        response = 0;    
    }

    return response;
}

/**
 * @brief     This function is used by the consumer thread on the creation phase.
 * @param[in] args - passed an index as parameter.
 */
void *on_consume(void *args)
{   
    int result = 0;
    ENGINE_DATA *engine = (ENGINE_DATA *) args;
    engine->index++;

    while (true == get_run(engine))
    {   
        result = onConsume(engine, engine->index);
    }

    printf("%s\n", "Removing the elements remained in the queue..");

    while (0 == is_empty(engine->queue))
    {
        result = pop(((ENGINE_DATA *) engine)->queue);
        printf("Consumer with thread's number %d popped from queue value: %d\n", engine->index, result);
    }

    printf("%s\n", "Done!");

    return NULL;
}

/**
 * @brief     This function is used by the producer threads on the creation phase.
 * @param[in] args - passed an index as parameter.
 */
void *on_produce(void *args)
{
    int result = 0;
    ENGINE_DATA *engine = (ENGINE_DATA *) args;
    engine->index++;

    while (true == get_run(engine))
    {
        result = produce(engine, engine->index);

        if (0 == result)
        {
            break;
        }
    }

    return NULL;
}
