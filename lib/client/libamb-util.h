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

#ifndef __LIBAMB_UTIL_H__
#define __LIBAMB_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <errno.h>

#include "libamb-common.h"
#include "libamb-client.h"

#define g_variant_get_guchar g_variant_get_byte
#define g_variant_get_gint16 g_variant_get_int16
#define g_variant_get_guint16 g_variant_get_uint16
#define g_variant_get_gint32 g_variant_get_int32
#define g_variant_get_guint32 g_variant_get_uint32
#define g_variant_get_gint64 g_variant_get_int64
#define g_variant_get_guint64 g_variant_get_uint64
#define g_variant_get_gdouble g_variant_get_double

/**
 * Generate Custom CAN Object data structure and its related utility function.
 *
 * @param[in] CAN Object name
 * @param[in] data type for CAN object
 * @param[in] Alias name that is used in AMB. If it is NULL, then this value
 * is ignored.
 *
 * @see amb_release_data()
 */
#define CAN_OBJECT(obj_name, value_type, alias_name) \
	struct obj_name ## Type { \
		gint32	Zone; \
		gint32	ValueSequence; \
		gdouble Time; \
		value_type Value; \
	}; \
	int amb_get_ ## obj_name ## _with_zone(struct obj_name ## Type **retobj, int zone) \
	{ \
		GVariant *variant; \
		GVariantIter *iter; \
		gchar *key; \
		GVariant *value; \
		GVariant *tmp; \
		int ret; \
		struct obj_name ## Type *retdata = NULL; \
		\
		retdata = g_malloc0(sizeof(*retdata)); \
		if (!retdata) { \
			return -ENOMEM; \
		} \
		ret = amb_get_property_all_with_zone(&variant, #obj_name, zone); \
		if (ret < 0) { \
			return ret; \
		} \
		\
		g_variant_get(variant, "(a{sv})", &iter); \
		while(g_variant_iter_loop(iter, "{sv}", &key, &value)) { \
			if (!g_strcmp0(key, "Zone")) { \
				g_variant_get(value, "v", &tmp); \
				retdata->Zone = g_variant_get_int32(tmp); \
				g_variant_unref(tmp); \
			} else if (!g_strcmp0(key, "ValueSequence") || !g_strcmp0(key, #alias_name "Sequence")) { \
				g_variant_get(value, "v", &tmp); \
				retdata->ValueSequence = g_variant_get_int32(tmp); \
				g_variant_unref(tmp); \
			} else if (!g_strcmp0(key, "Time")) { \
				g_variant_get(value, "v", &tmp); \
				retdata->Time = g_variant_get_double(tmp); \
				g_variant_unref(tmp); \
			} else if (!g_strcmp0(key, "Value") || !g_strcmp0(key, #alias_name)) { \
				g_variant_get(value, "v", &tmp); \
				retdata->Value = g_variant_get_ ## value_type(tmp); \
				g_variant_unref(tmp); \
			} \
		} \
		\
		g_variant_iter_free(iter); \
		amb_release_property_all_with_zone(variant); \
		*retobj = retdata; \
		return 0; \
	} \
	\
	int amb_convert_ ## obj_name ## Type (gpointer data, struct obj_name ## Type *retdata) \
	{ \
		GVariantIter *iter; \
		gchar *key; \
		GVariant *value; \
		GVariant *gdata; \
		\
		gdata = (GVariant *)data; \
		g_variant_get(gdata, "a{sv}", &iter); \
		while(g_variant_iter_loop(iter, "{sv}", &key, &value)) { \
			if (!g_strcmp0(key, "Zone")) { \
				g_variant_get(value, "i", &retdata->Zone); \
			} else if (!g_strcmp0(key, "ValueSequence") || !g_strcmp0(key, #alias_name "Sequence")) { \
				g_variant_get(value, "i", &retdata->ValueSequence); \
			} else if (!g_strcmp0(key, "Time")) { \
				g_variant_get(value, "d", &retdata->Time); \
			} else if (!g_strcmp0(key, "Value") || !g_strcmp0(key, #alias_name)) { \
				retdata->Value = g_variant_get_ ## value_type(value); \
			} \
		} \
		g_variant_iter_free(iter); \
		return 0; \
	}

#ifdef __cplusplus
}
#endif

#endif /* __LIBAMB_UTIL_H__ */
