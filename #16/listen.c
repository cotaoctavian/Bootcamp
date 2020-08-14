/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <glib.h>

/** 
 * gcc listen.c -o listen `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`
 */

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

static const char *RULE = "type='signal', interface='org.share.linux'";
static const char *INTERFACE = "org.share.linux";

/**************************************************************
 *              STATIC FUNCTIONS DECLARATION                  *
 **************************************************************/

static DBusHandlerResult dbus_filter (DBusConnection *connection, DBusMessage *message, void *user_data);

/**************************************************************
 *               STATIC FUNCTIONS DEFINITION                  *
 **************************************************************/

static DBusHandlerResult dbus_filter (DBusConnection *connection, DBusMessage *message, void *user_data) 
{
    /* Check if we got a message */
    if (0 != dbus_message_is_signal(message, INTERFACE, "Config"))
    {   
        printf("Customize option received.\n");
    }
    else if (0 != dbus_message_is_signal(message, INTERFACE, "Quit"))
    {
        printf("Quit option received.\n");

        GMainLoop *loop = (GMainLoop*) user_data;
        g_main_loop_quit(loop);
    }

    return DBUS_HANDLER_RESULT_HANDLED;
}

int main() 
{
    DBusConnection *connection = NULL;
    DBusError error;
    GMainLoop *loop;

    /* Create a new event loop */
    loop = g_main_loop_new(NULL, FALSE);

    /* Initialize DBusError variable */
    dbus_error_init(&error);

    /* Initialize DBus connection */
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (0 != dbus_error_is_set(&error)) 
    {
        fprintf(stderr, "%s", error.message);

        /* Free the allocated memory for DBusError. */
        dbus_error_free(&error);
    }

    /* Add match (get message from a specific interface). */
    dbus_bus_add_match(connection, RULE, NULL);

    /* Create a handle for all the incoming messages. */
    dbus_connection_add_filter (connection, dbus_filter, loop, NULL);

    /* Dbus-glib call that integrates the glib main loop & DBus bus events. */
    dbus_connection_setup_with_g_main(connection, NULL);

    /* Run glib main loop */
    g_main_loop_run(loop);

    return 0;
}