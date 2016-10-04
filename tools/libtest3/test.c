#include <stdio.h>

#include <libamb-client.h>
#include <libsamsung-can.h>

void test_get_object_list()
{
	GList *objlist;
	GList *item;
	int ret;

	fprintf(stdout, "\n== %s ==\n", __func__);

	ret = amb_get_object_list(&objlist);
	if (ret < 0) {
		printf("Fail to amb_get_object_list()\n");
		return ;
	}

	fprintf(stdout, "  - count: %u\n", g_list_length(objlist));
	for (item = objlist; item != NULL; item = item->next) {
		gchar *obj = item->data;
		fprintf(stdout, "%s ", obj);
	}
	printf("\n");

	amb_release_object_list(objlist);
}

void test_get_property_all(const char *obj_name)
{
	GList *retlist;
	GList *item;
	int ret;

	fprintf(stdout, "\n== %s ==\n", __func__);
	ret = amb_get_property_all(&retlist, obj_name);
	if (ret < 0) {
		printf("Fail to get_property_all()\n");
		return ;
	}

	fprintf(stdout, "  - count: %u\n", g_list_length(retlist));
	for (item = retlist; item != NULL; item = item->next) {
		GVariant *i = item->data;
		gchar *s = g_variant_print(i, TRUE);
		printf("%s\n", s);
		g_free(s);
	}

	amb_release_property_all(retlist);
}

void test_get_property_all_with_zone(const char *obj_name, int zone)
{
	GVariant *ret;
	int r;
	gchar *s;

	fprintf(stdout, "\n== %s ==\n", __func__);
	r = amb_get_property_all_with_zone(&ret, obj_name, zone);
	if (r < 0) {
		fprintf(stderr, "Fail to get_property_all_with_zone(): %s, %d\n", obj_name, zone);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);

	amb_release_property_all_with_zone(ret);
}

void test_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value)
{
	GVariant *ret;
	gchar *s;
	int r;

	fprintf(stdout, "== %s ==\n", __func__);

	if (amb_set_property(obj_name, prop_name, zone, value) != 0) {
		fprintf(stderr, "Fail to set_property()\nn");
		return ;
	}

	r = amb_get_property_all_with_zone(&ret, obj_name, zone);
	if (r < 0) {
		fprintf(stderr, "Fail to get_property_all_with_zone(): %s, %d\n", obj_name, zone);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);
	amb_release_property_all_with_zone(ret);
}

static void signal_handler(const gchar *objname, GVariant *data)
{
	gchar *s;

	if (!data)
		return ;

	s = g_variant_print(data, TRUE);
	g_print("== signal_handler: %s\n", objname);
	g_print("%s\n", s);

	g_free(s);
}

static gboolean timer_callback(gpointer d)
{
	int rc;
	gchar *objname = (gchar *)d;

	rc = amb_unregister_property_changed_handler(objname, 0);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_unregister_property_changed_handler(): %s\n", objname);
	}

	return FALSE;
}

void test_signal_listen(gchar *objname, ZoneType zone)
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler(objname, zone, (AMB_PROPERTY_CHANGED_CALLBACK)signal_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", objname);
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, objname);

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}

void test_VehicleOdometer_00()
{
	struct VehicleOdometer_t *retOdometer;
	get_VehicleOdometer_with_zone(&retOdometer, 0);

	fprintf(stderr, " == VehicleOdometer ==\n");
	fprintf(stderr, "    Time: %f\n", retOdometer->Time);
	fprintf(stderr, "    Zone: %d\n", retOdometer->Zone);
	fprintf(stderr, "    Value: %u\n", retOdometer->Value);
	fprintf(stderr, "    ValueSequence: %d\n", retOdometer->ValueSequence);

	// free_VehicleOdometer(retOdometer);
	free_result(retOdometer);
}

void test_GearboxPositionDisplay_00()
{
	struct GearboxPositionDisplay_t *ret;
	get_GearboxPositionDisplay_with_zone(&ret, 0);

	fprintf(stderr, " == VehicleOdometer ==\n");
	fprintf(stderr, "    Time: %f\n", ret->Time);
	fprintf(stderr, "    Zone: %d\n", ret->Zone);
	fprintf(stderr, "    ValueSequence: %d\n", ret->ValueSequence);
	fprintf(stderr, "    Value: ");
	switch(ret->Value) {
	case PARKING:
		fprintf(stderr, "PARKING\n");
		break;
	case DRIVE:
		fprintf(stderr, "DRIVE\n");
		break;
	case NEUTRAL:
		fprintf(stderr, "NEUTRAL\n");
		break;
	case REVERSE:
		fprintf(stderr, "REVERSE\n");
		break;
	default:
		fprintf(stderr, "%u (Error)\n", ret->Value);
		break;
	}

	// free_VehicleOdometer(retOdometer);
	free_result(ret);
}

void test_VehicleSpeed_00()
{
	struct VehicleSpeed_t *ret;
	get_VehicleSpeed_with_zone(&ret, 0);

	fprintf(stderr, " == VehicleOdometer ==\n");
	fprintf(stderr, "    Time: %f\n", ret->Time);
	fprintf(stderr, "    Zone: %d\n", ret->Zone);
	fprintf(stderr, "    Value: %u\n", ret->Value);
	fprintf(stderr, "    ValueSequence: %d\n", ret->ValueSequence);

	// free_VehicleOdometer(retOdometer);
	free_result(ret);
}


int main()
{
	test_get_property_all_with_zone("GearboxPositionDisplay", 0);

	enum GearboxPositionDisplay pos = PARKING;
	printf("%d\n", pos);

	test_VehicleSpeed_00();
	
#if 0
	test_GearboxPositionDisplay_00();
	test_VehicleOdometer_00();
	test_get_object_list();

	test_get_property_all("VehicleSpeed");

	test_get_property_all("ClimateControl");
	test_get_property_all("VehicleSpeed");

	test_get_property_all_with_zone("ClimateControl", 5);

	test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));

	test_signal_listen("VehicleSpeed", 0);
#endif

	return 0;
}
