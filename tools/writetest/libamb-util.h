
#ifndef __LIBAMB_UTIL_H__
#define __LIBAMB_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>

#include "libamb-client.h"

guchar get_guchar(GVariant *value);
guint16 get_guint16(GVariant *value);
gint32 get_gint32(GVariant *value);
guint32 get_guint32(GVariant *value);
gdouble get_gdouble(GVariant * value);
void amb_free_result(void *retdata);

#define g_variant_new_gboolean g_variant_new_boolean
#define g_variant_new_guchar g_variant_new_byte
#define g_variant_new_gint16 g_variant_new_int16
#define g_variant_new_guint16 g_variant_new_uint16
#define g_variant_new_gint32 g_variant_new_int32
#define g_variant_new_guint32 g_variant_new_uint32
#define g_variant_new_gint64 g_variant_new_int64
#define g_variant_new_guint64 g_variant_new_uint64
#define g_variant_new_gdouble g_variant_new_double

#define CAN_OBJECT_WRITE(obj_name, value_type, alias_name) \
	int amb_set_ ## obj_name ## _with_zone(value_type value, int zone) \
	{ \
		GVariant *variant; \
		int ret; \
		variant = g_variant_new_ ## value_type(value); \
		ret = amb_set_property(#obj_name, \
				g_strcmp0(#alias_name, "NULL") ? #alias_name : "Value", \
				zone, variant);\
		if (ret < 0) { \
			fprintf(stderr, "Fail to amb_set_property: %d\n", ret); \
			return ret; \
		} \
		\
		return 0; \
	}

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
		int ret; \
		struct obj_name ## Type *retdata = NULL; \
		\
		ret = amb_get_property_all_with_zone(&variant, #obj_name, zone); \
		if (ret < 0) { \
			return ret; \
		} \
		\
		retdata = g_malloc0(sizeof(*retdata)); \
		if (!retdata) { \
			return -1; \
		} \
		\
		g_variant_get(variant, "(a{sv})", &iter); \
		while(g_variant_iter_loop(iter, "{sv}", &key, &value)) { \
			if (!g_strcmp0(key, "Zone")) { \
				retdata->Zone = get_gint32(value); \
			} else if (!g_strcmp0(key, "ValueSequence") || !g_strcmp0(key, #alias_name "Sequence")) { \
				retdata->ValueSequence = get_gint32(value); \
			} else if (!g_strcmp0(key, "Time")) { \
				retdata->Time = get_gdouble(value); \
			} else if (!g_strcmp0(key, "Value") || !g_strcmp0(key, #alias_name)) { \
				retdata->Value = get_ ## value_type(value); \
			} \
		} \
		\
		g_variant_iter_free(iter); \
		amb_release_property_all_with_zone(variant); \
		*retobj = retdata; \
		return 0; \
	}

#ifdef __cplusplus
}
#endif

#endif /* __LIBAMB_UTIL_H__ */
