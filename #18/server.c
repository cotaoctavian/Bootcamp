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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static bool run = true;
static time_t my_time;
static struct tm *timeinfo;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static bool check_client_status(int client); 
static void *test_ping(void *args);
static void *server_client_communication(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

/**
 * @brief     This function checks if the client is alive or not sending messages at a specified interval.
 * @param[in] args - stores the file descriptor of the client
 * @return    NULL
 */
static bool check_client_status(int client)
{      
    bool response     = false;
    char message[256] = {0};
    clock_t start;
    clock_t end;
    
    bzero(message, 256);    

    start = clock();

    pthread_mutex_lock(&lock);

    /* read the message from client and copy it in message */
    read(client, message, sizeof(message)); 

    end = clock();

    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    printf("\n[Server] Elapsed time for checking if the client is working: %f seconds.\n", seconds);

    if (0 == strcmp(message, "Ping") && 5 > (int) seconds)
    {   

        time(&my_time);
        timeinfo = localtime(&my_time);

        /* print message which contains the client contents */
        printf("[%02d:%02d:%02d][Server] Message received from the client: %s\n\n", 
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);

        bzero(message, 256);
        strcpy(message, "Pong");
    
        /* and send that message to client */ 
        write(client, message, sizeof(message)); 

        response = true;
    }
    else
    {
        printf("[Server] Client is not working properly..\n");
        response = false;
    }

    pthread_mutex_unlock(&lock);


    return response;
}

/**
 * @brief     This function communicates with the client.
 * @param[in] args - stores the server's socket
 * @return    NULL
 */
static void *server_client_communication(void *args)
{   
    int *client       = (int *) args;
    char message[256] = {0};

    while (true == run)
    {
        bzero(message, sizeof(message));

        pthread_mutex_lock(&lock);

        read(*client, message, sizeof(message)); 

        if (0 < strlen(message))
        {
            time(&my_time);
            timeinfo = localtime(&my_time);

            /* print message which contains the client contents */
            printf("[%02d:%02d:%02d][Server] Message received from the client: %s\n", 
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
        }
        else 
        {
            printf("\n[Server] Didn't receive any message from the client..\n");
            printf("[Server] Establishing the connection with the client..\n\n");
            run = false;
            break;
        }

        bzero(message, sizeof(message));

        strcpy(message, "Hello, my client!");

        write(*client, message, sizeof(message));

        pthread_mutex_unlock(&lock);

        sleep(1);
    }

    close(*client);
    
    return NULL;
}

/**
 * @brief     This function calls the function that checks if the client is still working.
 * @param[in] args - stores the client's socket
 * @return    NULL
 */
static void *test_ping(void *args) 
{   
    int *client = (int *) args;

    while (true == run)
    {
        run = check_client_status(*client);

        if (true == run) 
        {
            sleep(5);
        }
        else 
        {
            break;
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

    struct sockaddr_in server;
    struct sockaddr_in from;

    pthread_t server_thread;
    pthread_t communication_thread;

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
                perror("[Server] Something went wrong while listening.. \n");
            }
            else 
            {   
                while (true)
                {
                    len = sizeof (from);
                    bzero (&from, sizeof (from));

                    if (false == run)
                    {
                        run = true;
                        system("x-terminal-emulator -e \"/home/uic84863/GIT/Bootcamp/#18/client\"");
                    }


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

                            pthread_create(&server_thread, NULL, test_ping, &client);

                            pthread_create(&communication_thread, NULL, server_client_communication, &client);

                            pthread_join(server_thread, NULL);

                            pthread_join(communication_thread, NULL);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
