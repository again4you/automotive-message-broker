
#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>

#include "libsamsung-can.h"
#include "libamb-client.h"

guchar get_guchar(GVariant *value)
{
	guchar ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "y", &ret);

	g_variant_unref(tmp);
	return ret;
}

guint16 get_guint16(GVariant *value)
{
	guint16 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "q", &ret);

	g_variant_unref(tmp);
	return ret;
}

gint32 get_gint32(GVariant *value)
{
	gint32 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "i", &ret);

	g_variant_unref(tmp);
	return ret;
}

guint32 get_guint32(GVariant *value)                                               
{
	guint32 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "u", &ret);

	g_variant_unref(tmp);
	return ret;
}

gdouble get_gdouble(GVariant * value)                                              
{                                                                                  
	gdouble ret;
	GVariant * tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "d", &ret);

	g_variant_unref(tmp);
	return ret;
}


/************************************************************************/

int get_VehicleOdometer_with_zone(struct VehicleOdometer_t **vo, int zone)
{
	GVariant *variant;
	GVariantIter *iter;
	gchar *key;
	GVariant *value;
	int ret;
	struct VehicleOdometer_t *retdata = NULL;

	// get data
	ret = amb_get_property_all_with_zone(&variant, "VehicleOdometer", zone);
	if (ret < 0) {
		fprintf(stderr, "Fail to amb_get_property_all_with_zone(): VehicleOdometer\n");
		return -1;
	}

	// allocation memory
	retdata = g_malloc0(sizeof(*retdata));
	if (!retdata) {
		fprintf(stderr, "Fail to g_malloc0()\n");
		return -1;
	}

	// fetch data
	g_variant_get(variant, "(a{sv})", &iter);
	while(g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "Zone")) {
			retdata->Zone = get_gint32(value);
		} else if (!g_strcmp0(key, "ValueSequence")) {
			retdata->ValueSequence = get_gint32(value);
		} else if (!g_strcmp0(key, "Time")) {
			retdata->Time = get_gdouble(value);
		} else if (!g_strcmp0(key, "Value")) {
			//
			retdata->Value = get_guint32(value);
		}
	}

	// cleanup
	g_variant_iter_free(iter);
	amb_release_property_all_with_zone(variant);

	//
	*vo = retdata;

	return 0;
}

int get_GearboxPositionDisplay_with_zone(struct GearboxPositionDisplay_t **vo, int zone)
{
	GVariant *variant;
	GVariantIter *iter;
	gchar *key;
	GVariant *value;
	int ret;
	struct GearboxPositionDisplay_t *retdata = NULL;	// Need to fix

	// get data - need to fix
	ret = amb_get_property_all_with_zone(&variant, "GearboxPositionDisplay", zone);
	if (ret < 0) {
		fprintf(stderr, "Fail to amb_get_property_all_with_zone(): VehicleOdometer\n");
		return -1;
	}

	// allocation memory
	retdata = g_malloc0(sizeof(*retdata));
	if (!retdata) {
		fprintf(stderr, "Fail to g_malloc0()\n");
		return -1;
	}

	// fetch data
	g_variant_get(variant, "(a{sv})", &iter);
	while(g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "Zone")) {
			retdata->Zone = get_gint32(value);
		} else if (!g_strcmp0(key, "ValueSequence")) {
			retdata->ValueSequence = get_gint32(value);
		} else if (!g_strcmp0(key, "Time")) {
			retdata->Time = get_gdouble(value);
		} else if (!g_strcmp0(key, "Value")) {
			// need to fix
			guchar tmp = get_guchar(value);
			retdata->Value = tmp;
		}
	}

	// cleanup
	g_variant_iter_free(iter);
	amb_release_property_all_with_zone(variant);

	//
	*vo = retdata;

	return 0;
}

int get_VehicleSpeed_with_zone(struct VehicleSpeed_t **vo, int zone)
{
	GVariant *variant;
	GVariantIter *iter;
	gchar *key;
	GVariant *value;
	int ret;
	struct VehicleSpeed_t *retdata = NULL;	// Need to fix

	// get data - need to fix
	ret = amb_get_property_all_with_zone(&variant, "VehicleSpeed", zone);
	if (ret < 0) {
		fprintf(stderr, "Fail to amb_get_property_all_with_zone(): VehicleOdometer\n");
		return -1;
	}

	// allocation memory
	retdata = g_malloc0(sizeof(*retdata));
	if (!retdata) {
		fprintf(stderr, "Fail to g_malloc0()\n");
		return -1;
	}

	// fetch data
	g_variant_get(variant, "(a{sv})", &iter);
	while(g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "Zone")) {
			retdata->Zone = get_gint32(value);
		} else if (!g_strcmp0(key, "ValueSequence")) {
			retdata->ValueSequence = get_gint32(value);
		} else if (!g_strcmp0(key, "Time")) {
			retdata->Time = get_gdouble(value);
		} else if (!g_strcmp0(key, "Value") || !g_strcmp0(key, "Speed")) {
			// need to fix
			retdata->Value = get_guint16(value);
		}
	}

	// cleanup
	g_variant_iter_free(iter);
	amb_release_property_all_with_zone(variant);

	//
	*vo = retdata;

	return 0;
}

void free_VehicleOdometer(struct VehicleOdometer_t *retdata)
{
	g_free(retdata);
}

void free_result(void *retdata)
{
	g_free(retdata);
}




