/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <dbus/dbus.h>

static int verbose;

#include <sys/time.h>

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

static void free_objs(GList *objs)
{
	GList *l;

	for (l = objs; l; l = g_list_next(l))
		free(l->data); // obj path string

	g_list_free(objs);
}

static GList *get_objects(DBusConnection *conn, const char *name)
{
	DBusMessage *msg;
	DBusMessageIter iter;
	const char *arg;
	DBusPendingCall *pending;
	int r;
	GList *list;

	START();
	msg = dbus_message_new_method_call("org.automotive.message.broker",
			"/",
			"org.automotive.Manager",
			"FindObject");

	arg = name;
	dbus_message_iter_init_append(msg, &iter);
	dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &arg);

	r = dbus_connection_send_with_reply(conn, msg, &pending, -1);
	if (!r) {
		fprintf(stderr, "send error\n");
		return NULL;
	}
	if (pending == NULL) {
		fprintf(stderr, "Pending call Null\n");
		return NULL;
	}

	dbus_connection_flush(conn);
	dbus_message_unref(msg);

	dbus_pending_call_block(pending);

	msg = dbus_pending_call_steal_reply(pending);
	if (msg == NULL) {
		fprintf(stderr, "Reply Null\n");
		return NULL;
	}
	dbus_pending_call_unref(pending);
	END();
	PRT(__func__);

	list = NULL;
	dbus_message_iter_init(msg, &iter);
	while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
		DBusMessageIter subiter;
		int stype;

		dbus_message_iter_recurse(&iter, &subiter);
		while ((stype = dbus_message_iter_get_arg_type(&subiter)) != DBUS_TYPE_INVALID) {
			dbus_message_iter_get_basic(&subiter, &arg);
			if (arg)
				list = g_list_append(list, strdup(arg));
			dbus_message_iter_next(&subiter);
		}
		dbus_message_iter_next(&iter);
	}

	dbus_message_unref(msg);

	return list;
}

static void printVariant(DBusMessageIter *iter)
{
	DBusMessageIter subiter;
	int stype;
	char *arg;
	double db;
	unsigned int ui;
	int i;

	dbus_message_iter_recurse(iter, &subiter);
	while ((stype = dbus_message_iter_get_arg_type(&subiter)) != DBUS_TYPE_INVALID) {
		switch (stype) {
		case DBUS_TYPE_STRING:
			dbus_message_iter_get_basic(&subiter, &arg);
			printf("\tString: %s\n", arg);
			break;
		case DBUS_TYPE_DOUBLE:
			dbus_message_iter_get_basic(&subiter, &db);
			printf("\tDouble: %lf\n", db);
			break;
		case DBUS_TYPE_INT16:
		case DBUS_TYPE_INT32:
			dbus_message_iter_get_basic(&subiter, &i);
			printf("\tInt: %u\n", i);
			break;
		case DBUS_TYPE_UINT16:
		case DBUS_TYPE_UINT32:
			dbus_message_iter_get_basic(&subiter, &ui);
			printf("\tUint: %u\n", ui);
			break;
		case DBUS_TYPE_VARIANT:
			printVariant(&subiter);
			break;
		default:
			printf("\tType: %d\n", stype);
			break;
		}
		dbus_message_iter_next(&subiter);
	}
}

static void printDict(DBusMessageIter *iter)
{
	DBusMessageIter subiter;
	int stype;
	char *arg;

	dbus_message_iter_recurse(iter, &subiter);
	while ((stype = dbus_message_iter_get_arg_type(&subiter)) != DBUS_TYPE_INVALID) {
		switch (stype) {
		case DBUS_TYPE_STRING:
			dbus_message_iter_get_basic(&subiter, &arg);
			printf("\tString: %s\n", arg);
			break;
		case DBUS_TYPE_VARIANT:
			printVariant(&subiter);
			break;
		default:
			printf("\tType: %d\n", stype);
			break;
		}
		dbus_message_iter_next(&subiter);
	}
}

static void printArray(DBusMessageIter *iter)
{
	DBusMessageIter subiter;
	int stype;

	dbus_message_iter_recurse(iter, &subiter);
	while ((stype = dbus_message_iter_get_arg_type(&subiter)) != DBUS_TYPE_INVALID) {
		switch (stype) {
		case DBUS_TYPE_DICT_ENTRY:
			printDict(&subiter);
			break;
		default:
			printf("\tType: %d\n", stype);
			break;
		}
		dbus_message_iter_next(&subiter);
	}
}

static void get_value(DBusConnection *conn, const char *objpath, const char *name)
{
	DBusMessage *msg;
	DBusMessageIter iter;
	const char *arg;
	DBusPendingCall *pending;
	int r;
	int type;

	START();
	msg = dbus_message_new_method_call("org.automotive.message.broker",
			objpath,
			"org.freedesktop.DBus.Properties",
			"GetAll");

	arg = name;
	dbus_message_iter_init_append(msg, &iter);
	dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &arg);

	r = dbus_connection_send_with_reply(conn, msg, &pending, -1);
	if (!r) {
		fprintf(stderr, "send error\n");
		return;
	}
	if (pending == NULL) {
		fprintf(stderr, "Pending call Null\n");
		return;
	}

	dbus_connection_flush(conn);
	dbus_message_unref(msg);

	dbus_pending_call_block(pending);

	msg = dbus_pending_call_steal_reply(pending);
	if (msg == NULL) {
		fprintf(stderr, "Reply Null\n");
		return;
	}
	dbus_pending_call_unref(pending);
	END();
	PRT(__func__);

	if (verbose) {
		dbus_message_iter_init(msg, &iter);
		while ((type = dbus_message_iter_get_arg_type(&iter)) != DBUS_TYPE_INVALID) {
			switch (type) {
			case DBUS_TYPE_ARRAY:
				printArray(&iter);
				break;
			default:
				printf("\tType: %d\n", type);
				break;
			}
			dbus_message_iter_next(&iter);
		}
	}

	dbus_message_unref(msg);
}

static void get_objs_value(DBusConnection *conn, GList *objs, const char *name)
{
	GList *l;
	char buf[1024];

	snprintf(buf, sizeof(buf), "org.automotive.%s", name);

	for (l = objs; l; l = g_list_next(l))
		get_value(conn, l->data, buf);
}

int main(int argc, char *argv[])
{
	DBusConnection *conn;
	DBusError err;
	GList *objs;

	if (argc < 2) {
		printf("usage) %s object_name\n", argv[0]);
		return 1;
	}
	if (argc > 2)
		verbose = 1;

	dbus_error_init(&err);
	START();
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	END();
	PRT("connect");
	if (!conn) {
		fprintf(stderr, "Failed to connect: %s\n", err.message);
		dbus_error_free(&err);
		return 1;
	}

	objs = get_objects(conn, argv[1]);

	get_objs_value(conn, objs, argv[1]);

	free_objs(objs);
	dbus_connection_unref(conn);

	return 0;
}
