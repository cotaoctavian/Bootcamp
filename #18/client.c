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
 *                      GLOBAL VARIABLES                      *
 **************************************************************/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static bool run = true;
static time_t my_time;
static struct tm *timeinfo;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static bool check_server_status(int server_sock);
static void *test_pong(void *args);
static void *client_server_communication(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
 * @brief     This function checks if the server is still working
 * @param[in] args - stores the server's socket
 * @return    NULL
 */
static bool check_server_status(int server_sock)
{      
    bool response     = false;
    char message[256] = {0};
    clock_t start, end;

    bzero(message, sizeof(message)); 

    strcpy(message, "Ping");

    pthread_mutex_lock(&lock);

    /* Send it to the server. */
    write(server_sock, message, sizeof(message)); 
    bzero(message, sizeof(message)); 
    
    start = clock();

    /* Read the message from the server. */
    read(server_sock, message, sizeof(message)); 

    pthread_mutex_unlock(&lock);

    end = clock();

    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    printf("\n[Client] Elapsed time for checking if the server is working: %f seconds.\n", seconds);

    /* Check the exit case. */
    if (0 == strcmp(message, "Pong") && 5 > (int) seconds) 
    {   
        time(&my_time);
        timeinfo = localtime(&my_time);

        /* print message which contains the client contents */
        printf("[%02d:%02d:%02d][Client] Message received from the server: %s\n\n", 
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);

        response = true;
    }  
    else
    {
        printf("[Client] Server is not working properly..\n"); 
        response = false; 
    }

    return response;
}

/**
 * @brief     This function communicates with the server.
 * @param[in] args - stores the server's socket
 * @return    NULL
 */
static void *client_server_communication(void *args)
{   
    char message[256] = {0};
    int *server_sock  = (int *) args;

    while (true == run)
    {
        /* Send other messages than ping pong */
        bzero(message, sizeof(message));

        pthread_mutex_lock(&lock);

        strcpy(message, "Hello, my server!");

        write(*server_sock, message, sizeof(message)); 

        bzero (message, sizeof(message));

        read(*server_sock, message, sizeof(message));

        pthread_mutex_unlock(&lock);

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
            run = false;
            break;
        }
    }

    close(*server_sock);

    return NULL;
}

/**
 * @brief     This function calls the function that checks if the server is still working.
 * @param[in] args - stores the server's socket
 * @return    NULL
 */
static void *test_pong(void *args) 
{
    int *server = (int *) args;

    while (true == run)
    {
        run = check_server_status(*server);

        if (true == run) 
        {
            sleep(5);
        }
        else 
        {
            break;
        }
    }

    return NULL;
}

int main(void) 
{   
    int sock = 0;
    pthread_t client_thread;
    pthread_t communication_thread;

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sock)
    {
        perror("[Client] Something went wrong while creating the socket");
    }
    else 
    {
        /* assign IP, PORT */ 
        server.sin_family      = AF_INET; 
        server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        server.sin_port        = htons(PORT); 

        /* Connect to the server */
        if (-1 == connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)))
        {
            perror("[Client] Something went wrong while connecting to the server");
        }
        else
        {
            printf("[Client] Connected to the server.\n");

            pthread_create(&client_thread, NULL, test_pong, &sock);

            pthread_create(&communication_thread, NULL, client_server_communication, &sock);

            pthread_join(client_thread, NULL);

            pthread_join(communication_thread, NULL);
        }
    }

    return 0;
}
