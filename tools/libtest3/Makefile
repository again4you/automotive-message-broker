PKG_GIO=`pkg-config --cflags --libs gio-2.0`
CFLAGS=-Wall -g -I../../lib -I.

TARGET=test
all:
	gcc -o $(TARGET) libamb-client.c test.c libamb-util.c $(PKG_GIO) $(CFLAGS) -lamb
clean:
	rm -f $(TARGET)
