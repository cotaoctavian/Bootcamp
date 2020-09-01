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

static char command[256] = {0};
static bool send_cmd     = false;
static bool run          = true;
static bool stop         = false;
static time_t my_time;
static struct tm *timeinfo;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**************************************************************
 *                FUNCTIONS DECLARATION                       *
 **************************************************************/

static bool get_stop();
static bool get_run();
static bool get_cmd();
static void set_stop(bool value);
static void set_run(bool value);
static void set_cmd(bool value);
static void *get_acknowledge(void *args);
static void *send_command(void *args);

/**************************************************************
 *                FUNCTIONS DEFINITION                        *
 **************************************************************/

static void set_stop(bool value)
{
    pthread_mutex_lock(&lock);
    stop = value;
    pthread_mutex_unlock(&lock);
}

static void set_run(bool value)
{
    pthread_mutex_lock(&lock);
    run = value;
    pthread_mutex_unlock(&lock);
}

static void set_cmd(bool value)
{
    pthread_mutex_lock(&lock);
    send_cmd = value;
    pthread_mutex_unlock(&lock);
}

static bool get_stop()
{
    bool local_stop = false;

    pthread_mutex_lock(&lock);
    local_stop = stop;
    pthread_mutex_unlock(&lock);

    return local_stop;
}

static bool get_run()
{
    bool local_run = false;

    pthread_mutex_lock(&lock);
    local_run = run;
    pthread_mutex_unlock(&lock);

    return local_run;
}

static bool get_cmd()
{
    bool local_cmd = false;

    pthread_mutex_lock(&lock);
    local_cmd = send_cmd;
    pthread_mutex_unlock(&lock);

    return local_cmd;
}

static void *send_command(void *args)
{   
    int k             = 0;
    char message[256] = {0};

    while (false == get_stop())
    {
        if (true == get_run())
        {   
            time(&my_time);
            timeinfo = localtime(&my_time);

            printf("[%02d:%02d:%02d][Server] Type the command you want to send: ", 
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

            k = 0;

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
                strcpy(command, "START_INSTALLATION");
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
                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("[%02d:%02d:%02d][Server] This command is not available. Try again.\n", 
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
                    
                response = false;
            }

            set_cmd(response);
            set_run(false);
        }
    }
}

static void *get_acknowledge(void *args)
{
    char message[256] = {0};
    int *client       = (int *) args;
    int k             = 0;
    int res           = 0;
    clock_t start;
    clock_t end;

    while (false == get_stop())
    {   
        if (true == get_cmd())
        {
            write(*client, command, sizeof(command));

            bzero(message, sizeof(message));

            start = clock();

            int result = read(*client, message, sizeof(message)); 

            end = clock();

            double seconds = (double) (end - start) / CLOCKS_PER_SEC;

            if (0 < result && 5 > (int) seconds)
            {
                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("[%02d:%02d:%02d][Server] Message received from the client: %s\n", 
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);

                if (0 == strcmp(message, "REQUEST_ACCEPTED"))
                {
                    bzero(message, sizeof(message));

                    res = 0;

                    while (0 >= (res = read(*client, message, sizeof(message))))
                    {
                        printf("%d", res);
                    }

                    printf("[%02d:%02d:%02d][Server] Message received from the client: %s\n", 
                    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
                }
                else
                {
                    set_run(true);
                    set_cmd(false);
                }
            }
            else 
            {   
                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("\n[%02d:%02d:%02d][Server] Didn't receive any message from the client in the specified time..\n", 
                        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("[%02d:%02d:%02d][Server] Try to reconnect again\n\n", 
                      timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

                break;
            }

            set_cmd(false);
            set_run(true);
        }
        else
        {   
            bzero(message, sizeof(message));
            
            char buffer[32];
            res = recv(*client, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);

            if (0 == res)
            {
                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("\n[%02d:%02d:%02d][Server] Didn't receive any message from the client..\n", 
                        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

                time(&my_time);
                timeinfo = localtime(&my_time);

                printf("[%02d:%02d:%02d][Server] Try to reconnect again\n\n", 
                      timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

                set_stop(true);

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
    pthread_t send_thread;

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
                        set_stop(false);

                        if(0 == (childpid = fork()))
                        {
                            printf("[Server] The client connected to the server.\n");
                            fflush (stdout);

                            pthread_create(&server_thread, NULL, get_acknowledge, &client);
                            pthread_create(&send_thread, NULL, send_command, NULL);

                            pthread_join(server_thread, NULL);
                            
                            if (true == stop) 
                            {   
                                pthread_cancel(send_thread);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
