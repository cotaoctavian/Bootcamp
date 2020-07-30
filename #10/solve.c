/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define THREAD_SIZE 10

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static bool run = true;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

void set_run(bool value);
bool get_run(); 
void *processing(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
 * @brief     This function sets the value of the run variable.
 * @param[in] value - is the variable that sets the value for run variable.
 */
void set_run(bool value) 
{
    pthread_mutex_lock(&lock);
    run = value;
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  This function gets the value of the run variable.
 * @return true is the threads are running, false otherwise. 
 */
bool get_run() 
{
    bool local_run = false;

    pthread_mutex_lock(&lock);
    local_run = run;
    pthread_mutex_unlock(&lock);

    return local_run;
}

/**
 * @brief     This function prints out the value passed as a parameter
 * @param[in] args - is the variable that is going to be printed passed as a parameter to thread's function
 * @return    returns NULL 
 */
void *processing(void *args) 
{
    int *no_of_thread = (int *) args;

    printf("%s%d%s\n", "Thread #", *no_of_thread, " is working.");

    while (get_run()) 
    {

    }

    printf("%s%d%s\n", "Thread #", *no_of_thread, " stopped.");

    return NULL;
}

int main() 
{
    pthread_t tid[THREAD_SIZE];
    int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i = 0;

    for (i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_create(&tid[i], NULL, processing, &v[i]); 
    }

    sleep(5);

    set_run(false);

    for (i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_join(tid[i], NULL); 
    }

    return 0;
}
