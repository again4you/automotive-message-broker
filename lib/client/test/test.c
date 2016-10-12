#include <stdio.h>

#include <libamb-client.h>
#include <libamb-objects.h>

static void test_get_object_list()
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

static void test_get_property_all(const char *obj_name)
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

static void test_get_property_all_with_zone(const char *obj_name, int zone)
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

static void test_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value)
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

static void signal_handler(const gchar *objname, gpointer data)
{
	gchar *s;
	GVariant *gdata = (GVariant *)data;

	if (!data)
		return ;

	s = g_variant_print(gdata, TRUE);
	g_print("== signal_handler: %s\n", objname);
	g_print("%s\n", s);

	g_free(s);
}

static gboolean timer_callback(gpointer d)
{
	int rc;
	gchar *objname = (gchar *)d;

        fprintf(stderr, "Enter %s(): %s\n", __func__, objname);

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

static void VehicleOdometer_handler(const gchar *objname, gpointer data)
{
	struct VehicleOdometerType retdata;

	if (!data)
		return ;

	amb_convert_VehicleOdometerType(data, &retdata);

	fprintf(stderr, " == VehicleOdometer ==\n");
	fprintf(stderr, "    Zone: %d\n", retdata.Zone);
	fprintf(stderr, "    Value: %d\n", retdata.Value);
	fprintf(stderr, "    ValueSequence: %d\n", retdata.ValueSequence);
	fprintf(stderr, "    Time: %f\n", retdata.Time);
}

static void test_VehicleOdometer_listen()
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler("VehicleOdometer", 0, 
			(AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "VehicleOdometer");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}

static void test_samsungcan_GearboxPosition()
{
	struct GearboxPositionDisplayType *p;
	int ret = amb_get_GearboxPositionDisplay_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
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

	amb_release_data(p);
	return ;
}

static void test_samsungcan_VehicleSpeed()
{
	struct VehicleSpeedType *p;
	int ret = amb_get_VehicleSpeed_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == VehicleSpeed ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    Value: %d\n", p->Value);
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
	return ;
}

static void test_LampHazardLight()
{
	struct LampHazardLightType *p;
	int ret = amb_get_LampHazardLight_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == LampHazardLight ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    Value: %s\n", (p->Value) ? "Set" : "None");
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
}

static void LampHazardLight_handler(const gchar *objname, gpointer data)
{
	struct LampHazardLightType retdata;

	if (!data)
		return ;

	amb_convert_LampHazardLightType(data, &retdata);

	fprintf(stderr, " == LampHazardLight ==\n");
	fprintf(stderr, "    Zone: %d\n", retdata.Zone);
	fprintf(stderr, "    Value: %s\n", (retdata.Value) ? "Set" : "None");
	fprintf(stderr, "    ValueSequence: %d\n", retdata.ValueSequence);
	fprintf(stderr, "    Time: %f\n", retdata.Time);
}

static void test_LampHazardLight_listen()
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler("LampHazardLight", 0,
			(AMB_PROPERTY_CHANGED_CALLBACK)LampHazardLight_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "LampHazardLight");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "LampHazardLight");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}

static void test_WarningSafetybelts()
{
	struct WarningSafetybeltsType *p;
	int ret = amb_get_WarningSafetybelts_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == WarningSafetybelt ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    Value: %s\n", (p->Value) ? "Set" : "None");
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
}

static void WarningSafetybelts_handler(const gchar *objname, gpointer data)
{
	struct WarningSafetybeltsType retdata;

	if (!data)
		return ;

	amb_convert_WarningSafetybeltsType(data, &retdata);

	fprintf(stderr, " == WarningSafetybelts ==\n");
	fprintf(stderr, "    Zone: %d\n", retdata.Zone);
	fprintf(stderr, "    Value: %s\n", (retdata.Value) ? "Set" : "None");
	fprintf(stderr, "    ValueSequence: %d\n", retdata.ValueSequence);
	fprintf(stderr, "    Time: %f\n", retdata.Time);
}

static void test_WarningSafetybelts_listen()
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler("WarningSafetybelts", 0,
			(AMB_PROPERTY_CHANGED_CALLBACK)WarningSafetybelts_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "WarningSafetybelts");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "WarningSafetybelts");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}

static void test_samsungcan_VehicleOdometer()
{
	struct VehicleOdometerType *p;
	int ret = amb_get_VehicleOdometer_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == VehicleOdometer ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    Value: %d\n", p->Value);
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
	return ;
}

static void test_TPMS_FL()
{
	struct TPMS_FLType *p;
	int ret = amb_get_TPMS_FL_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == TPMS_FL ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	// fprintf(stderr, "    Value: %u\n", (unsigned int)p->Value);
	fprintf(stderr, "    Value: %u\n", p->Value);
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
}

static void TPMS_FL_handler(const gchar *objname, gpointer data)
{
	struct TPMS_FLType retdata;

	if (!data)
		return ;

	amb_convert_TPMS_FLType(data, &retdata);

	fprintf(stderr, " == TPMS_FL ==\n");
	fprintf(stderr, "    Zone: %d\n", retdata.Zone);
	fprintf(stderr, "    Value: %u\n", retdata.Value);
	fprintf(stderr, "    ValueSequence: %d\n", retdata.ValueSequence);
	fprintf(stderr, "    Time: %f\n", retdata.Time);
}

static void test_TPMS_FL_listen()
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_property_changed_handler("TPMS_FL", 0,
			(AMB_PROPERTY_CHANGED_CALLBACK)TPMS_FL_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "WarningSafetybelts");
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, "TPMS_FL");

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}

static void test_FuelGage()
{
	struct FuelGageType *p;
	int ret = amb_get_FuelGage_with_zone(&p, None);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, " == FuelGage ==\n");
	fprintf(stderr, "    Zone: %d\n", p->Zone);
	fprintf(stderr, "    Value: %u\n", p->Value);
	fprintf(stderr, "    ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "    Time: %f\n", p->Time);

	amb_release_data(p);
}

int main()
{
	test_get_object_list();

	// cansend vcan0 104#3C.00.00.00.00.00.00.00
	test_FuelGage();

	// cansend vcan0 104#3C.00.00.00.00.00.00.00
	test_TPMS_FL();
	test_TPMS_FL_listen();

	// cansend vcan0 206#00.00.00.00.01.00.00.00
	test_LampHazardLight();
	test_LampHazardLight_listen();

	// cansend vcan0 206#01.00.00.00.00.00.00.00
	test_WarningSafetybelts();
	test_WarningSafetybelts_listen();
	test_samsungcan_VehicleOdometer();
	test_samsungcan_VehicleSpeed();
	test_samsungcan_GearboxPosition();

	test_VehicleOdometer_listen();

#ifdef EXAMPLE_PLUGIN
	test_get_property_all("ClimateControl");
	test_get_property_all("VehicleSpeed");

	test_get_property_all_with_zone("ClimateControl", 5);

	test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));

	test_signal_listen("VehicleSpeed", 0);

	test_samsungcan_GearboxPosition();
#endif

	return 0;
}
