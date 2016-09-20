#include <stdio.h>

#include <libamb-client.h>
void test_get_object_list()
{
	GList *objlist;
	GList *item;

	fprintf(stdout, "== %s ==\n", __func__);

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

	fprintf(stdout, "== %s ==\n", __func__);
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

int main()
{
	test_get_object_list();

	test_get_property_all("ClimateControl");
	test_get_property_all("VehicleSpeed");

	return 0;
}



