/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include "message_queue.h"

int main() 
{
    MessageQueue *queue = initialize_message_queue();

    pthread_t producer;
    pthread_t consumer;

    /* Create producer thread */
    pthread_create(&producer, NULL, send_message, queue);

    /* Create consumer thread */
    pthread_create(&consumer, NULL, receive_message, queue);

    /* Terminate threads */
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    deinitialize_message_queue(queue);

    return 0;
}