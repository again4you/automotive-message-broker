# Makefile

PKG=`pkg-config --cflags --libs dbus-1 glib-2.0`
CFLAGS=-Wall -g

TARGET=perf_amb

all: $(TARGET) $(TARGET2)

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG) $(CFLAGS)

clean:
	rm -f $(TARGET)
