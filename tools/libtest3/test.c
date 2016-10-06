#include <stdio.h>
#include <stdlib.h>

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
	exit(0);

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

#if 1
int convert_VehicleOdometerType(GVariant *data, struct VehicleOdometerType *retdata)
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
#endif

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

	rc = amb_register_property_changed_handler("VehicleOdometer", 0, (AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "VehicleOdometer");
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}


#if 0
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
#endif

void test_samsungcan_GearboxPosition()
{
	struct GearboxPositionDisplayType *p;
	int ret = amb_get_GearboxPositionDisplay_with_zone(&p, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		amb_free_result(p);
		return ;
	}

	fprintf(stderr, " == GearboxPosition ==\n");
	fprintf(stderr, "    Time: %f\n", p->Time);
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Value: ");
	switch(p->Value) {
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
		fprintf(stderr, "%u (Error)\n", p->Value);
		break;
	}

	amb_free_result(p);
	return ;
}

void test_samsungcan_VehicleSpeed()
{
	struct VehicleSpeedType *p;
	int ret = amb_get_VehicleSpeed_with_zone(&p, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		amb_free_result(p);
		return ;
	}
	fprintf(stderr, "Zone: %d\n", p->Zone);
	fprintf(stderr, "Value: %d\n", p->Value);
	fprintf(stderr, "ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "Time: %f\n", p->Time);

	amb_free_result(p);
	return ;
}

void test_samsungcan_VehicleOdometer()
{
	struct VehicleOdometerType *p;
	int ret = amb_get_VehicleOdometer_with_zone(&p, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		amb_free_result(p);
		return ;
	}
	fprintf(stderr, "Zone: %d\n", p->Zone);
	fprintf(stderr, "Value: %d\n", p->Value);
	fprintf(stderr, "ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "Time: %f\n", p->Time);

	amb_free_result(p);
	return ;
}


int main()
{
	test_VehicleOdometer_listen();
#if 0
	test_signal_listen("VehicleOdometer", 0);
	test_samsungcan_GearboxPosition();
	test_samsungcan_VehicleOdometer();

	test_samsungcan_VehicleSpeed();
#endif
#if 0
	test_get_property_all_with_zone("VehicleOdometer", 0);
#endif

#if 0
	test_GearboxPositionDisplay_00();
	test_get_property_all_with_zone("GearboxPositionDisplay", 0);

	enum GearboxPositionDisplay pos = PARKING;
	printf("%d\n", pos);
#endif

#if 0
	// CAN_OBJECT(VehicleOdometer2, guint32, NULL);
	struct VehicleOdometerType *p;
	int ret = get_VehicleOdometer_with_zone(&p, 0);
	fprintf(stderr, "Zone: %d\n", p->Zone);
	fprintf(stderr, "Value: %d\n", p->Value);
	fprintf(stderr, "ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "Time: %f\n", p->Time);
#endif


#if 0
	test_VehicleSpeed_00();
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
