/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define THREAD_SIZE 10

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static void *print_info(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/** 
* @brief     This function prints out the value passed as a parameter
* @param[in] args - is the variable that is going to be printed passed as a parameter to thread's function
* @return    returns NULL 
*/
static void *print_info(void *args) 
{
    int *ptr = (int *) args;
    printf("%d\n", *ptr);

    return NULL;
}

int main() 
{
    pthread_t thread_id[THREAD_SIZE];
    int i = 0;
    
    for (i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_create(&thread_id[i], NULL, print_info, &i);
    }

    for(i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_join(thread_id[i], NULL);
    }
    
    return 0;
}
