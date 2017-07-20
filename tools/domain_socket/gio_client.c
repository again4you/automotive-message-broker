#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#define SOCK_PATH "/tmp/amb_socket"
#define BUFSIZE 1024

struct udsmsg_prop {
	int32_t zone;
	uint32_t len;
	uint8_t name[0];
};

struct udsmsg_val {
	int32_t res;
	uint16_t type; /* gvariant type char */
	uint16_t len;
	uint8_t val[0];
};

enum udsmsg_type {
	UT_GET_REQ,
	UT_GET_RES,
	UT_SET_REQ,
	UT_SET_RES,
	UT_END,
};

struct udsmsg_head {
	uint32_t type; /* enum UDSMSG_TYPE */
	uint32_t len;
};

static int udsmsg_fill_get_req(uint8_t *buf, uint32_t size,
		int zone, const char *name)
{
	int len;
	struct udsmsg_head *uh;
	struct udsmsg_prop *up;

	if (!buf || size == 0 || !name)
		return -1;

	len = strlen(name) + 1;
	if (len + sizeof(*uh) + sizeof(*up) > size)
		return -1;

	up = (void *)buf + sizeof(*uh);
	up->zone = zone;
	up->len = len;
	memcpy(up->name, name, len);

	uh = (void *)buf;
	uh->type = UT_GET_REQ;
	uh->len = sizeof(*uh) + sizeof(*up) + len;

	return uh->len;
}

static int udsmsg_fill_set_req(uint8_t *buf, uint32_t size,
		int zone, const char *name,
		uint8_t vtype, uint16_t vsz, void *value)
{
	int len;
	struct udsmsg_head *uh;
	struct udsmsg_prop *up;
	struct udsmsg_val *uv;

	if (!buf || size == 0 || !name || !value)
		return -1;

	len = strlen(name) + 1;
	if (len + vsz + sizeof(*uh) + sizeof(*up) + sizeof(*uv) > size)
		return -1;

	uv = (void *)buf + sizeof(*uh) + sizeof(*up) + len;
	uv->res = 0;
	uv->type = vtype;
	uv->len = vsz;
	memcpy(uv->val, value, vsz);

	up = (void *)buf + sizeof(*uh);
	up->zone = zone;
	up->len = len;
	memcpy(up->name, name, len);

	uh = (void *)buf;
	uh->type = UT_SET_REQ;
	uh->len = sizeof(*uh) + sizeof(*up) + len + sizeof(*uv) + vsz;

	return uh->len;
}

int main(int argc, char *argv[])
{
    GError *error;
    GSocket *socket;
    GSocketAddress *sock_addr;

    gssize nread;
    char buf[BUFSIZE] = {0, };
    gchar bufread[BUFSIZE] = {0, };

    int zone;

    if (argc < 3) {
        printf(" usage) %s {set|get} zone name vtype value\n", argv[0]);
        printf("   zone:\n");
        printf("     None = 0,\n");
        printf("     Front = 1,\n");
        printf("     Middle = 2,\n");
        printf("     Right = 4,\n");
        printf("     Left = 8,\n");
        printf("     Rear = 16,\n");
        printf("     Center = 32,\n");
        printf("     LeftSide = 64,\n");
        printf("     RightSide = 128,\n");
        printf("     FrontSide = 256,\n");
        printf("     BackSide = 512\n");
	printf("   vtype - i, s, f\n");
	return 1;
    }

    error = NULL;
    socket = g_socket_new(G_SOCKET_FAMILY_UNIX,
                        G_SOCKET_TYPE_STREAM,
                        G_SOCKET_PROTOCOL_DEFAULT,
                        &error);
    if (error) {
        g_printerr("Fail to g_socket_new: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    sock_addr = g_unix_socket_address_new_with_type(SOCK_PATH,
                                        -1, G_UNIX_SOCKET_ADDRESS_PATH);
    if (!g_socket_connect(socket, sock_addr, NULL, &error)) {
        g_printerr("Fail to g_socket_connect: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

	zone = atoi(argv[2]);
	if (!strcmp(argv[1], "set")) {
		void *v;
		int vsz;
		int v_i;
		double v_f;

		switch(argv[4][0]) {
			case 's':
				v = argv[5];
				vsz = strlen(v);
				break;
			case 'i':
				v_i = atoi(argv[5]);
				v = &v_i;
				vsz = sizeof(v_i);
				break;
			case 'f':
				v_f = atof(argv[5]);
				v = &v_f;
				vsz = sizeof(v_f);
				break;
			default:
				g_printerr("Unknown value type: '%c'\n", argv[4][0]);
				return 1;
		}

		nread = udsmsg_fill_set_req((uint8_t *)buf, sizeof(buf), zone, argv[3],
				argv[4][0], vsz, v);
	} else if (!strcmp(argv[1], "get")) {
		nread = udsmsg_fill_get_req((uint8_t *)buf, sizeof(buf), zone, argv[3]);
	} else {
		g_printerr("Unknown command: %s\n", argv[1]);
		return 1;
	}

    g_socket_send(socket, buf, nread, NULL, &error);
    if (error) {
        g_printerr("Fail to g_socket_send: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

	{
		struct udsmsg_head uh;
		struct udsmsg_prop *up;
		struct udsmsg_val *uv;

		nread = g_socket_receive(socket, (gchar *)&uh, sizeof(uh), NULL, &error);
		if (error) {
			g_printerr("Fail to g_socket_receive: %s\n", error->message);
			g_error_free(error);
			return -1;
		}
		printf("Recv: [%s] %d\n", uh.type == UT_SET_RES ? "Set" : "Get", uh.len);

		nread = g_socket_receive(socket, bufread, uh.len - sizeof(uh), NULL, &error);
		if (error) {
			g_printerr("Fail to g_socket_receive: %s\n", error->message);
			g_error_free(error);
			return -1;
		}

		// up = (void *)bufread;
		up = (struct udsmsg_prop *)bufread;
		printf(" zone %d len %u [%s]\n", up->zone, up->len, up->name);

		uv = (void *)bufread + sizeof(*up) + up->len;
		printf(" type '%c' len %u", uv->type, uv->len);
		switch (uv->type) {
			case 's':
				printf(" [%s]\n", uv->val);
				break;
			case 'i':
				printf(" [%d]\n", *(int *)uv->val);
				break;
			case 'f':
				printf(" [%lf]\n", *(double *)uv->val);
				break;
			default:
				printf(" Unknown\n");
				break;
		}
	}

	g_socket_close(socket, &error);
	if (error) {
        g_printerr("Fail to g_socket_close: %s\n", error->message);
        g_error_free(error);
        return -1;
    }
    g_object_unref(sock_addr);

    return 0;
}
