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
* @param[in] queue - is the queue.
* @return    Returns false or true.
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
* @return    Returns true or false.
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
* @brief         Adds an element to the queue.
* @param[in/out] queue   - is the modified queue
* @param[in]     message - is the item that is going to be added into the queue
*/ 
static void push(MessageQueue *queue, char *message) 
{
    if (NULL != queue)
    {
        if (1 == is_full(queue)) 
        {
            printf("The queue is full.\n");
        }
        else 
        {
            queue->rear = (queue->rear + 1) % queue->capacity;

            queue->message[queue->rear] = strdup(message);

            queue->size++; 

            printf("Producer pushed to queue: %s\n", message);

            free(message);
        }
    }
    else 
    {
        printf("The queue is NULL.\n");
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
        printf("The queue is NULL.\n");
    }

    return result;
}

/**************************************************************
 *             PUBLIC FUNCTIONS DEFINITION                    *
 **************************************************************/

/** 
* @brief  Initialize a new queue. 
* @return returns the initialized queue
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
 * @brief     Deinitialize the queue.
 * @param[in] queue - is the queue that is going to be deinitialized
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
* @brief     Function that adds an element to the queue.
* @param[in] queue - is the queue that is going to be modified.
* @param[in] word  - is the string that is going to be added into the queue.
*/
void send_message(MessageQueue *queue, char word[]) 
{
    pthread_mutex_lock(&queue->lock);

    push(queue, word);

    pthread_mutex_unlock(&queue->lock);
}

/** 
* @brief  Function that removes the top element from the queue.
* @return Returns the string removed from the queue.
*/
char *receive_message(MessageQueue *queue) 
{
    char *message = "";

    pthread_mutex_lock(&queue->lock);
 
    message = strdup(pop(queue));
    
    pthread_mutex_unlock(&queue->lock);

    return message;
}
