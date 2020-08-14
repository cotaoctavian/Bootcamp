/**************************************************************
 *                      INCLUDES                              *
 **************************************************************/

#include <stdio.h>
#include <dbus/dbus.h>
#include <string.h>

/**************************************************************
 *                 GLOBAL VARIABLES                           *
 **************************************************************/

const char *PATH = "/org/share/linux";
const char *INTERFACE = "org.share.linux";
const char *CONFIG = "Config";
const char *QUIT = "Quit";

/**************************************************************
 *              STATIC FUNCTIONS DECLARATION                  *
 **************************************************************/

static void send_quit(DBusConnection *connection);
static void send_config(DBusConnection *connection);

/**************************************************************
 *               STATIC FUNCTIONS DEFINITION                  *
 **************************************************************/

static void send_quit(DBusConnection *connection) 
{
    DBusMessage *message;
    
    /* Create a new signal */
    message = dbus_message_new_signal(PATH, INTERFACE, QUIT);

    /* Send the signal */
    dbus_connection_send(connection, message, NULL);

    /* Decrements the reference count of a DBusMessage */
    dbus_message_unref(message);
}

static void send_config(DBusConnection *connection) 
{
    DBusMessage *message;
    
    /* Create a new signal */
    message = dbus_message_new_signal(PATH, INTERFACE, CONFIG);

    /* Send the signal */
    dbus_connection_send(connection, message, NULL);

    /* Decrements the reference count of a DBusMessage */
    dbus_message_unref(message);
}

int main(int argc, char **argv) 
{
    DBusConnection *connection;
    DBusError error;

    dbus_error_init(&error);

    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (0 != dbus_error_is_set(&error)) 
    {
        fprintf (stderr, "Failed to connect to the D-BUS daemon: %s", error.message);
        
        /* Free the allocated memory for DBusError. */
        dbus_error_free(&error);
    }
    else if (1 == argc) 
    {
        printf("Only one parameter passed. Try again.\n");
    }
    else 
    {
        int i = 1;
        
        for (i = 1; i < argc; i++)
        {
            if (0 == strcmp(argv[i], "-c"))
            {
                send_config(connection);
            }
            else if (0 == strcmp(argv[i], "-q"))
            {
                send_quit(connection);
            }
        }
    }
}
