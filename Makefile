CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

all: libhashmap.so

libhashmap.so: hashmap.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o libhashmap.so hashmap.c -lpthread

clean:
	rm -f libhashmap.so