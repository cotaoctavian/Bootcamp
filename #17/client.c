/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <dbus/dbus.h>

/*
gcc client.c -o client `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`
*/

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

const char *const INTERFACE_NAME = "org.linux.dbus_example";
const char *const SERVER_BUS_NAME = "org.linux.server";
const char *const CLIENT_BUS_NAME = "org.linux.client";
const char *const SERVER_OBJECT_PATH_NAME = "/org/linux/adder";
const char *const CLIENT_OBJECT_PATH_NAME = "/org/linux/client";
const char *const METHOD_NAME = "add_numbers";
DBusError dbus_error;

/**************************************************************
 *              STATIC FUNCTIONS DECLARATION                  *
 **************************************************************/

static void print_dbus_error (char *str);

/**************************************************************
 *               STATIC FUNCTIONS DEFINITION                  *
 **************************************************************/

static void print_dbus_error (char *str) 
{
    fprintf (stderr, "%s: %s\n", str, dbus_error.message);
    dbus_error_free (&dbus_error);
}

int main (int argc, char **argv)
{
    DBusConnection *conn;
    int ret;
    char input[80];

    dbus_error_init (&dbus_error);

    conn = dbus_bus_get (DBUS_BUS_SESSION, &dbus_error);

    if (0 != dbus_error_is_set (&dbus_error))
    {
        print_dbus_error ("dbus_bus_get");
    }

    printf ("Please type two numbers: ");

    while (NULL != fgets (input, 78, stdin)) 
    {
        while (1) 
        {
            ret = dbus_bus_request_name (conn, CLIENT_BUS_NAME, 0, &dbus_error);

            if (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) 
            {
                break;
            }

            if (ret == DBUS_REQUEST_NAME_REPLY_IN_QUEUE) 
            {
               fprintf (stderr, "Waiting for the bus ... \n");
               sleep (1);
            }
            else
            {
                if (0 != dbus_error_is_set (&dbus_error))
                {
                    print_dbus_error ("dbus_bus_get");
                }   
            }            
        }
        
        DBusMessage *request;
        request = dbus_message_new_method_call (SERVER_BUS_NAME, SERVER_OBJECT_PATH_NAME, INTERFACE_NAME, METHOD_NAME);

        if (NULL == request) 
        {
            fprintf (stderr, "Error in dbus_message_new_method_call\n");
        }
        else
        {
            DBusMessageIter iter;
            dbus_message_iter_init_append (request, &iter);
            char *ptr = input;

            if (0 == dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING, &ptr)) 
            {
                fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
            }
            else
            {
                DBusPendingCall *pending_return;

                if (0 == dbus_connection_send_with_reply (conn, request, &pending_return, -1))
                {
                    fprintf (stderr, "Error in dbus_connection_send_with_reply\n");
                }
                else 
                {
                    if (NULL == pending_return) 
                    {
                        fprintf (stderr, "pending return is NULL");
                    }
                    else 
                    {
                        dbus_connection_flush (conn);
                            
                        dbus_message_unref (request);	

                        dbus_pending_call_block (pending_return);

                        DBusMessage *reply;
                        reply = dbus_pending_call_steal_reply (pending_return);

                        if (NULL == reply) 
                        {
                            fprintf (stderr, "Error in dbus_pending_call_steal_reply");;
                        }
                        else 
                        {
                            dbus_pending_call_unref	(pending_return);

                            char *s = NULL;

                            if (0 != dbus_message_get_args (reply, &dbus_error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID))
                            {
                                printf ("%s\n", s);
                            }
                            else
                            {
                                fprintf (stderr, "Did not get arguments in reply\n");
                            }

                            dbus_message_unref (reply);	

                            if (-1 == dbus_bus_release_name (conn, CLIENT_BUS_NAME, &dbus_error)) 
                            {
                                fprintf (stderr, "Error in dbus_bus_release_name\n");
                            }
                            else
                            {                               
                                printf ("Please type two numbers: ");
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
