#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_SIZE 10

void *printInfo(void *args) {
    int *ptr = (int *) args;
    printf("%d\n", *ptr);

    return NULL;
}

int main() {
    pthread_t thread_id[THREAD_SIZE];
    int i;
    int v = 0;
    for(i = 0; i < THREAD_SIZE; i++) {
        pthread_create(&thread_id[i], NULL, printInfo, &i);
        pthread_join(thread_id[i], NULL);
    }
    
    return 0;
}