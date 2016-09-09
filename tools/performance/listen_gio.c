/*
 *
 */

#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <gio/gio.h>

#include <sys/time.h>
#include <time.h>

struct timeval _ts;
struct timeval _te;
struct timeval _tr;
#define START() gettimeofday(&_ts, NULL)
#define END() do { \
	gettimeofday(&_te, NULL); \
	timersub(&_te, &_ts, &_tr); \
} while (0)

static inline void PRT(const char *prefix)
{
	printf("T [%16s] %ld.%06ld sec\n", prefix, _tr.tv_sec, _tr.tv_usec);
}

static char _sig_name[1024];

static GDBusProxy *get_manager(void)
{
	GError *err;
	GDBusProxy *proxy;

	err = NULL;
	START();
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,
			"org.automotive.message.broker",
			"/",
			"org.automotive.Manager",
			NULL,
			&err);
	END();
	PRT(__func__);

	if (!proxy) {
		fprintf(stderr, "Proxy error: %s\n", err->message);
		g_clear_error(&err);
		return NULL;
	}

	return proxy;
}

static GDBusProxy *get_object(const char *objpath)
{
	GError *err;
	GDBusProxy *proxy;

	err = NULL;
	START();
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,
			"org.automotive.message.broker",
			objpath,
			"org.freedesktop.DBus.Properties",
			NULL,
			&err);
	END();
	PRT(__func__);

	if (!proxy) {
		fprintf(stderr, "Proxy error: %s\n", err->message);
		g_clear_error(&err);
		return NULL;
	}

	return proxy;
}

static void free_objs(GList *objs)
{
	GList *l;

	for (l = objs; l; l = g_list_next(l))
		g_object_unref(l->data);

	g_list_free(objs);
}

static GList *get_objects(GDBusProxy *proxy, const char *name)
{
	GError *err;
	GVariant *res;
	GList *list;
	const gchar *obj;
	GVariantIter *iter;

	err = NULL;
	START();
	res = g_dbus_proxy_call_sync(proxy,
			"FindObject",
			g_variant_new("(s)", name),
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&err);
	END();
	PRT(__func__);
	if (!res) {
		fprintf(stderr, "Call error: %s\n", err->message);
		g_clear_error(&err);
		return NULL;
	}
	g_clear_error(&err);

	list = NULL;
	g_variant_get(res, "(ao)", &iter);
	while (g_variant_iter_loop(iter, "o", &obj)) {
		GDBusProxy *objproxy;

		objproxy = get_object(obj);
		if (objproxy)
			list = g_list_append(list, objproxy);
	}
	g_variant_iter_free(iter);
	g_variant_unref(res);

	return list;
}

static void on_signal(GDBusProxy *proxy, const gchar *sender_name, const gchar *signal_name,
		GVariant *parameters, gpointer user_data)
{
	GVariantIter *iter;
	gchar *key;
	GVariant *value;

	struct timespec ts;
	double cur_ms;

	if (!strcmp(signal_name, _sig_name))
		return;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	cur_ms = ts.tv_sec + ts.tv_nsec / (double)1000000000;

	g_variant_get(parameters, "(sa{sv}as)", NULL, &iter, NULL);
	while (g_variant_iter_next(iter, "{sv}", &key, &value)) {
		if (!strcmp(key, "Time")) {
			double f = g_variant_get_double(value);
			printf("%lf = %lf - %lf\n", cur_ms - f, cur_ms, f);
		}
	}
	g_variant_iter_free(iter);
}

static void listen_objs(GList *objs)
{
	GList *l;

	for (l = objs; l; l = g_list_next(l)) {
		g_signal_connect(l->data, "g-signal", G_CALLBACK(on_signal), NULL);
	}
}

int main(int argc, char *argv[])
{
	GMainLoop *loop;
	GList *objs;
	GDBusProxy *mgr;

	if (argc < 2) {
		printf("usage) %s object_name\n", argv[0]);
		return 1;
	}

	mgr = get_manager();
	if (!mgr)
		return 1;

	objs = get_objects(mgr, argv[1]);
	if (!objs) {
		g_object_unref(mgr);
		return 1;
	}

	snprintf(_sig_name, sizeof(_sig_name), "org.automotive.%s", argv[1]);

	listen_objs(objs);

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	g_main_loop_unref(loop);

	free_objs(objs);
	g_object_unref(mgr);

	return 0;
}
