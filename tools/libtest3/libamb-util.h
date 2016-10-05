
#ifndef __LIBAMB_UTIL_H__
#define __LIBAMB_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>

#include "libamb-client.h"

guchar get_guchar(GVariant *value);
guint16 get_guint16(GVariant *value);
gint32 get_gint32(GVariant *value);
guint32 get_guint32(GVariant *value);
gdouble get_gdouble(GVariant * value);
void amb_free_result(void *retdata);

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
			} else if (!g_strcmp0(key, "ValueSequence")) { \
				retdata->ValueSequence = get_gint32(value); \
			} else if (!g_strcmp0(key, "Time")) { \
				retdata->Time = get_gdouble(value); \
			} else if (!g_strcmp0(key, "Value")) { \
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
