# Makefile

PKG_CFG=`pkg-config --cflags --libs amb-client`
CFLAGS=-Wall -g

TARGET=test

all: $(TARGET)

$(TARGET): % : %.c
	$(CC) -o $@ $@.c $(PKG_CFG) $(CFLAGS)

clean:
	rm -rf ${TARGET}
