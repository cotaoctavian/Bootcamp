#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <glib.h>
 
/**
 * HOW TO COMPILE: gcc main.c -o main `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`
 * This problem presents how to set a name to a dbus app.
 */ 

int main() {

    /* DBusConnection used to create a connection */
    DBusConnection *connection = NULL;
    DBusError error;
    char *dbus_name            = "org.communcation.app";
    int request_name_reply     = 0;

    /* Initialize errors */
    dbus_error_init(&error);

    /* Initialize the connection */
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    /* Check if everything went fine while creating the connection*/
    if (0 != dbus_error_is_set(&error)) 
    {   
        fprintf(stderr, "%s", error.message);

        /* Free the allocated memory for DBusError */
        dbus_error_free(&error);
    }
    else 
    {   
        /* Check if the dbus_name we want is available for our DBus app */
        dbus_bool_t ret = dbus_bus_name_has_owner(connection, dbus_name, &error);

        /* Check if everything went fine while checking if the name is available s*/
        if (0 != dbus_error_is_set(&error)) 
        {   
            fprintf(stderr, "%s", error.message);

            /* Free the allocated memory for DBusError */
            dbus_error_free(&error);
        }
        
        /* If ret is false it means that it doesn't have an owner => the dbus_name is available */
        if (FALSE == ret) 
        {
            printf("Reserving the dbus_name %s.\n", dbus_name);

            request_name_reply = dbus_bus_request_name(connection, dbus_name, DBUS_NAME_FLAG_DO_NOT_QUEUE, &error);

            /* Check if everything went fine while reserving a dbus_name */
            if (0 != dbus_error_is_set(&error)) 
            {   
                fprintf(stderr, "%s", error.message);

                /* Free the allocated memory for DBusError */
                dbus_error_free(&error);
            }

            /* Check if we are the owner */
            if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER == request_name_reply) 
            {
                printf("Bus name %s successfully reserved.\n", dbus_name);
                
            }
        }
    }

    return 0;
}