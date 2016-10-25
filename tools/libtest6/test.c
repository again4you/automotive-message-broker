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
struct user_data_test *udata2;

static gboolean timer_callback(gpointer d)
{
	int rc;
	gchar *objname = (gchar *)d;

#if 0
	rc = amb_unregister_property_changed_handler(objname, 0, 1);
	if (rc != 0) {
		fprintf(stderr, "Fail to amb_unregister_property_changed_handler(): %s\n", objname);
	}
#endif

#if 1
	// cleanup
	free(udata->name);
	g_free(udata);
	exit(0);
#endif

	return FALSE;
}

void test_VehicleOdometer_listen()
{
	GMainLoop *loop;
	guint32 id1;
	guint32 id2;
	int rc;

	udata = g_new0(struct user_data_test, 1);
	udata->value = 20;
	udata->name = strdup("Test Name");

	rc = amb_register_property_changed_handler("VehicleOdometer",
					0,
					(AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler,
					(void *)udata, &id1);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}

#if 0
	udata2 = g_new0(struct user_data_test, 1);
	udata2->value = 100;
	udata2->name = strdup("Test Name2");
	rc = amb_register_property_changed_handler("VehicleOdometer",
					0,
					(AMB_PROPERTY_CHANGED_CALLBACK)VehicleOdometer_handler,
					(void *)udata2, &id2);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_register_property_changed_handler(): %s\n", "VehicleOdometer");
                return ;
	}
	fprintf(stderr, "id1: %u, id2: %u\n", id1, id2);
#endif

#if 1
	rc = amb_unregister_property_changed_handler("VehicleOdometer", 0, id1);
	if (rc != 0) {
                fprintf(stderr, "Fail to amb_unregister_property_changed_handler(): %u\n", id1);
		return ;
	}
#endif

	g_timeout_add_seconds(10, timer_callback, "VehicleOdometer");
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
}


GList *cb_list;

struct callback_item {
	AMB_PROPERTY_CHANGED_CALLBACK callback;
	void *user_data;
	gboolean is_delete;
	guint32 cid;
};

guint32 cid;

int main()
{
#if 1
	test_VehicleOdometer_listen();
#endif
	
#if 0
	struct callback_item *citem01;
	struct callback_item *citem02;
	GList *l;
	GList *ll;
	cid = 0;

	citem01 = g_new0(struct callback_item, 1);
	citem01->is_delete = FALSE;
	citem01->user_data = (void *)strdup("item_01");
	citem01->cid = __atomic_fetch_add(&cid, 1, __ATOMIC_RELAXED);

	cb_list = g_list_append(cb_list, citem01);

	citem02 = g_new0(struct callback_item, 1);
	citem02->is_delete = FALSE;
	citem02->user_data = (void *)strdup("item_02");
	citem02->cid = __atomic_fetch_add(&cid, 1, __ATOMIC_RELAXED);

	cb_list = g_list_append(cb_list, citem02);

#if 0
	ncid = __atomic_fetch_add(&cid, 1, __ATOMIC_RELAXED);
	fprintf(stderr, "cid: %u, ncid: %u\n", cid, ncid);
	ncid = __atomic_fetch_add(&cid, 1, __ATOMIC_RELAXED);
	fprintf(stderr, "cid: %u, ncid: %u\n", cid, ncid);
#endif

	// list
	fprintf(stderr, "\n* Listing\n");
	for (l = cb_list; l != NULL; l = l->next) {
		struct callback_item *citem = l->data;
		fprintf(stderr, "citem->cid: %u\n", citem->cid);	
		fprintf(stderr, "citem->user_data: %s\n", (char*)citem->user_data);	
	}


	// find 
	fprintf(stderr, "\n* Finding\n");
	for (l = cb_list; l != NULL; l = l->next) {
		struct callback_item *citem = l->data;
		citem->is_delete = TRUE;
		fprintf(stderr, "citem->cid: %u\n", citem->cid);	
		fprintf(stderr, "citem->user_data: %s\n", (char*)citem->user_data);	
	}
	
	// delete
	fprintf(stderr, "\n* Deleting\n");
	for (l = cb_list, ll = g_list_next(l);
		l != NULL;
		l = ll, ll = g_list_next(ll)) {
		struct callback_item *citem = l->data;
		fprintf(stderr, "citem->cid: %u\n", citem->cid);	
		if (citem->is_delete) {
			fprintf(stderr, "Delete citem->cid: %u\n", citem->cid);	
			cb_list = g_list_delete_link(cb_list, l);
		}

	}

	// list
	fprintf(stderr, "\n* Listing\n");
	for (l = cb_list; l != NULL; l = l->next) {
		struct callback_item *citem = l->data;
		fprintf(stderr, "citem->cid: %u\n", citem->cid);	
		fprintf(stderr, "citem->user_data: %s\n", (char*)citem->user_data);	
	}

	if (g_list_length(cb_list) == 0) {
		fprintf(stderr, "cb_list item is 0\n");
	}

	// cleanup
	g_free(citem01);
	g_free(citem02);
#endif

	return 0;
}
