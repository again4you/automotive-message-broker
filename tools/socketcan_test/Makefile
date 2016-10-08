PKG_GIO=`pkg-config --cflags --libs gio-2.0`
CFLAGS=-Wall -g 
TARGET=test

all:
	g++ -o $(TARGET) test.c $(PKG_GIO) $(CFLAGS)

clean:
	rm -f $(TARGET)
