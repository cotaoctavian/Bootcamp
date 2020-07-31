/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "message_queue.h"

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define QUEUE_SIZE 50000

/**************************************************************
 *                STATIC DECLARATION                          *
 **************************************************************/

static void push(MessageQueue *queue, char *message);
static int is_empty(MessageQueue *queue);
static int is_full(MessageQueue *queue);
static char *pop(MessageQueue *queue);

/**************************************************************
 *             STATIC FUNCTIONS DEFINITION                    *
 **************************************************************/

/** 
* @brief     Check if the queue is empty.
* @param[in] queue - is the queue
* @return    returns false/true
*/
static int is_empty(MessageQueue *queue) 
{
    int ret_val = 0;

    if (NULL != queue) 
    {
        ret_val = queue->size == 0 ? 1 : 0; 
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
static int is_full(MessageQueue *queue) 
{
    int ret_val = 0;

    if (NULL != queue)
    {
        ret_val = queue->size == queue->capacity ? 1 : 0;
    }
    else 
    {
        ret_val = -1;
    }

    return ret_val;
}

/** 
* @brief         Add an element to queue.
* @param[in]     message - is the item that is going to be added into the queue
* @param[in/out] queue   - is the modified queue
*/ 
static void push(MessageQueue *queue, char *message) 
{
    if (NULL != queue)
    {
        if (1 == is_full(queue)) 
        {
            printf("The queue is full.");
        }
        else 
        {
            queue->rear = (queue->rear + 1) % queue->capacity;

            queue->message[queue->rear] = strdup(message);

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
* @return        returns the removed element or a NULL value in case of an error
*/
static char *pop(MessageQueue *queue) 
{
    char *result = "";

    if (NULL != queue) 
    {
        if (1 == is_empty(queue))
        {
            result = "";
        }
        else 
        {   
            result = strdup(queue->message[queue->top]);

            queue->top = (queue->top + 1) % queue->capacity; 

            queue->size--;
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }

    return result;
}

/**************************************************************
 *             PUBLIC FUNCTIONS DEFINITION                    *
 **************************************************************/

/** 
* @brief     Initialize a new queue. 
* @return    returns the initialized queue
*/
MessageQueue *initialize_message_queue(void) 
{
    MessageQueue *queue = (MessageQueue *) malloc (sizeof(MessageQueue));

    queue->capacity = QUEUE_SIZE;
    queue->rear = QUEUE_SIZE - 1; 
    queue->size = 0;
    queue->top = 0;
    queue->message = (char **) malloc (sizeof(char *) * QUEUE_SIZE);
    queue->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return queue;
}

/**
 * @brief     Deinitialize the stack.
 * @param[in] stack - is the stack that is going to be deinitialized
 */
void deinitialize_message_queue(MessageQueue *queue)
{
    if (NULL != queue)
    {
        if (NULL != queue->message) 
        {
            free(queue->message);
            queue->message = NULL;
        }

        free(queue);
        queue = NULL;
    }
    else
    {
        printf("The queue is NULL.");
    }
}

/**
* @brief     This is the producer function that adds elements to the queue
* @param[in] args - stores the value that is decremented to add n values to the queue
* @return    returns NULL
*/
void send_message(MessageQueue *queue, char word[]) 
{
    pthread_mutex_lock(&queue->lock);

    push(queue, word);
    printf("Producer pushed to queue: %s\n", word);

    pthread_mutex_unlock(&queue->lock);
}

/** 
* @brief  Consumer function that reads the elements from the queue and clears it
* @return returns NULL
*/
void receive_message(MessageQueue *queue) 
{
    char message[256] = {0};

    pthread_mutex_lock(&queue->lock);
 
    strcpy(message, pop(queue));
    printf("Consumer poped from queue: %s\n", message);
    
    pthread_mutex_unlock(&queue->lock);
}
