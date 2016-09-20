PKG_GIO=`pkg-config --cflags --libs gio-2.0`
CFLAGS=-Wall -g -I../../lib

TARGET=libamb-client

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG_GIO) $(CFLAGS)

clean:
	rm -f $(TARGET)
