#ifndef MESSAGEQ_H

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define MESSAGEQ_H

typedef struct _MessageQueue 
{   
    int top;
    int rear;
    unsigned int size;
    unsigned int capacity;
    char **message;
    pthread_mutex_t lock;
} MessageQueue;

/** 
* @brief  Initialize a new queue. 
* @return returns the initialized queue
*/
MessageQueue *initialize_message_queue(void);

/**
 * @brief     Deinitialize the queue.
 * @param[in] queue - is the queue that is going to be deinitialized
 */
void deinitialize_message_queue(MessageQueue *queue);

/**
* @brief     Function that adds an element to the queue
* @param[in] queue - is the queue that is going to be modified.
* @param[in] word  - is the string that is going to be added into the queue.
*/
void send_message(MessageQueue *queue, char word[]);

/** 
* @brief  Function that removes the top element from the queue
* @return returns the string popped from the queue
*/
char *receive_message(MessageQueue *queue);

#endif
