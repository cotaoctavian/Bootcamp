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

static void *onProduce(void *args);
static void *onConsume(void *args);

/**************************************************************
 *               STATIC FUNCTIONS DEFINITION                  *
 **************************************************************/

/**
 * @brief     This function is used by the producer thread to send messages.
 * @param[in] Parameters used to pass the queue
 * @return    Returns null
 */
static void *onProduce(void *args) 
{   
    MessageQueue *queue = (MessageQueue *) args;

    while (true)
    {   
        send_message(queue, words[rand() % 7]);

        sleep(0.2);
    }

    return NULL;
}

/**
 * @brief     This function is used by the consumer thread to get the messages.
 * @param[in] Parameters used to pass the queue
 * @return    Returns null
 */
static void *onConsume(void *args)
{
    MessageQueue *queue = (MessageQueue *) args;
    
    while (true)
    {
        receive_message(queue);

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
    pthread_create(&producer, NULL, onProduce, queue);

    /* Create consumer thread */
    pthread_create(&consumer, NULL, onConsume, queue);

    /* Terminate threads */
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    deinitialize_message_queue(queue);

    return 0;
}
