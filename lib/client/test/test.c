#include <stdio.h>

#include <libamb-client.h>
void test_get_object_list()
{
	GList *objlist;
	GList *item;

	fprintf(stdout, "\n== %s ==\n", __func__);

	objlist = get_object_list();

	fprintf(stdout, "  - count: %u\n", g_list_length(objlist));
	for (item = objlist; item != NULL; item = item->next) {
		gchar *obj = item->data;
		fprintf(stdout, "%s ", obj);
	}
	printf("\n");

	// clean up
	for (item = objlist; item != NULL; item = item->next) {
		gchar *obj = item->data;
		objlist = g_list_remove(objlist, obj);
		if (obj)
			g_free(obj);
	}
}

void test_get_property_all(const char *obj_name)
{
	GList *retlist;
	GList *item;

	fprintf(stdout, "\n== %s ==\n", __func__);
	retlist = get_property_all(obj_name);

	fprintf(stdout, "  - count: %u\n", g_list_length(retlist));
	for (item = retlist; item != NULL; item = item->next) {
		GVariant *i = item->data;
		gchar *s = g_variant_print(i, TRUE);
		printf("%s\n", s);
		g_free(s);
	}

	// clean up
	for (item = retlist; item != NULL; item = item->next) {
		GVariant *i = item->data;
		g_variant_unref(i);
	}
}

void test_get_property_all_with_zone(const char *obj_name, int zone)
{
	GVariant *ret;
	gchar *s;

	fprintf(stdout, "\n== %s ==\n", __func__);
	ret = get_property_all_with_zone(obj_name, zone);
	if (!ret) {
		fprintf(stderr, "Fail to get_property_all_with_zone(): %s, %d\n", obj_name, zone);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);
	g_variant_unref(ret);
}

void test_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value)
{
	GVariant *ret;
	gchar *s;

	fprintf(stdout, "== %s ==\n", __func__);

	if (set_property(obj_name, prop_name, zone, value) != 0) {
		fprintf(stderr, "Fail to set_property()\nn");
		return ;
	}

	ret = get_property_all_with_zone(obj_name, zone);
	if (!ret) {
		fprintf(stderr, "Fail to get_property_all_with_zone(): %s, %d\n", obj_name, zone);
		return ;
	}

	s = g_variant_print(ret, TRUE);
	printf("%s\n", s);

	g_free(s);
	g_variant_unref(ret);
}

int main()
{
	test_get_object_list();

	test_get_property_all("ClimateControl");
	test_get_property_all("VehicleSpeed");

	test_get_property_all_with_zone("ClimateControl", 5);

	test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));

	return 0;
}
