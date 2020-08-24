/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <dbus/dbus.h>

/*
gcc server.c -o server `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`
*/

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

const char *const INTERFACE_NAME = "org.linux.dbus_example";
const char *const SERVER_BUS_NAME = "org.linux.server";
const char *const OBJECT_PATH_NAME = "/org/linux/adder";
const char *const METHOD_NAME = "add_numbers";
DBusError dbus_error;

/**************************************************************
 *              STATIC FUNCTIONS DECLARATION                  *
 **************************************************************/

/**
 * @brief     This function prints out the raised error.
 * @param[in] str - Message for the raise error.
 */
static void print_dbus_error (char *str);

/**
 * @brief     This functions verifies if a string is an integer.
 * @param[in] ptr - is the string that is going to be checked if it's an integer or not.
 * @return    true / false
 */
static bool isinteger (char *ptr);

/**************************************************************
 *              STATIC FUNCTIONS DEFINITION                   *
 **************************************************************/

static bool isinteger (char *ptr)
{   
    bool result = true;

    if ('+' == *ptr|| '-' == *ptr)
    {
        ptr++;
    }

    while (0 != *ptr) 
    {
        if (false == isdigit ((int) *ptr++))
        {
            result = false;
        }
    }
    
    return result;
}

static void print_dbus_error (char *str) 
{
    fprintf (stderr, "%s: %s\n", str, dbus_error.message);
    dbus_error_free (&dbus_error);
}

int main (int argc, char **argv)
{
    DBusConnection *conn;
    int ret;

    dbus_error_init (&dbus_error);

    conn = dbus_bus_get (DBUS_BUS_SESSION, &dbus_error);

    if (0 != dbus_error_is_set (&dbus_error))
    {
        print_dbus_error ("dbus_bus_get");
    }

    // Get a well known name
    ret = dbus_bus_request_name (conn, SERVER_BUS_NAME, DBUS_NAME_FLAG_DO_NOT_QUEUE, &dbus_error);

    if (0 != dbus_error_is_set (&dbus_error))
    {
        print_dbus_error ("dbus_bus_request_name");
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) 
    {
        fprintf (stderr, "Dbus: not primary owner, ret = %d\n", ret);
    }
    else 
    {
        while (1) 
        {
            // Block for msg from client
            if (0 == dbus_connection_read_write_dispatch (conn, -1)) {
                fprintf (stderr, "Not connected now.\n");
            }
            else
            {
                DBusMessage *message;
                message = dbus_connection_pop_message (conn);

                if (NULL == message) 
                {
                    fprintf (stderr, "Did not get message\n");
                } 
                else
                {
                    if (0 != dbus_message_is_method_call (message, INTERFACE_NAME, METHOD_NAME)) 
                    {
                        char *s;
                        char *str1 = NULL, *str2 = NULL;
                        const char space [4] = " \n\t";
                        long i, j;
                        bool error = false;

                        if (0 != dbus_message_get_args (message, &dbus_error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) 
                        {
                            printf ("%s", s);

                            // Validate received message
                            str1 = strtok (s, space);

                            if (0 != str1)
                            {
                                str2 = strtok (NULL, space);
                            }
            
                            if (0 == str1 || 0 == str2)
                            {
                                error = true; 
                            }
                        
                            if (0 == error)
                            {
                                if (true == isinteger (str1))
                                {
                                    i = atol (str1);
                                }
                                else
                                {
                                    error = true;
                                }
                            }

                            if (0 == error)
                            {
                                if (true == isinteger (str2))
                                {
                                    j = atol (str2);
                                }
                                else
                                {
                                    error = true;
                                }
                            }

                            if (0 == error) 
                            {
                                // send reply
                                DBusMessage *reply;
                                char answer [40];

                                reply = dbus_message_new_method_return (message);
                                sprintf (answer, "Sum is %ld", i + j);

                                if (NULL == reply) 
                                {
                                    fprintf (stderr, "Error in dbus_message_new_method_return\n");
                                }
                                else 
                                {
                                    DBusMessageIter iter;
                                    dbus_message_iter_init_append (reply, &iter);
                                    char *ptr = answer;

                                    if (0 == dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING, &ptr)) 
                                    {
                                        fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
                                    }
                                    else 
                                    {
                                        if (0 == dbus_connection_send (conn, reply, NULL)) 
                                        {
                                            fprintf (stderr, "Error in dbus_connection_send\n");
                                        }
                                        else 
                                        {
                                            dbus_connection_flush (conn);
                                            dbus_message_unref (reply);	
                                        }
                                    }
                                }
                            }
                            else
                            {
                                DBusMessage *dbus_error_msg;
                                char error_msg [] = "Error in input";
                                dbus_error_msg = dbus_message_new_error (message, DBUS_ERROR_FAILED, error_msg);

                                if (NULL == dbus_error_msg) 
                                {
                                    fprintf (stderr, "Error in dbus_message_new_error\n");
                                }
                                else
                                {
                                    if (0 == dbus_connection_send (conn, dbus_error_msg, NULL)) 
                                    {
                                        fprintf (stderr, "Error in dbus_connection_send\n");
                                    }
                                    else
                                    {                            
                                        dbus_connection_flush (conn);
                                        dbus_message_unref (dbus_error_msg);
                                    }
        
                                }
                            }
                        }
                        else
                        {
                            print_dbus_error ("Error getting message");
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}
