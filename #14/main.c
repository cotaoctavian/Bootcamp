/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "message_queue.h"

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

static char words[256][8] = {"unu", "doi", "trei", "patru", "cinci", "sase", "sapte", "opt"};

/**************************************************************
 *              STATIC FUNCTIONS DECLARATION                  *
 **************************************************************/

static void *on_produce(void *args);
static void *on_consume(void *args);

/**************************************************************
 *               STATIC FUNCTIONS DEFINITION                  *
 **************************************************************/

/**
 * @brief     This function is used by the producer thread to send messages.
 * @param[in] args - Parameter used to pass the queue
 * @return    null
 */
static void *on_produce(void *args) 
{   
    MessageQueue *queue = (MessageQueue *) args;

    while (true)
    {   
        send_message(queue, words[rand() % 7]);
        send_message(queue, words[rand() % 7]);
        send_message(queue, words[rand() % 7]);

        sleep(0.2);

        send_message(queue, words[rand() % 7]);
    }

    return NULL;
}

/**
 * @brief     This function is used by the consumer thread to get the messages.
 * @param[in] args - Parameters used to pass the queue
 * @return    null
 */
static void *on_consume(void *args)
{
    MessageQueue *queue = (MessageQueue *) args;
    
    while (true)
    {
        char response[256] = {0};

        strcpy(response, receive_message(queue));
        printf("Consumer poped from queue: %s\n", response);

        strcpy(response, "");
        strcpy(response, receive_message(queue));
        printf("Consumer poped from queue: %s\n", response);

        sleep(0.2);
    }

    return NULL;
}

int main() 
{
    MessageQueue *queue = initialize_message_queue();

    pthread_t producer;
    pthread_t consumer;

    /* Create producer thread */
    pthread_create(&producer, NULL, on_produce, queue);

    /* Create consumer thread */
    pthread_create(&consumer, NULL, on_consume, queue);

    /* Terminate threads */
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    deinitialize_message_queue(queue);

    return 0;
}
