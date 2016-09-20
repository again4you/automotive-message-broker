#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

#include "libamb-client.h"

#define AMB_BUS_NAME        "org.automotive.message.broker"
#define AMB_INTERFACE_NAME  "org.automotive.Manager"
#define DBUS_INTERFACE_NAME "org.freedesktop.DBus.Properties"

/******************************************************************************
 * internal
 *****************************************************************************/

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

static GDBusProxy *find_objects_with_zone(GDBusProxy *proxy, const char *obj_name, int zone)
{
	GError *err;
	GVariant *ret;
	GDBusProxy *objproxy;
	gchar *obj;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
				"FindObjectForZone",
				g_variant_new("(si)", obj_name, zone),
				G_DBUS_PROXY_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	g_variant_get(ret, "(o)", &obj);
	objproxy = get_proxy_from_obj(obj);

	g_variant_unref(ret);

	return objproxy;
}

static GList *find_objects(GDBusProxy *proxy, const char *obj_name)
{
	GError *err;
	GVariant *ret;
	GVariantIter *iter;
	GList *list = NULL;
	const gchar *obj;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
			"FindObject",
			g_variant_new("(s)", obj_name),
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

static GVariant *get_all(GDBusProxy *proxy, const char *name)
{
	gchar *obj_name;
	GError *err;
	GVariant *ret;

	obj_name = g_strdup_printf("org.automotive.%s", name);
	if (!obj_name)
		return NULL;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
			"GetAll",
			g_variant_new("(s)", obj_name),
			G_DBUS_PROXY_FLAGS_NONE,
			-1,
			NULL,
			&err);
	if (!ret) {
		fprintf(stderr, "Error(%s): %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	g_free(obj_name);
	return ret;
}



/******************************************************************************
 * higher APIs
 *****************************************************************************/
GVariant *get_property_all_with_zone(const char *obj_name, int zone)
{
	GDBusProxy *proxy;
	GDBusProxy *objproxy;
	GVariant *ret;

	proxy = get_manager();
	if (!proxy)
		return NULL;

	objproxy = find_objects_with_zone(proxy, obj_name, zone);
	if (!objproxy) {
		g_object_unref(proxy);
		return NULL;
	}

	ret = get_all(objproxy, obj_name);

	g_object_unref(objproxy);
	g_object_unref(proxy);

	return ret;
}

GList *get_property_all(const char *obj_name)
{
	GDBusProxy *proxy;
	GVariant *ret;
	GList *obj;
	GList *objlist;
	GList *retlist;

	proxy = get_manager();
	if (!proxy)
		return NULL;

	objlist = find_objects(proxy, obj_name);
	if (!objlist)
		return NULL;

	retlist = NULL;
	for (obj = objlist; obj != NULL; obj = obj->next) {
		ret = get_all(obj->data, obj_name);
		retlist = g_list_append(retlist, ret);
	}

	return retlist;
}

GList *get_object_list()
{
	GDBusProxy *proxy;
	GError *err;
	GVariant *ret;
	GVariantIter *iter;
	GList *objlist;
	gchar *objname;


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

	objlist = NULL;
	g_variant_get(ret, "(as)", &iter);
	while (g_variant_iter_loop(iter, "s", &objname)) {
		objlist = g_list_append(objlist, g_strdup(objname));
	}

	g_variant_iter_free(iter);
	g_variant_unref(ret);
	g_object_unref(proxy);

	return objlist;
}
