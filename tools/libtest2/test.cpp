#include <stdio.h>

#include <libamb-client.h>

void test_get_object_list()
{
        GList *objlist;
        GList *item;
	int ret;

        fprintf(stdout, "\n== %s ==\n", __func__);

        ret = get_object_list(&objlist);
	if (ret < 0) {
        	printf("Fail to get_object_list()\n");
		return ;
	}

        fprintf(stdout, "  - count: %u\n", g_list_length(objlist));
        for (item = objlist; item != NULL; item = item->next) {
                gchar *obj = static_cast<gchar*>(item->data);
                fprintf(stdout, "%s ", obj);
        }
        printf("\n");

	release_object_list(objlist);
}

void test_get_property_all(const char *obj_name)
{
        GList *retlist;
        GList *item;
	int ret;

        fprintf(stdout, "\n== %s ==\n", __func__);
        ret = get_property_all(&retlist, obj_name);
	if (ret < 0) {
        	printf("Fail to get_property_all()\n");
		return ;
	}

        fprintf(stdout, "  - count: %u\n", g_list_length(retlist));
        for (item = retlist; item != NULL; item = item->next) {
                GVariant *i = static_cast<GVariant*>(item->data);
                gchar *s = g_variant_print(i, TRUE);
                printf("%s\n", s);
                g_free(s);
        }

	release_property_all(retlist);
}

void test_get_property_all_with_zone(const char *obj_name, int zone)
{
        GVariant *ret;
	int r;
        gchar *s;

        fprintf(stdout, "\n== %s ==\n", __func__);
        r = get_property_all_with_zone(&ret, obj_name, zone);
        if (r < 0) {
                fprintf(stderr, "Fail to get_property_all_with_zone(): %s, %d\n", obj_name, zone);
                return ;
        }

        s = g_variant_print(ret, TRUE);
        printf("%s\n", s);

        g_free(s);

	release_property_all_with_zone(ret);
}

void test_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value)
{
        GVariant *ret;
        gchar *s;
	int r;

        fprintf(stdout, "== %s ==\n", __func__);

        if (set_property(obj_name, prop_name, zone, value) != 0) {
                fprintf(stderr, "Fail to set_property()\nn");
                return ;
        }

        r = get_property_all_with_zone(&ret, obj_name, zone);
        if (r < 0) {
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

#if 0
        test_get_property_all_with_zone("ClimateControl", 5);
        test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));
        test_get_property_all("ClimateControl");
        test_get_property_all("VehicleSpeed");

        test_get_property_all_with_zone("ClimateControl", 5);

        test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));
#endif
	return 0;
}
