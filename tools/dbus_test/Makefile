PKG_GIO=`pkg-config --cflags --libs glib-2.0 gio-2.0`
PKG_DBUS=`pkg-config --cflags --libs dbus-1 glib-2.0`
CFLAGS=-Wall -g

TARGET=listen_gio
TARGET2=listen
DAEMON=daemon

all: $(TARGET) $(TARGET2) ${DAEMON}

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG_GIO) $(CFLAGS)

$(TARGET2): % : %.c
	$(CC) -o $@ $@.c $(PKG_DBUS) $(CFLAGS)

$(DAEMON): % : %.c
	$(CC) -o $@ $@.c $(PKG_GIO) $(CFLAGS)

clean:
	rm -f $(TARGET) $(TARGET2) $(DAEMON)



# HowTo
# 1) run daemon
# 2) dbus-send --type=signal --system /Com/Samsung/Test com.samsung.test.Print
