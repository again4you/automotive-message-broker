# Makefile

PKG_GIO=`pkg-config --cflags --libs gio-2.0`
PKG_DBUS=`pkg-config --cflags --libs dbus-1 glib-2.0`
CFLAGS=-Wall -g

TARGET=get_gio listen_gio

TARGET2=get listen

all: $(TARGET) $(TARGET2)

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG_GIO) $(CFLAGS)

$(TARGET2): % : %.c
	$(CC) -o $@ $@.c $(PKG_DBUS) $(CFLAGS)

clean:
	rm -f $(TARGET) $(TARGET2)
