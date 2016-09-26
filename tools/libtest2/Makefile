PKG_GIO=`pkg-config --cflags --libs gio-2.0`
CFLAGS=-Wall -g -I../../lib -I. -std=c++11

TARGET=test

all:
	g++ -o $(TARGET) libamb-client.cpp test.cpp $(PKG_GIO) $(CFLAGS) -lamb

clean:
	rm -f $(TARGET)
