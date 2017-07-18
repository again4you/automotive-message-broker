CFLAGS=-Wall -g
LDFLAGS=

CLIENT=gio_client

all: $(CLIENT)

$(CLIENT): gio_client.c
		$(CC) -o $@ $@.c `pkg-config --cflags --libs gio-2.0 gio-unix-2.0` $(CFLAGS) $(LDFLAGS)
clean:
		rm -rf $(CLIENT)
