#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

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

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    return x->tv_sec < y->tv_sec;
}

int main(int argc, char **argv)
{
	GError *error;
	GSocket *socket;
	GSocketAddress *sock_addr;

    struct timeval start_time, end_time, diff_time;

	msg_req_t msg_rev;
	msg_req_t msg_req;
	msg_hdr_t *msg_hdr;
	memset(&msg_req, 0x00, sizeof(msg_req_t));

    if (argc < 3) {
        g_printerr("%s: [OP] [OBJECT_NUM]\n", argv[0]);
        return 0;
    }

	msg_hdr = (msg_hdr_t *)&msg_req;
	msg_hdr->mode = atoi(argv[1]);
    msg_hdr->object = atoi(argv[2]);
	msg_hdr->size = sizeof(msg_req_t);
    if (msg_hdr->mode == 1)
        msg_req.value = atoi(argv[3]);

    // START TIME MEASURE
    gettimeofday(&start_time, NULL);
    
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
	g_socket_send(socket, (const gchar*)&msg_req, sizeof(msg_req_t), NULL, &error);
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

    // END TIME MEASURE
    gettimeofday(&end_time, NULL);

	g_socket_close(socket, &error);
	if (error) {
		g_printerr("g_socket_close Error: %s\n", error->message);
		g_error_free(error);
		return -1;
	}

    timeval_subtract(&diff_time, &end_time, &start_time);
	g_printerr("msg_head.mode: %d\n", msg_rev.msg_head.mode);
	g_printerr("Odometer value: %d\n", msg_rev.value);
    g_printerr("%ld.%06ld sec\n", diff_time.tv_sec, diff_time.tv_usec);

	return 0;
}
