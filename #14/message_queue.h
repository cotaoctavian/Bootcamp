#ifndef MESSAGEQ_H

/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/
#define MESSAGEQ_H
#define QUEUE_SIZE 500

typedef struct _MessageQueue 
{   
    int top;
    int rear;
    unsigned int size;
    unsigned int capacity;
    char **message;
} MessageQueue;

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool signal = false;
char words[256][8] = {"test", "haha", "nu", "queue", "salut", "idk", "buna", "pa"};

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

MessageQueue *initialize_message_queue(void);
void deinitialize_message_queue(MessageQueue *queue);
void *send_message(void *args);
void *reiceive_message(void *args);
void push(MessageQueue *queue, char *message);
void set_signal(bool value);
bool get_signal();
int is_empty(MessageQueue *queue);
int is_full(MessageQueue *queue);
char *pop(MessageQueue *queue);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
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

    return queue;
}

/** 
* @brief     Check if the queue is empty.
* @param[in] queue - is the queue
* @return    returns false/true
*/
int is_empty(MessageQueue *queue) 
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
int is_full(MessageQueue *queue) 
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
void push(MessageQueue *queue, char *message) 
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
* @brief         Remove top element from queue and return it.
* @param[in/out] queue - is the modified queue
* @return        returns the removed element or a NULL value in case of an error
*/
char *pop(MessageQueue *queue) 
{
    char *result = NULL;

    if (NULL != queue) 
    {
        if (1 == is_empty(queue))
        {
            result = NULL;
        }
        else 
        {   
            char *item = (char *) malloc (sizeof(queue->message[queue->top]));
            strcpy(item, queue->message[queue->top]);
            queue->top = (queue->top + 1) % queue->capacity; 
            queue->size--;

            result = strdup(item);
        }
    }
    else 
    {
        printf("The queue is NULL.");
    }

    return result;
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
void *send_message(void *args) 
{
    MessageQueue *queue = (MessageQueue *) args;

    while (true) 
    {   
        if (false == get_signal()) 
        {   
            char word[256] = {0};
            strcpy(word, words[rand() % 8]);
            push(queue, word);
            printf("Producer pushed to queue: %s\n", word);
            set_signal(true);
        }
    }

    return NULL;
}

/** 
* @brief  Consumer function that reads the elements from the queue and clears it
* @return returns NULL
*/
void *receive_message(void *args) 
{
    MessageQueue *queue = (MessageQueue *) args;
    
    while (true) 
    {   
        if ((true == get_signal()) && (0 == is_empty(queue))) 
        {
            printf("Consumer poped from queue: %s\n", pop(queue));
            set_signal(false);
        }
    }

    return NULL;
}

/**
 * @brief     This function sets a value to signal that the consumer thread can read data from the queue or not.
 * @param[in] value - is the signal's modified value.
 */
void set_signal(bool value) 
{   
    pthread_mutex_lock(&lock);
    signal = value;
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  This function returns the state of the signal
 * @return true, otherwise false
 */
bool get_signal()
{   
    bool local_signal = false;
    
    pthread_mutex_lock(&lock);
    local_signal = signal;
    pthread_mutex_unlock(&lock);

    return local_signal;
}

#endif