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

/**************************************************************
 *                PUBLIC FUNCTIONS DECLARATION                *
 **************************************************************/

MessageQueue *initialize_message_queue(void);
void deinitialize_message_queue(MessageQueue *queue);
void send_message(MessageQueue *queue, char word[]);
void receive_message(MessageQueue *queue);

#endif
