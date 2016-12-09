#include <stdio.h>
#include <errno.h>

#include <glib.h>
#include <gio/gio.h>

#define INTERFACE_NAME  "com.samsung.test"
#define OBJ_PATH        "/Com/Samsung/Test"
#define SIGNAL_PRINT    "Print"

#define DEBUGOUT(fmt, ...) do { fprintf(stderr, fmt, ##__VA_ARGS__); } while(0);

GMainLoop *loop = NULL;
static guint signalid;
static GDBusConnection* conn = NULL;

static void signal_handler(GDBusConnection *connection,
        const gchar *sender_name,
        const gchar *object_path,
        const gchar *interface_name,
        const gchar *signal_name,
        GVariant *parameters,
        gpointer user_data)
{
    DEBUGOUT("DBus Signal catch: %s\n", signal_name);
}

static int subscribe_dbus_signal()
{
    GError *error;

    DEBUGOUT("%s: Enter subscribe_dbus_signal()\n", __func__);

    error = NULL;
    conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (conn == NULL) {
        DEBUGOUT("%s: %s\n", __func__, error->message);
        return -1;
    }

    signalid = g_dbus_connection_signal_subscribe(conn,
            NULL,
            INTERFACE_NAME,
            SIGNAL_PRINT,
            OBJ_PATH,
            NULL,
            G_DBUS_SIGNAL_FLAGS_NONE,
            signal_handler,
            NULL,
            NULL);

    if (!signalid) {
        DEBUGOUT("%s: Fail to g_dbus_connection_signal_subscribe()\n", __func__);
        g_object_unref(conn);
        return -1;
    }
    return 0;
}

int main()
{
    loop = g_main_loop_new (NULL, TRUE);
    if (!loop)
        return -ENOMEM;

    if (subscribe_dbus_signal())
        return -1;

    g_main_loop_run(loop);

    g_main_loop_unref(loop);

    return 0;
}
