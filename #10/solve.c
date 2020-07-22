#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#define THREAD_SIZE 10

static bool run = true;

/*
    * @brief - This function prints out the value passed as a parameter
    * @param[in] - void *args is the variable that is going to be printed passed as a parameter to thread's function
    * @return - returns NULL 
*/
void *processing(void *args) 
{

    int *noOfThread = (int *) args;

    printf("%s%d%s\n", "Thread #", *noOfThread, " is working.");

    while (run) 
    {

    }

    printf("%s%d%s\n", "Thread #", *noOfThread, " stopped.");

    return NULL;
}

int main() {
    pthread_t tid[THREAD_SIZE];
    int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int i;
    for (i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_create(&tid[i], NULL, processing, &v[i]); 
    }

    sleep(5);

    run = false;

    for (i = 0; i < THREAD_SIZE; i++) 
    {
        pthread_join(tid[i], NULL); 
    }

    return 0;
}
