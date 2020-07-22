#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_SIZE 10

pthread_mutex_t lock;

void *printInfo(void *args) {
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

int main() {
    pthread_t tid[THREAD_SIZE];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n Something went wrong while initializing the mutex.\n");
        return 1;
    }

    printf("------------------------------\n");

    int i;
    int v = 0;
    for(i = 0; i < THREAD_SIZE; i++) {
        pthread_create(&tid[i], NULL, printInfo, &v);
    }

    for(i = 0; i < THREAD_SIZE; i++) {
        pthread_join(tid[i], NULL);
    }   

    pthread_mutex_destroy(&lock);
    
    return 0;
}