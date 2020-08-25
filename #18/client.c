/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <glib.h>
#include <time.h>

/* gcc client.c -Wall -o client `pkg-config --cflags --libs glib-2.0` -lpthread */

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define PORT 8080 

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static void *check_server_status(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

static void *check_server_status(void *args)
{   
    char message[256] = {0};
    int *server_sock = (int *) args;
    clock_t start, end;

    while (true)
    {
        bzero(message, sizeof(message)); 

        strcpy(message, "Ping");

        /* Send it to the server. */
        write(*server_sock, message, sizeof(message)); 
        bzero(message, sizeof(message)); 
        
        start = clock();

        /* Read the message from the server. */
        read(*server_sock, message, sizeof(message)); 

        end = clock();

        double seconds = (double) (end - start) / CLOCKS_PER_SEC;

        printf("[Client] Elapsed time: %f seconds.\n", seconds);

        /* Check the exit case. */
        if (0 == strcmp(message, "Pong") && 5 > (int) seconds) 
        {   
            printf("[Client] Message received from the server: %s\n", message); 
            sleep(5);
        }  
        else
        {
            printf("[Client] Server is not working properly..\n"); 
            break; 
        }
    }

    close(*server_sock);

    return NULL;
}

int main(void) 
{
    int sock = 0;
    pthread_t client_thread;

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sock)
    {
        perror("[Client] Something went wrong while creating the socket");
    }
    else 
    {
        /* assign IP, PORT */ 
        server.sin_family = AF_INET; 
        server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        server.sin_port = htons(PORT); 

        /* Connect to the server */
        if (-1 == connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)))
        {
            perror("[Client] Something went wrong while connecting to the server");
        }
        else
        {
            printf("[Client] Connected to the server.\n");

            pthread_create(&client_thread, NULL, check_server_status, &sock);

            pthread_join(client_thread, NULL);
        }
    }

    return 0;
}