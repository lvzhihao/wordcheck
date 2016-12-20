include ../include.mk

all:
	gcc -g -O2 -fPIC -o wordcheck wordcheck.c -L../src/.libs -I../src -pthread -lwordcheck -lm `pkg-config --cflags --libs glib-2.0`	

install: ./wordcheck.c
	install -c -d ${DESTDIR}${PREFIX}/bin
	install -c -m 0755 wordcheck ${DESTDIR}${PREFIX}/bin/wordcheck
	
clean:
	rm -rf wordcheck
