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

/* gcc server.c -o server `pkg-config --cflags --libs glib-2.0` -lpthread */

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
    char command[256] = {0};
    int *client = (int *) args;
    clock_t start;
    clock_t end;

    while (true)
    {
        bzero(message, sizeof(message));

        printf("[Server] Type the command you want to send: ");
        
        int k = 0;
        while ('\n' != (message[k++] = getchar()));

        message[strcspn(message, "\n")] = 0;

        bzero(command, sizeof(command));

        bool response = true;

        if (0 == strcmp(message, "1")) 
        {
            strcpy(command, "START_VALIDATION");
        } 
        else if (0 == strcmp(message, "2")) 
        {
            strcpy(command, "CHECK_PACKAGE");
        } 
        else if (0 == strcmp(message, "3"))
        {
            strcpy(command, "START_INSTALATION");
        }
        else if (0 == strcmp(message, "4"))
        {
            strcpy(command, "STOP_INSTALLATION");
        }
        else if (0 == strcmp(message, "5"))
        {
            strcpy(command, "PAUSE_INSTALLATION");
        }
        else if (0 == strcmp(message, "6"))
        {
            strcpy(command, "RESUME_INSTALLATION");
        }
        else 
        {
            printf("\n[Server] This command is not available. Try again.\n");
            response = false;
        }

        if (true == response)
        {
            write(*client, command, sizeof(command));

            bzero(message, sizeof(message));

            start = clock();

            read(*client, message, sizeof(message)); 

            end = clock();

            double seconds = (double) (end - start) / CLOCKS_PER_SEC;

            if (0 < strlen(message) && 5 > (int) seconds)
            {
                time(&my_time);
                timeinfo = localtime(&my_time);

                /* print message which contains the client contents */
                printf("[%02d:%02d:%02d][Server] Message received from the client: %s\n", 
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
            }
            else 
            {
                printf("\n[Server] Didn't receive any message from the client in the specified time..\n");
                printf("[Server] Try to reconnect again\n\n");
                break;
            }
        }
    }

    close(*client);

    return NULL; 
}

int main(void) 
{
    int optval = 1;
    int client = 0;
    int sock   = 0; 
    int len    = 0;
    pid_t childpid;
    pthread_t server_thread;

    struct sockaddr_in server;
    struct sockaddr_in from;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sock)
    {
        perror("[Server] Something went wrong while creating the socket. \n");
    }
    else 
    { 
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        bzero(&server, sizeof(server));

        server.sin_family      = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port        = htons(PORT);

        if (-1 == bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)))
        {
            perror("[Server] Something went wrong while trying to bind the socket. \n");
        } 
        else 
        {
            if (-1 == listen(sock, 10))
            {
                perror("[Server] Something went wrong while trying to listen for clients. \n");
            }
            else 
            {
                while (true) 
                {
                    len = sizeof (from);
                    bzero (&from, sizeof (from));

                    client = accept(sock, (struct sockaddr *) &from, &len);

                    if (0 > client)
                    {
                        perror("[Server] Error raised while accepting the connection.\n");
                    }
                    else 
                    {
                        if(0 == (childpid = fork()))
                        {
                            printf("[Server] The client connected to the server.\n");
                            fflush (stdout);

                            pthread_create(&server_thread, NULL, get_acknowledge, &client);
                            pthread_join(server_thread, NULL);
                        }
                    }
                }
            }
        }

    }

    return 0;
}