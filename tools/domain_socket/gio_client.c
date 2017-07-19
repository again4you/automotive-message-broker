#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#define SOCK_PATH "/tmp/socket_file"
#define BUFSIZE 1024

typedef struct {
	unsigned int mode;
	unsigned int object;
	unsigned int size;
} msg_hdr_t;

typedef struct {
	msg_hdr_t msg_head;
	int value;
} msg_req_t;

typedef struct {
	guint8	*data;
	guint32	data_size;
	guint32 buf_size;
} msg_buf_t;

int main()
{
	GError *error;
	GSocket *socket;
	GSocketAddress *sock_addr;

	msg_req_t msg_rev;

	msg_req_t *msg_req;
	msg_hdr_t *msg_hdr;
	msg_req = malloc(sizeof(*msg_req));
	memset(msg_req, 0x00, sizeof(*msg_req));

	msg_hdr = (msg_hdr_t *)msg_req;
	msg_hdr->mode = 0;
	msg_hdr->object = 101;
	msg_hdr->size = sizeof(*msg_req);
	msg_req->value = 200;

	// socket create
	error = NULL;
	socket = g_socket_new(G_SOCKET_FAMILY_UNIX,
						G_SOCKET_TYPE_STREAM,
						G_SOCKET_PROTOCOL_DEFAULT,
						&error);
	if (error != NULL) {
		g_printerr("g_socket_new Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

	// server address setting
	sock_addr = g_unix_socket_address_new_with_type(SOCK_PATH,
												-1,
												G_UNIX_SOCKET_ADDRESS_PATH);

	// connect
	if (g_socket_connect(socket, sock_addr, NULL, &error) == FALSE) {
		g_printerr("g_socket_connect Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

	// request
	g_socket_send(socket, (const gchar*)msg_req, sizeof(msg_req_t), NULL, &error);
	if (error != NULL) {
		g_printerr("g_socket_send Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

	// get result
	g_socket_receive(socket, (gchar*)&msg_rev, sizeof(msg_req_t), NULL, &error);
	if (error) {
		g_printerr("g_socket_receive Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

	g_printerr("msg_head.mode: %d\n", msg_rev.msg_head.mode);
	g_printerr("Odometer value: %d\n", msg_rev.value);

	g_socket_close(socket, &error);
	if (error) {
		g_printerr("g_socket_close Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

	return 0;
}
