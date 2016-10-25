#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libamb-client.h>
#include <libsamsung-can.h>

struct user_data_test {
	int value;
	char *name;
};

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

static void VehicleOdometer_handler(const gchar *objname, gpointer data, void *user_data)
{
	struct VehicleOdometerType retdata;
	struct user_data_test *udata = (struct user_data_test *)user_data;

	fprintf(stderr, "== User Data ==\n");
	fprintf(stderr, "  value: %d\n", udata->value);
	fprintf(stderr, "  name: %s\n", udata->name);


	if (!data)
		return ;

	convert_VehicleOdometerType(data, &retdata);
}

struct user_data_test *udata;

static gboolean timer_callback(gpointer d)
{
	int rc;
	gchar *objname = (gchar *)d;

	rc = amb_unregister_property_changed_handler(objname, 0);
	if (rc != 0) {
		fprintf(stderr, "Fail to amb_unregister_property_changed_handler(): %s\n", objname);
	}
	// cleanup
	free(udata->name);
	g_free(udata);
	exit(0);

	return FALSE;
}

void test_VehicleOdometer_listen()
{
	GMainLoop *loop;
	int rc;

	udata = g_new0(struct user_data_test, 1);
	udata->value = 20;
	udata->name = strdup("Test Name");

	rc = amb_register_property_changed_handler("VehicleOdometer",
					0,
					(AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler,
					(void *)udata);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "VehicleOdometer");
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}
int main()
{
	test_VehicleOdometer_listen();
	return 0;
}
