/*
 * Automotive Message Broker Client Library
 *
 * Copyright (C) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License)
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include <errno.h>

#include "libamb-client.h"

#ifndef EXPORT
#  define EXPORT __attribute__((visibility("default")))
#endif

#if defined DEBUG
#  define DEBUGOUT(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); } while(0);
#else
#  define DEBUGOUT(fmt,...)
#endif /* DEBUG */


#define AMB_BUS_NAME        "org.automotive.message.broker"
#define AMB_INTERFACE_NAME  "org.automotive.Manager"
#define DBUS_INTERFACE_NAME "org.freedesktop.DBus.Properties"

struct signal_item {
	guint id;
	GDBusProxy *obj;
};

/******************************************************************************
 * internal
 *****************************************************************************/
static GDBusProxy *get_manager()
{
	static GDBusProxy *proxy = NULL;

	if(!proxy) {
		GError *err = NULL;
		proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
				G_DBUS_PROXY_FLAGS_NONE,
				NULL,
				AMB_BUS_NAME,
				"/",
				AMB_INTERFACE_NAME,
				NULL,
				&err);
		if (!proxy) {
			DEBUGOUT("%s: %s\n", __func__, err->message);
			g_clear_error(&err);
			return NULL;
		}
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
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	return proxy;
}

static GDBusProxy *find_objects_with_zone(GDBusProxy *proxy, const char *obj_name, ZoneType zone)
{
	GError *err;
	GVariant *ret;
	GDBusProxy *objproxy;
	gchar *obj;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
				"FindObjectForZone",
				g_variant_new("(si)", obj_name, zone),
				G_DBUS_CALL_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	g_variant_get(ret, "(&o)", &obj);
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
	gchar *obj;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
			"FindObject",
			g_variant_new("(s)", obj_name),
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);
	if (!ret) {
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	g_variant_get(ret, "(ao)", &iter);
	while (g_variant_iter_loop(iter, "&o", &obj)) {
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
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);
	g_free(obj_name);
	if (!ret) {
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_clear_error(&err);
		return NULL;
	}

	return ret;
}

static int set_prop(GDBusProxy *proxy, const char *name, const char *prop_name, GVariant *value)
{
	GError *err;
	gchar *obj_name;
	GVariant *ret;

	obj_name = g_strdup_printf("org.automotive.%s", name);
	if (!obj_name)
		return -ENOMEM;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
				"Set",
				g_variant_new("(ssv)", obj_name, prop_name, value),
				G_DBUS_CALL_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_free(obj_name);
		g_clear_error(&err);
		return -EINVAL;
	}

	g_variant_unref(ret);
	g_free(obj_name);
	return 0;
}

static GHashTable *get_htable()
{
	static GHashTable *htable = NULL;
	if (!htable) {
		htable = g_hash_table_new(g_str_hash, g_str_equal);
	}
	return htable;
}

static void on_signal_handler(GDBusProxy *proxy,
			gchar *sender_name,
			gchar *signal_name,
			GVariant *parameters,
			gpointer user_callback)
{
	gchar *obj_name;
	GVariant *value;
	AMB_PROPERTY_CHANGED_CALLBACK callback = (AMB_PROPERTY_CHANGED_CALLBACK)user_callback;

	if (g_strcmp0("PropertiesChanged", signal_name)) {
		DEBUGOUT("Error: signal name: %s\n", signal_name);
		return ;
	}

	g_variant_get(parameters, "(s@a{sv}as)", &obj_name, &value, NULL);
	if (callback)
		callback(obj_name, value);

	g_free(obj_name);
	g_variant_unref(value);

	return ;
}

/******************************************************************************
 * higher APIs
 *****************************************************************************/
EXPORT int amb_set_property(const char *obj_name, const char *prop_name, ZoneType zone, GVariant *value)
{
	int ret;
	GDBusProxy *proxy;
	GDBusProxy *objproxy;

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	objproxy = find_objects_with_zone(proxy, obj_name, zone);
	if (!objproxy) {
		return -EINVAL;
	}

	ret = set_prop(objproxy, obj_name, prop_name, value);
	if (ret != 0) {
		g_object_unref(objproxy);
		return ret;
	}

	g_object_unref(objproxy);

	return 0;
}

EXPORT int amb_get_property_all_with_zone(GVariant **proplist, const char *obj_name, ZoneType zone)
{
	GDBusProxy *proxy;
	GDBusProxy *objproxy;

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	objproxy = find_objects_with_zone(proxy, obj_name, zone);
	if (!objproxy) {
		fprintf(stderr, "%s: obj_name: %s, zone: %d\n", __func__, obj_name, zone);
		return -EINVAL;
	}

	*proplist = get_all(objproxy, obj_name);

	g_object_unref(objproxy);

	return 0;
}

