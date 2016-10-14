#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dbus/dbus.h>

static int verbose;
static double create_time;

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

static double get_double(DBusMessageIter *iter)
{
	DBusMessageIter subiter;
	int stype;
	double db;

	dbus_message_iter_recurse(iter, &subiter);
	while ((stype = dbus_message_iter_get_arg_type(&subiter)) != DBUS_TYPE_INVALID) {
		switch (stype) {
		case DBUS_TYPE_DOUBLE:
			dbus_message_iter_get_basic(&subiter, &db);
			return db;
		default:
			break;
		}
		dbus_message_iter_next(&subiter);
	}

	return 0.0;
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
			if (verbose)
				printf("\tString: %s\n", arg);
			break;
		case DBUS_TYPE_DOUBLE:
			dbus_message_iter_get_basic(&subiter, &db);
			if (verbose)
				printf("\tDouble: %lf\n", db);
			break;
		case DBUS_TYPE_INT16:
		case DBUS_TYPE_INT32:
			dbus_message_iter_get_basic(&subiter, &i);
			if (verbose)
				printf("\tInt: %u\n", i);
			break;
		case DBUS_TYPE_UINT16:
		case DBUS_TYPE_UINT32:
			dbus_message_iter_get_basic(&subiter, &ui);
			if (verbose)
				printf("\tUint: %u\n", ui);
			break;
		case DBUS_TYPE_VARIANT:
			printVariant(&subiter);
			break;
		default:
			if (verbose)
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
			if (verbose)
				printf("\tString: %s\n", arg);
			if (!strcmp(arg, "Time")) {
				dbus_message_iter_next(&subiter);
				create_time = get_double(&subiter);
				if (verbose)
					printf("\tDouble: %lf\n", create_time);
			}
			break;
		case DBUS_TYPE_VARIANT:
			printVariant(&subiter);
			break;
		default:
			if (verbose)
				printf("\tType: %c\n", stype);
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
			if (verbose)
				printf("\tType: %c\n", stype);
			break;
		}
		dbus_message_iter_next(&subiter);
	}
}

static DBusHandlerResult signal_filter(DBusConnection *conn,
		DBusMessage *msg, void *data)
{
	int r;
	const char *member;
	int type;
	DBusMessageIter iter;
	struct timespec ts;
	double cur_ms;

	r = dbus_message_get_type(msg);
	if (r != DBUS_MESSAGE_TYPE_SIGNAL)
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	member = dbus_message_get_member(msg);
	if (strcmp(member, "PropertiesChanged"))
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	cur_ms = ts.tv_sec + ts.tv_nsec / (double)1000000000;

	if (verbose)
		printf("Received: %s\n", member);

	dbus_message_iter_init(msg, &iter);
	while ((type = dbus_message_iter_get_arg_type(&iter)) != DBUS_TYPE_INVALID) {
		char *arg;

		switch (type) {
		case DBUS_TYPE_STRING:
			dbus_message_iter_get_basic(&iter, &arg);
			if (verbose)
				printf("\tString: %s\n", arg);
			break;
		case DBUS_TYPE_ARRAY:
			printArray(&iter);
			break;
		default:
			if (verbose)
				printf("\tType: %c\n", type);
			break;
		}
		dbus_message_iter_next(&iter);
	}

	printf("T [%16s] %lf sec = %lf - %lf\n", "Round trip time", cur_ms - create_time, cur_ms, create_time);
	// printf("[Round trip time]%lf (sec) = %lf - %lf\n", cur_ms - create_time, cur_ms, create_time);

	return DBUS_HANDLER_RESULT_HANDLED;
}

int main(int argc, char **argv)
{
	DBusConnection *conn;
	DBusError err;
	GList *objs;
	int r;

	if (argc < 2) {
		printf("usage) %s object_name\n", argv[0]);
		return 1;
	}
	if (argc > 2)
		verbose = 1;

	dbus_error_init(&err);
	START();
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (!conn) {
		fprintf(stderr, "Failed to connect: %s\n", err.message);
		dbus_error_free(&err);
		return 1;
	}
	END();
	PRT("connect");

	objs = get_objects(conn, argv[1]);
	if (!objs) {
		dbus_connection_unref(conn);
		return 1;
	}

	snprintf(_sig_name, sizeof(_sig_name), "org.automotive.%s", argv[1]);

	dbus_bus_add_match(conn, "type='signal',interface='org.freedesktop.DBus.Properties'", &err);
	dbus_connection_flush(conn);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Match error: %s\n", err.message);
		dbus_error_free(&err);
		free_objs(objs);
		dbus_connection_unref(conn);
		return 1;
	}

	r = dbus_connection_add_filter(conn, signal_filter, NULL, NULL);
	if (r == 0) {
		fprintf(stderr, "Add filter error\n");
		free_objs(objs);
		dbus_connection_unref(conn);
		return 1;
	}

	while (dbus_connection_read_write_dispatch(conn, 25000));

	free_objs(objs);
	dbus_connection_unref(conn);

	return 0;
}
