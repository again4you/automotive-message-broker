#include <stdio.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <libamb-client.h>

void test_get_object_list()
{
        GList *objlist;
        GList *item;
	int ret;

        fprintf(stdout, "\n== %s ==\n", __func__);

        ret = amb_get_object_list(&objlist);
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
                GVariant *i = static_cast<GVariant*>(item->data);
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


void signal_handler(const gchar *objname, GVariant *data)
{
	gchar *s;

	if (!data)
		return ;

	s = g_variant_print(data, TRUE);
	g_print("== signal_handler: %s\n", objname);
	g_print("%s\n", s);

	g_free(s);
}

void test_signal_listen(gchar *objname)
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_signal_handler(objname, (AMB_SIGNAL_CALLBACK)signal_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_signal_handler(): %s\n", objname);
                return ;
	}

#if 0
	rc = amb_unregister_signal_handler(objname);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_unregister_signal_handler(): %s\n", objname);
                return ;
	}
#endif

#if 1
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
#endif
}

static gboolean timer_callback(gpointer d)
{
	int rc;
	gchar *objname = (gchar *)d;

	rc = amb_unregister_signal_handler(objname);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_unregister_signal_handler(): %s\n", objname);
	}

	return false;
}

void test_signal_listen2(gchar *objname)
{
	GMainLoop *loop;
	int rc;

	rc = amb_register_signal_handler(objname, (AMB_SIGNAL_CALLBACK)signal_handler);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_signal_handler(): %s\n", objname);
                return ;
	}

	g_timeout_add_seconds(10, timer_callback, objname);

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}


#if 0
void test_gvariant()
{
	gchar *obj1 = "VehicleSpeed";
	gchar *obj2 = "ClimateControl";
	guint size;

	GHashTable *htable;
	GList *id_list1;
	GList *id_list2;
	GList *get_list;
	GList *item;

	// init hash
	htable = g_hash_table_new(g_str_hash, g_str_equal);
	// add item list
	id_list1 = NULL;
	id_list1 = g_list_append(id_list1, GINT_TO_POINTER(10));
	id_list1 = g_list_append(id_list1, GINT_TO_POINTER(20));

	id_list2 = NULL;
	id_	ist2 = g_list_append(id_list2, GINT_TO_POINTER(300));
	id_list2 = g_list_append(id_list2, GINT_TO_POINTER(400));

	// if (!g_hash_table_insert(htable, g_strdup(obj1), id_list1)) {
	if (!g_hash_table_insert(htable, obj1, id_list1)) {
		printf("Fail to g_hash_table_insert()\n");
	}

	if (!g_hash_table_insert(htable, obj2, id_list2)) {
		printf("Fail to g_hash_table_insert()\n");
		return ;
	}

	// size
	size = g_hash_table_size(htable);
	printf("Size: %u\n", size);

	// lookup
	get_list = (GList*)g_hash_table_lookup(htable, obj1);
	for (item = get_list; item != NULL; item = item->next) {
		printf("%d \n", item->data);
	}

	// remove
	if (!g_hash_table_remove(htable, obj1)) {
		printf("Fail to g_hash_table_remove()\n");
		return ;
	}

	// size
	size = g_hash_table_size(htable);
	printf("Size: %u\n", size);

	// contain check
	if (g_hash_table_contains(htable, obj2)) {
		printf("%s exists\n", obj2);
		
	}

	// cleanup
	g_list_free(id_list1);
	g_list_free(id_list2);
	

	g_hash_table_remove(htable, obj2);
	g_hash_table_destroy(htable);

}
#endif

int main()
{
#if 0
        test_get_object_list();

	test_get_property_all("ClimateControl");
        test_get_property_all("VehicleSpeed");

        test_get_property_all_with_zone("ClimateControl", 5);

        test_set_property("ClimateControl", "AirConditioning", 5, g_variant_new("b", TRUE));
	test_gvariant();
#endif

	// test_signal_listen("VehicleSpeed");
	test_signal_listen2("VehicleSpeed");
	// test_signal_listen("ClimateControl");
	return 0;
}
