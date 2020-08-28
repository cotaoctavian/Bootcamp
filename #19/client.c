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
#include <pthread.h>
#include <glib.h>
#include <time.h>
#include <arpa/inet.h>

/* gcc client.c -o client `pkg-config --cflags --libs glib-2.0` -lpthread */

/**************************************************************
 *                      DEFINES                               *
 **************************************************************/

#define PORT 8080

/**************************************************************
 *                      GLOBAL VARIABLES                      *
 **************************************************************/

static time_t my_time;
static struct tm *timeinfo;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static void *get_acknowledge(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

static void *get_acknowledge(void *args)
{
    char message[256] = {0};
    int *server_sock = (int *) args;

    while (true)
    {
        bzero (message, sizeof(message));

        read(*server_sock, message, sizeof(message));

        if (0 < strlen(message))
        {
            time(&my_time);
            timeinfo = localtime(&my_time);

            /* print message which contains the client contents */
            printf("[%02d:%02d:%02d][Client] Message received from the server: %s\n", 
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);

            sleep(1);
        }
        else 
        {
            printf("[Client] Didn't receive any message from the server..\n");
            break;
        }

        /* Send other messages than ping pong */
        bzero(message, sizeof(message));

        strcpy(message, "I received the command!\n");

        write(*server_sock, message, sizeof(message)); 
    }

    close(*server_sock);

    return NULL;
}

int main(void) 
{   
    int sock = 0;
    
    struct sockaddr_in server;

    pthread_t client_thread;

    /* Initialize the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sock)
    {
        perror("[Client] Something went wrong while initializing the socket.\n");
    }
    else
    {
        /* Assign IP, PORT, FAMILY */
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_port = htons(PORT);
        server.sin_family = AF_INET;

        /* Connect to the server */
        if (-1 == connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)))
        {
            perror("[Client] Something went wrong while connecting to the server.\n");
        }
        else 
        {
            printf("[Client] Connected to the server. \n");

            pthread_create(&client_thread, NULL, get_acknowledge, &sock);
            pthread_join(client_thread, NULL);
        }
    }

    return 0;
}