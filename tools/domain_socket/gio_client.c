#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#define SOCK_PATH "/tmp/socket_file"
#define BUFSIZE 1024

int main()
{
	GError *error;
	GSocket *socket;
	GSocketAddress *sock_addr;

	gssize nread;
	char buf[BUFSIZE] = {0, };
	char bufread[BUFSIZE] = {0, };

	// socket create
	error = NULL;
	socket = g_socket_new(G_SOCKET_FAMILY_UNIX,
						G_SOCKET_TYPE_STREAM,
						G_SOCKET_PROTOCOL_DEFAULT,
						&error);
	if (error != NULL) {
		g_printerr("g_socket_new Error: %s\n", error->message);
		return -1;
	}

	// server address setting
	// sock_addr = g_unix_socket_address_new(SOCK_PATH);
	sock_addr = g_unix_socket_address_new_with_type(SOCK_PATH,
												-1,
												G_UNIX_SOCKET_ADDRESS_PATH);
	// connect
	if (g_socket_connect(socket, sock_addr, NULL, &error) == FALSE) {
		g_printerr("g_socket_connect Error: %s\n", error->message);
		return -1;
	}

	// send
	snprintf(buf, sizeof(buf), "Test Message");
	g_socket_send(socket, buf, strlen(buf), NULL, &error);
	if (error != NULL) {
		g_printerr("g_socket_send Error: %s\n", error->message);
		return -1;
	}

    nread = g_socket_receive(socket, bufread, sizeof(bufread), NULL, &error);
    if (error != NULL) {
        g_printerr("g_socket_receive Error: %s\n", error->message);
        return -1;
    }

    printf("Received Msg: %s\n", bufread);

    g_socket_close(socket, &error);
    if (error != NULL) {
		g_printerr("g_socket_close Error: %s\n", error->message);
		return -1;
    }
    g_object_unref(sock_addr);

	
	return 0;
}
