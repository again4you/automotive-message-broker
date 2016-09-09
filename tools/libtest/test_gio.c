#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

/************************************************************************/
#define AMB_BUS_NAME		"org.automotive.message.broker"
#define AMB_INTERFACE_NAME	"org.automotive.Manager"
#define DBUS_INTERFACE_NAME	"org.freedesktop.DBus.Properties"



static GDBusProxy *get_proxy_from_obj(const char *objpath);

/************************************************************************/
/* public api */

static void get_object_list(GDBusProxy *proxy)
{
	GError *err;
	GVariant *ret;
	gchar *s;

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
		return ;
	}

	// TODO Need to export
	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_variant_unref(ret);
	g_free(s);
}

static void get_property_all(GDBusProxy *proxy, const char *name)
{
	GError *err;
	GVariant *ret;
	gchar *s;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy, 
				"GetAll",
				g_variant_new("(s)", name),
				G_DBUS_PROXY_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
		g_clear_error(&err);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);
	g_variant_unref(ret);
	return ;
}

// TODO need to reture value
static void get_property_one(GDBusProxy *proxy, const char *obj_name, const char *property_name)
{
	GError *err;
	GVariant *ret;
	gchar *s;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy, 
				"Get",
				g_variant_new("(ss)", obj_name, property_name),
				G_DBUS_PROXY_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
		g_clear_error(&err);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);
	g_variant_unref(ret);
	return ;
}

/************************************************************************/
/* private */

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

static GDBusProxy *get_proxy_from_obj(const char *objpath)
{
	GError *err;
	GDBusProxy *proxy;

	err = NULL;
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
					G_DBUS_PROXY_FLAGS_NONE,
					NULL,
					AMB_BUS_NAME,
					objpath,
					DBUS_INTERFACE_NAME,
					NULL,
					&err);
	if (!proxy) {
		fprintf(stderr, "Error: %s(): %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	return proxy;
}

static GList *find_objects(GDBusProxy *proxy, const char *name)
{
	GError *err;
	GVariant *ret;
	GVariantIter *iter;
	GList *list;
	const gchar *obj;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
				"FindObject",
				g_variant_new("(s)", name),
				G_DBUS_PROXY_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	g_variant_get(ret, "(ao)", &iter);
	while (g_variant_iter_loop(iter, "o", &obj)) {
		GDBusProxy *objproxy;

		objproxy = get_proxy_from_obj(obj);
		if (objproxy)
			list = g_list_append(list, objproxy);
	}

	g_variant_iter_free(iter);
	g_variant_unref(ret);

	return list;
}

/************************************************************************/

int main()
{
	GDBusProxy *mgr;
	GList *objlist;
	GList *l;

	mgr = get_manager();
	if (!mgr)
		return 1;

	// 1. show all object list
	get_object_list(mgr);

	// 2. show all properties for specific object
	objlist = find_objects(mgr, "VehicleSpeed");
	if (!objlist)
		return 1;

	for (l = objlist; l != NULL; l = l->next) {
		get_property_all(l->data, "org.automotive.VehicleSpeed");
		get_property_one(l->data, "org.automotive.VehicleSpeed", "Speed");
	}

	

	g_object_unref(mgr);
	return 0;
}
