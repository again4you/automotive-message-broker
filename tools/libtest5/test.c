#include <stdio.h>
#include <stdlib.h>

#include <libamb-client.h>
#include <libsamsung-can.h>

static int convert_VehicleOdometerType(GVariant *data, struct VehicleOdometerType *retdata)
{
	GVariantIter *iter;
	gchar *key;
	GVariant *value;
	GVariant *gdata;

	gdata = (GVariant *)data;

	
	g_variant_get(gdata, "a{sv}", &iter);
	while(g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "Zone")) {
			g_variant_get(value, "i", &retdata->Zone);
			fprintf(stderr, "%s: %d\n", key, retdata->Zone);
		} else if (!g_strcmp0(key, "ValueSequence") || !g_strcmp0(key, "ValueSequence")) {
			g_variant_get(value, "i", &retdata->ValueSequence);
			fprintf(stderr, "%s: %d\n", key, retdata->ValueSequence);
		} else if (!g_strcmp0(key, "Time")) {
			g_variant_get(value, "d", &retdata->Time);
			fprintf(stderr, "%s: %f\n", key, retdata->Time);
		} else if (!g_strcmp0(key, "Value") || !g_strcmp0(key, "Vaule")) {
			g_variant_get(value, "u", &retdata->Value);
			fprintf(stderr, "%s: %d\n", key, retdata->Value);
		}
	}
	g_variant_iter_free(iter);
	return 0;
}
static void VehicleOdometer_handler(const gchar *objname, gpointer data)
{
	struct VehicleOdometerType retdata;

	if (!data)
		return ;

	convert_VehicleOdometerType(data, &retdata);
}

void test_VehicleOdometer_listen()
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler("VehicleOdometer",
					0,
					(AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler,
					NULL);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}

	// g_timeout_add_seconds(10, timer_callback, "VehicleOdometer");
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}
int main()
{
	test_VehicleOdometer_listen();
	return 0;
}
