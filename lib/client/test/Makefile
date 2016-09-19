# Makefile

PKG_CFG=`pkg-config --cflags --libs ambd-client`
CFLAGS=-Wall -g

TARGET=test

all: $(TARGET)

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG_CFG) $(CFLAGS)
