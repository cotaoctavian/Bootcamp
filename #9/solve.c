#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_SIZE 10

pthread_mutex_t lock;

/** 
* @brief     - This function prints out the value passed as a parameter and increment it, allowing only to one thread a time to acces the critical section
* @param[in] - void *args - is the variable that is going to be printed passed as a parameter to thread's function
* @return    - returns NULL 
*/
void *print_info(void *args) 
{
    /* Lock the thread using mutex */
    pthread_mutex_lock(&lock);

    /* Get the variable from args */
    int *ptr = (int *) args;    

    printf("Job %d started.\n", *ptr);

    printf("Job %d finished.", *ptr);

    printf("\n------------------------------\n");

    /* Increment it */
    (*ptr)++;

    /* Unlock the thread using mutex */
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() 
{
    int i = 0;
    int v = 0;
    pthread_t tid[THREAD_SIZE];

    if (0 != pthread_mutex_init(&lock, NULL)) 
    {
        printf("\n Something went wrong while initializing the mutex.\n");
    }
    else 
    {
        printf("------------------------------\n");

        for (i = 0; i < THREAD_SIZE; i++) 
        {
            pthread_create(&tid[i], NULL, print_info, &v);
        }

        for (i = 0; i < THREAD_SIZE; i++) 
        {
            pthread_join(tid[i], NULL);
        }   

        pthread_mutex_destroy(&lock);
    }

    return 0;
}
