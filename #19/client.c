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
static enum {NONE = 0, VALIDATION_FAILED = 0, PACKAGE_CHECK_FAILED = 0, INSTALLATION_FAILED = 0, INSTALLATION_CANCELED = 0,
            VALIDATION_SUCCESS = 1, PACKAGE_CHECK_SUCCESS = 2, INSTALLATION_STARTED = 3, INSTALLATION_PAUSED = 4, INSTALLATION_SUCCESS = 5};
static int state_machine = NONE;

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

        int response = read(*server_sock, message, sizeof(message));

        if (0 >= response)
        {   
            time(&my_time);
            timeinfo = localtime(&my_time);

            printf("[%02d:%02d:%02d][Client] Didn't receive any message from the server..\n",
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
            break;   
        }
        else 
        {
            time(&my_time);
            timeinfo = localtime(&my_time);

            printf("[%02d:%02d:%02d][Client] Message received from the server: %s\n", 
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);


            char state[256] = {0};
            bool response = true;

            if (0 == strcmp(message, "START_VALIDATION"))
            {   
                if (NONE == state_machine) 
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is VALIDATION_SUCCESS");
                    state_machine = VALIDATION_SUCCESS;
                }
                else 
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = VALIDATION_FAILED;
                    response = false;
                }

            }
            else if (0 == strcmp(message, "CHECK_PACKAGE"))
            {
                if (VALIDATION_SUCCESS == state_machine)
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is CHECK_PACKAGE");
                    state_machine = PACKAGE_CHECK_SUCCESS;
                }
                else 
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = PACKAGE_CHECK_FAILED;
                    response = false;
                }
            }
            else if (0 == strcmp(message, "START_INSTALLATION"))
            {
                if (PACKAGE_CHECK_SUCCESS == state_machine)
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is START_INSTALLATION");
                    state_machine = INSTALLATION_STARTED;
                }
                else 
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = INSTALLATION_FAILED;
                    response = false;
                }
            }
            else if (0 == strcmp(message, "STOP_INSTALLATION"))
            {
                if(INSTALLATION_STARTED == state_machine)
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is STOP_INSTALLATION");
                    state_machine = INSTALLATION_CANCELED;
                }
                else 
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = INSTALLATION_FAILED;
                    response = false;
                }
            }
            else if (0 == strcmp(message, "PAUSE_INSTALLATION"))
            {
                if (INSTALLATION_STARTED == state_machine)
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is PAUSE_INSTALLATION");
                    state_machine = INSTALLATION_PAUSED;
                }
                else
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = INSTALLATION_FAILED;
                    response = false;
                }
            }
            else if (0 == strcmp(message, "RESUME_INSTALLATION"))
            {
                if (INSTALLATION_PAUSED == state_machine)
                {
                    strcpy(message, "REQUEST_ACCEPTED");
                    strcpy(state, "The current machine's state is RESUME_INSTALLATION");
                    state_machine = INSTALLATION_STARTED;
                }
                else
                {
                    strcpy(message, "REQUEST_REJECTED");
                    state_machine = INSTALLATION_FAILED;
                    response = false;
                }
            }

            write(*server_sock, message, sizeof(message)); 

            if (true == response)
            {
                sleep(3);
                write(*server_sock, state, sizeof(state));
            }
        }
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
            printf("\n[Client] Connected to the server. \n");

            pthread_create(&client_thread, NULL, get_acknowledge, &sock);
            pthread_join(client_thread, NULL);
        }
    }

    return 0;
}
