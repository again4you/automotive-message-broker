#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

#include "libdbus-client.h"

#define AMB_BUS_NAME        "org.automotive.message.broker"
#define AMB_INTERFACE_NAME  "org.automotive.Manager"
#define DBUS_INTERFACE_NAME "org.freedesktop.DBus.Properties"

static GDBusProxy *get_manager()
{
		GError *err;
		GDBusProxy *proxy;

		err = NULL;
		proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						NULL,
						AMB_BUS_NAME,
						"/",
						AMB_INTERFACE_NAME,
						NULL,
						&err);
		if (!proxy) {
				fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
				g_clear_error(&err);
				return NULL;
		}
		return proxy;
}


GVariant *get_object_list()
{
		GDBusProxy *proxy;
		GError *err;
		GVariant *ret;
		gchar *s;

		proxy = get_manager();
		if (!proxy)
				return NULL;

		err = NULL;
		ret = g_dbus_proxy_call_sync(proxy,
					"List",
					NULL,
					G_DBUS_PROXY_FLAGS_NONE,
					-1,
					NULL,
					&err);

		if (!ret) {
				fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
				g_clear_error(&err);
				return NULL;
		}

		s = g_variant_print(ret, TRUE);
		printf("%s\n", s);

		g_free(s);
		g_object_unref(proxy);

		return ret;
}
