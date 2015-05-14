CC = gcc

CFLAGS = -g -Wall -I./lib/
LIBS = -lcurl

influxctl: src/libinflux.c src/main.c
	$(CC) $(CFLAGS) src/libinflux.c src/main.c $(LIBS) -o influxctl


clean:
	-rm -f influxctl src/*.o src/*.a src/a.out src/core src/core.* src/*.core
