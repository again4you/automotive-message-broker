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
#include "debugout.h"

#ifndef EXPORT
#  define EXPORT __attribute__((visibility("default")))
#endif


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
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
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
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
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
				G_DBUS_CALL_FLAGS_NONE,
				-1,
				NULL,
				&err);
	if (!ret) {
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
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
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);
	if (!ret) {
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
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
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);
	if (!ret) {
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
		g_clear_error(&err);
		return NULL;
	}

	g_free(obj_name);
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
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
		g_free(obj_name);
		g_clear_error(&err);
		return -1;
	}

	g_variant_unref(ret);
	g_free(obj_name);
	return 0;
}

/******************************************************************************
 * higher APIs
 *****************************************************************************/

EXPORT int set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value)
{
	int ret;
	GDBusProxy *proxy;
	GDBusProxy *objproxy;

	proxy = get_manager();
	if (!proxy)
		return -1;

	objproxy = find_objects_with_zone(proxy, obj_name, zone);
	if (!objproxy) {
		g_object_unref(proxy);
		return -1;
	}

	ret = set_prop(objproxy, obj_name, prop_name, value);
	if (ret != 0) {
		g_object_unref(objproxy);
		g_object_unref(proxy);
		return ret;
	}

	g_object_unref(objproxy);
	g_object_unref(proxy);

	return 0;
}

EXPORT int get_property_all_with_zone(GVariant **proplist, const char *obj_name, int zone)
{
	GDBusProxy *proxy;
	GDBusProxy *objproxy;

	proxy = get_manager();
	if (!proxy)
		return -1;

	objproxy = find_objects_with_zone(proxy, obj_name, zone);
	if (!objproxy) {
		g_object_unref(proxy);
		return -1;
	}

	*proplist = get_all(objproxy, obj_name);

	g_object_unref(objproxy);
	g_object_unref(proxy);

	return 0;
}

EXPORT int get_property_all(GList **proplist, const char *obj_name)
{
	GDBusProxy *proxy;
	GVariant *ret;
	GList *obj;
	GList *objlist;

	proxy = get_manager();
	if (!proxy)
		return -1;

	objlist = find_objects(proxy, obj_name);
	if (!objlist)
		return -1;

	*proplist = NULL;
	for (obj = objlist; obj != NULL; obj = obj->next) {
		ret = get_all(static_cast<GDBusProxy*>(obj->data), obj_name);
		*proplist = g_list_append(*proplist, ret);
	}

	return 0;
}

EXPORT int get_object_list(GList **objlist)
{
	GDBusProxy *proxy;
	GError *err;
	GVariant *ret;
	GVariantIter *iter;
	gchar *objname;

	proxy = get_manager();
	if (!proxy)
		return -1;

	err = NULL;
	ret = g_dbus_proxy_call_sync(proxy,
			"List",
			NULL,
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);

	if (!ret) {
		DebugOut(DebugOut::Error) << __func__ << ": " << err->message;
		g_clear_error(&err);
		return -1;
	}

	*objlist = NULL;
	g_variant_get(ret, "(as)", &iter);
	while (g_variant_iter_loop(iter, "s", &objname)) {
		*objlist = g_list_append(*objlist, g_strdup(objname));
	}

	g_variant_iter_free(iter);
	g_variant_unref(ret);
	g_object_unref(proxy);

	return 0;
}