EXPORT int amb_get_property_all(GList **proplist, const char *obj_name)
{
	GDBusProxy *proxy;
	GVariant *ret;
	GList *obj;
	GList *objlist;

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	objlist = find_objects(proxy, obj_name);
	if (!objlist) {
		return -EINVAL;
	}

	*proplist = NULL;
	for (obj = objlist; obj != NULL; obj = obj->next) {
		ret = get_all(obj->data, obj_name);
		*proplist = g_list_append(*proplist, ret);
	}

	amb_release_property_all(objlist);

	return 0;
}

EXPORT int amb_get_object_list(GList **objlist)
{
	GDBusProxy *proxy;
	GError *err;
	GVariant *ret;
	GVariantIter *iter;
	gchar *objname;

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
			"List",
			NULL,
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);

	if (!ret) {
		DEBUGOUT("%s: %s\n", __func__, err->message);
		g_clear_error(&err);
		return -ENETUNREACH;
	}

	*objlist = NULL;
	g_variant_get(ret, "(as)", &iter);
	while (g_variant_iter_loop(iter, "&s", &objname)) {
		*objlist = g_list_append(*objlist, g_strdup(objname));
	}

	g_variant_iter_free(iter);
	g_variant_unref(ret);

	return 0;
}

EXPORT void amb_release_property_all_with_zone(GVariant *proplist)
{
	g_variant_unref(proplist);
}

EXPORT void amb_release_object_list(GList *objlist)
{
	g_list_free_full(objlist, g_free);
}

EXPORT void amb_release_property_all(GList *proplist)
{
	g_list_free_full(proplist, (GDestroyNotify)g_variant_unref);
}

EXPORT int amb_register_property_changed_handler(gchar *objname,
				ZoneType zone,
				AMB_PROPERTY_CHANGED_CALLBACK callback)
{
	GDBusProxy *proxy;
	GDBusProxy *objproxy;
	guint id;
	struct signal_item *item;
	GHashTable *htable;

	htable = get_htable();
	if (!htable) {
		DEBUGOUT("Error: get_htable() returns NULL\n");
		return -ENOMEM;
	}

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	objproxy = find_objects_with_zone(proxy, objname, zone);
	if (!objproxy) {
		DEBUGOUT("Error: find_objects_with_zone() Object Name: %s, ZoneType: %d\n",
				objname, zone);
		return -EINVAL;
	}

	item = g_new0(struct signal_item, 1);
	if (!item) {
		DEBUGOUT("Error: fail to g_new0()\n");
		g_object_unref(objproxy);
		return -ENOMEM;
	}

	id = g_signal_connect(objproxy, "g-signal", G_CALLBACK(on_signal_handler), (gpointer)callback);
	item->id = id;
	item->obj = objproxy;

	g_hash_table_insert(htable,
			g_strdup(g_dbus_proxy_get_object_path(objproxy)),
			item);

	DEBUGOUT("instance: %s ID: %u\n", g_dbus_proxy_get_object_path(objproxy), id);

	return 0;
}

EXPORT int amb_unregister_property_changed_handler(gchar *objname, ZoneType zone)
{
	GHashTable *htable;
	GDBusProxy *proxy;
	GDBusProxy *objproxy;
	gpointer key;
	struct signal_item *item;
	gchar *objpath;

	htable = get_htable();
	if (!htable) {
		DEBUGOUT("Error: get_htable() returns NULL\n");
		return -ENOMEM;
	}

	proxy = get_manager();
	if (!proxy)
		return -ENETUNREACH;

	objproxy = find_objects_with_zone(proxy, objname, zone);
	if (!objproxy) {
		DEBUGOUT("Error: find_objects_with_zone() Object Name: %s, ZoneType: %d\n",
				objname, zone);
		return -EINVAL;
	}

	objpath = (gchar*)g_dbus_proxy_get_object_path(objproxy);

	if (!g_hash_table_lookup_extended(htable, objpath, &key, (gpointer*)&item)) {
		DEBUGOUT("Error: fail to find the object :%s\n", objpath);
		g_object_unref(objproxy);
		return -EINVAL;
	}

	DEBUGOUT("instance: %s ID: %u\n", objpath, item->id);

	g_signal_handler_disconnect(item->obj, item->id);
	if (!g_hash_table_remove(htable, objpath)) {
		DEBUGOUT("Error: fail to g_hash_table_remove()\n");
	}

	g_free(key);
	g_free(item);

	return 0;
}
