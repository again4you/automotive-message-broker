#include "libamb-util.h"

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

void amb_free_result(void *retdata)
{
	g_free(retdata);
}
