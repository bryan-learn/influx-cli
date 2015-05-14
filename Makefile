CC = gcc

CFLAGS = -g -Wall -I./lib/
LIBS = -lcurl

influx-cli: src/libinflux.c src/main.c
	$(CC) $(CFLAGS) src/libinflux.c src/main.c $(LIBS) -o influx-cli


clean:
	-rm -f influx-cli src/*.o src/*.a src/a.out src/core src/core.* src/*.core
